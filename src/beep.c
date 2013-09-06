// will it work?

#include <system.h>

void StartSound(unsigned int Frequency)
{
   unsigned int Divisor = 1193180 / Frequency;

   outportb(0x43, 0xB6);
   outportb(0x42, Divisor & 0xFF);
   outportb(0x42, Divisor >> 8);

   outportb(0x61, inportb(0x61) | 3);
}

void StopSound()
{
   outportb(0x61, inportb(0x61) & ~3);
}

void beep(unsigned int Frequency, unsigned int Duration)
{
   k_printstr("Beepity beep");
   StartSound(Frequency);
   timer_wait(Duration);
   StopSound();
}
