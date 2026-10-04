#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "allocator.h"
#include "../video/video.h"
#include "../str.h"

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

void init_alloc_driver(uint64_t e820_count) {
    MMEmmap* mmap = (MMEmmap*)0x6000;

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
    }

    reserve_range(0x0, 0x200000);
    reserve_range(0x10000, 0x200000);
    reserve_range(0x90000, 0xA0000);
    reserve_range(0xB8000, 0xC0000);
    
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

