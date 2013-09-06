echo Now compiling Mammoth..
echo " "
echo " "
gcc kernel.c  -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I./include -c -o kernel.o
gcc beep.c  -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I./include -c -o beep.o
gcc gdt.c  -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I./include -c -o gdt.o
gcc idt.c  -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I./include -c -o idt.o
gcc irq.c  -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I./include -c -o irq.o
gcc isrs.c  -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I./include -c -o isrs.o
gcc kb.c  -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I./include -c -o kb.o
gcc timer.c  -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I./include -c -o timer.o
nasm -f elf -o start.o start.asm
ld -T link.ld -o kernel.bin start.o kernel.o gdt.o idt.o isrs.o irq.o timer.o beep.o kb.o
mv kernel.bin ./build
cd build
cat stage1 stage2 pad kernel.bin > floppy.img
cd ..
echo " "
echo " "
echo "Mammoth is compiled (unless you find errors above; please check.)"
