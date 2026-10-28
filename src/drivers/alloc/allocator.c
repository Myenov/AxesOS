#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../../lib/memory/allocator.h"

#define PML4_PHYS   0x1000
#define PDPT_PHYS   0x2000
#define PD0_PHYS    0x3000
#define PAGE_SIZE   0x1000
#define HUGE_PAGE   0x200000
#define GB          0x40000000

static uint64_t next_free_phys = 0x150000;

typedef struct {
    uint64_t base_addr;
    uint64_t length;
    uint32_t type;
    uint32_t acpiext;
} __attribute__((packed)) MMEmmap;

volatile uint64_t all_ram = 0;
volatile uint64_t use_ram = 0;

MMEmmap* e820buffer[E820_MAX_ENTRIES];
uint16_t e820t1count = 0;

typedef struct Node {
    struct Node* next;
    struct Node* prev;
    bool free;
    int level;
} Node;

Node* free_lists[MAX_LEVEL] = {NULL};

void add_list(int level, Node* node) {
    node->level = level;
    node->free = true;
    node->prev = NULL;
    node->next = free_lists[level];
    if (free_lists[level]) free_lists[level]->prev = node;
    free_lists[level] = node;
}

static void remove_list(int level, Node* node) {
    if (node->prev) node->prev->next = node->next;
    else            free_lists[level]   = node->next;
    if (node->next) node->next->prev = node->prev;
    node->next = node->prev = NULL;
    node->free = false;
}

static uint64_t alloc_page_phys(void) {
    uint64_t p = next_free_phys;
    next_free_phys += PAGE_SIZE;
    uint8_t *ptr = (uint8_t*)p;
    for (uint16_t i = 0; i < 4096; i++) ptr[i] = 0;
    return p;
}

static void map_huge2mb(uint64_t phys_addr) {
    uint64_t pml4_idx = (phys_addr >> 39) & 0x1FF;
    uint64_t pdpt_idx = (phys_addr >> 30) & 0x1FF;
    uint64_t pd_idx   = (phys_addr >> 21) & 0x1FF;

    uint64_t *pml4 = (uint64_t*)PML4_PHYS;
    if (!(pml4[pml4_idx] & 1)) {
        uint64_t pdpt = alloc_page_phys();
        pml4[pml4_idx] = pdpt | 0x03; // pw
    }
    uint64_t *pdpt = (uint64_t*)(pml4[pml4_idx] & ~0xFFFULL);
    
    if (!(pdpt[pdpt_idx] & 1)) {
        uint64_t pd = alloc_page_phys();
        pdpt[pdpt_idx] = pd | 0x03;
    }
    uint64_t *pd = (uint64_t*)(pdpt[pdpt_idx] & ~0xFFFULL);
    pd[pd_idx] = (phys_addr & ~0x1FFFFFULL) | 0x83;
}

static void flush_tlb(void) {
    uint64_t cr3;
    asm volatile("mov %%cr3, %0" : "=r"(cr3));
    asm volatile("mov %0, %%cr3" : : "r"(cr3) : "memory");
}

void map_all_ram(uint64_t max_addr) {
    max_addr = (max_addr + HUGE_PAGE - 1) & ~(HUGE_PAGE - 1);
    if (max_addr < 0x100000000ULL) max_addr = 0x100000000ULL;

    for (uint64_t addr = GB; addr < max_addr; addr += HUGE_PAGE) {
        map_huge2mb(addr);
    }

    flush_tlb();
}

