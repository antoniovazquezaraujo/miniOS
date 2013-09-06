/*                   Mammoth Kernel Experiment 0.2                 */
/*  Kernel Experiment, development path for Mammoth 0.0.1 Alpha    */
/*                         By Callum Fare                          */
/*    Created 14/01/09, last significant modification 14/01/09     */
/*								   */
/* File: /include/system.h                                         */
/* This is the core library of the kernel. It contains all the     */
/* functions for the kernel, which are called by the sys components*/
#ifndef __SYSTEM_H
#define __SYSTEM_H

#define whitetxt 0x0F 
#define greytxt 0x07

// Global variables



typedef int size_t;

struct regs
{
    unsigned int gs, fs, es, ds;
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
    unsigned int int_no, err_code;
    unsigned int eip, cs, eflags, useresp, ss;    
};

// Function predefinitions====
//Display
extern void k_clear_screen();
extern void k_scrollText();
extern unsigned int k_printf(char *message, unsigned int line, unsigned char colour);
extern void k_printline(unsigned char c);
extern void k_printstr(unsigned char *text);
//extern void update_cursor(int row, int col);

//Core Functions
extern void *memcpy(void *dest, const void *src, size_t count);
extern void *memset(void *dest, char val, size_t count);
extern unsigned short *memsetw(unsigned short *dest, unsigned short val, size_t count);
extern size_t strlen(const char *str);
extern unsigned char inportb (unsigned short _port);
extern void outportb (unsigned short _port, unsigned char _data);


//GDT (Functions are fully defined in gdt.c)
extern void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran);
extern void gdt_install();

//IDT
extern void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags);
extern void idt_install();

//ISRs
extern void isrs_install();

//IRQs
extern void irq_install_handler(int irq, void (*handler)(struct regs *r));
extern void irq_uninstall_handler(int irq);
extern void irq_install();

//Clock
extern void timer_wait(int ticks);
extern void timer_install();

//beep
extern void beep(unsigned int Frequency, unsigned int Duration);
//=============================	




extern void keyboard_install();



/////////////
//FUNCTIONS//
/////////////






#endif

