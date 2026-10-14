Доброго времени суток. Я пишу свою OS.
Запускаю я через QEMU в WSL на Windows.

путь - путь до папки в WSL (как примеры) | 
путьВ - путь до папки в Windows | 
команда копирования до рабочей папки (если смотрите source в windows):
```sh
 cd ~/путь/ && rm -rf OS && cp -r /mnt/c/путьВ ./ && cd ~/путь/путь1 && rm -rf build && mkdir build && cd build && cmake .. && make
```

у меня:
```sh
 cd ~/projects/ && rm -rf OS && cp -r /mnt/c/users/myen/desktop/OS ./ && cd ~/projects/OS && rm -rf build && mkdir build && cd build && cmake .. && make
```
===========

QEMU:
```sh
qemu-system-x86_64 -drive file=AxesOS.img,format=raw,if=ide,index=0 -no-reboot -no-shutdown -d int,cpu_reset -D qemu.log
```