void init_alloc_driver(uint64_t e820_count, uintptr_t ebda_address, uint8_t ebda_size) {
    MMEmmap* mmap = (MMEmmap*)0x6000;
    uint64_t max_addr = 0;
    
    for (uint32_t i = 0; i < e820_count; i++) {
        all_ram += mmap[i].length;

        if (mmap[i].type == 1) {
            e820buffer[e820t1count] = &mmap[i];
            e820t1count++;
        
            use_ram += mmap[i].length;
        
            uint64_t start = mmap[i].base_addr;
            uint64_t end = mmap[i].base_addr + mmap[i].length;
        
            if (end <= 0x200000)
                continue;
        
            if (start < 0x200000)
                start = 0x200000;
        
            start = (start + MAX_BLOCK_SIZE - 1) &
                    ~(uint64_t)(MAX_BLOCK_SIZE - 1);
        
            end = end &
                  ~(uint64_t)(MAX_BLOCK_SIZE - 1);
        
            for (uint64_t p = start;
                 p + MAX_BLOCK_SIZE <= end;
                 p += MAX_BLOCK_SIZE) {
        
                add_list(MAX_LEVEL - 1, (Node*)p);
            }
        }
        uint64_t end = mmap[i].base_addr + mmap[i].length;
        if (end > max_addr) max_addr = end;
    }
    
    if (max_addr > GB) {
        map_all_ram(max_addr);
    }
    reserve_range(0x4000, 0x100000);
    reserve_range(0x0, 0x200000);
    reserve_range(0x10000, 0x200000);
    reserve_range(0x90000, 0xA0000);
    reserve_range(0xB8000, 0xC0000);
    reserve_range(0x000E0000, 0x000FFFFF);
    reserve_range(0x00000, 0x003FF); // IVT
    reserve_range(0x00400, 0x004FF); //BDA
    reserve_range(0x00500, 0x9FBFF);
    reserve_range(ebda_address, ebda_address+(ebda_size*1024)); //EBDA
    reserve_range(0xA0000, 0xBFFFF); //VGA
    reserve_range(0xC0000, 0xFFFFF); //BIOS area
}

void* buddy_alloc(uint64_t size) {
    if (size == 0 || size > MAX_BLOCK_SIZE)
        return NULL;

    int need = 0;
    uint64_t s = MIN_BLOCK_SIZE;

    while (s < size) {
        s <<= 1;
        need++;
    }

    int level = need;

    while (level < MAX_LEVEL && free_lists[level] == NULL)
        level++;

    if (level == MAX_LEVEL)
        return NULL;

    Node* node = free_lists[level];
    remove_list(level, node);

    uint64_t addr = (uint64_t)node;

    while (level > need) {
        level--;

        uint64_t buddy =
            addr + ((uint64_t)MIN_BLOCK_SIZE << level);

        add_list(level, (Node*)buddy);
    }

    return (void*)addr;
}

static bool take_block(uint64_t addr, int level) {
    Node* n = free_lists[level];
    while (n && (uint64_t)n != addr) n = n->next;
    if (!n) return false;
    remove_list(level, n);
    return true;
}

void buddy_free(void* ptr, uint64_t size) {
    if (!ptr || size == 0) return;

    int need = 0;
    uint64_t s = MIN_BLOCK_SIZE;
    while (s < size) { s <<= 1; need++; }
   
    uint64_t addr = (uint64_t)ptr;
    int lvl = need;

    while (lvl < MAX_LEVEL - 1) {
        uint64_t block_size = (uint64_t)MIN_BLOCK_SIZE << lvl;
        uint64_t buddy_addr = addr ^ block_size;

        Node* b = free_lists[lvl];
        while (b && (uint64_t)b != buddy_addr) b = b->next;
        if (!b) break;

        remove_list(lvl, b);
        addr = (addr < buddy_addr) ? addr : buddy_addr;
        lvl++;
    }

    add_list(lvl, (Node*)addr);
}

void reserve_range(uint64_t start, uint64_t end) {
    start = (start + MIN_BLOCK_SIZE - 1) & ~(uint64_t)(MIN_BLOCK_SIZE - 1);
    end   = end & ~(uint64_t)(MIN_BLOCK_SIZE - 1);

    while (start < end) {
        int lvl = MAX_LEVEL - 1;
        while (lvl > 0) {
            uint64_t bs = (uint64_t)MIN_BLOCK_SIZE << lvl;
            if ((start & (bs - 1)) == 0 && start + bs <= end) break;
            lvl--;
        }
        uint64_t bs = (uint64_t)MIN_BLOCK_SIZE << lvl;
        bool taken = false;
        for (int L = lvl; L < MAX_LEVEL && !taken; L++) {
            uint64_t base = start & ~(((uint64_t)MIN_BLOCK_SIZE << L) - 1);
            if (take_block(base, L)) {
                int cur = L;
                uint64_t cur_addr = base;
                while (cur > lvl) {
                    cur--;
                    uint64_t buddy = cur_addr + ((uint64_t)MIN_BLOCK_SIZE << cur);
                    add_list(cur, (Node*)buddy);
                }
                taken = true;
            }
        }

        if (!taken) {}
        start += bs;
    }
}

uint64_t get_all_ram() {
    return all_ram;
}

uint64_t get_use_ram() {
    return use_ram;
}

