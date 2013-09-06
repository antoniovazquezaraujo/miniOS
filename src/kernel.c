/*                      Mammoth Kernel 0.0.2                       */
/*                         By Callum Fare                          */
/*    Created 06/12/08, last significant modification 18/01/09     */
/*								   */
/* File: kernel.c                                                  */
/* Spawned by startup process, hosts kernel processes and drivers. */
/* kmain is invoked, which begins the kernel fully.                */


#include <system.h> //Contains all functions.


//Predefine some colour bytes to keep the code tidy:
#define whitetxt 0x0F 
#define greytxt 0x07

//And list functions for later on.
int lastLine = -1;
int csr_x = 0, csr_y = 0;
unsigned short *textmemptr = 0xB8000;
int attrib = 0x0F;

void move_csr(void)
{
    unsigned temp;

    /* The equation for finding the index in a linear
    *  chunk of memory can be represented by:
    *  Index = [(y * width) + x] */
    temp = csr_y * 80 + csr_x;

    /* This sends a command to indicies 14 and 15 in the
    *  CRT Control Register of the VGA controller. These
    *  are the high and low bytes of the index that show
    *  where the hardware cursor is to be 'blinking'. To
    *  learn more, you should look up some VGA specific
    *  programming documents. A great start to graphics:
    *  http://www.brackeen.com/home/vga */
    outportb(0x3D4, 14);
    outportb(0x3D5, temp >> 8);
    outportb(0x3D4, 15);
    outportb(0x3D5, temp);
}
	
void scroll(void)
{
    unsigned blank, temp;

    /* A blank is defined as a space... we need to give it
    *  backcolor too */
    blank = 0x20 | (attrib << 8);

    /* Row 25 is the end, this means we need to scroll up */
    if(csr_y >= 25)
    {
	//k_printline("SCROLLLL!!!! DAMN YOU!", greytxt);
        /* Move the current text chunk that makes up the screen
        *  back in the buffer by a line */
        temp = csr_y - 25 + 1;
        memcpy (textmemptr, textmemptr + temp * 80, (25 - temp) * 80 * 2);

        /* Finally, we set the chunk of memory that occupies
        *  the last line of text to our 'blank' character */
        memsetw (textmemptr + (25 - temp) * 80, blank, 80);
        csr_y = 25 - 1;
	
    }
}

void k_clear_screen() // This function will clear the screen of all characters by overwriting with a space character.
{
	char *vidmem = (char *) 0xb8000; // Set video memory location
	unsigned int i=0; // A simple integer for processing the amount of characters.
	while(i < (80*25*2)) // While we've not set the value of all of the characters.
	{
		vidmem[i]=' '; //Overwrite the character with a space
		i++;  //Move to the next character
		vidmem[i]=whitetxt; //Set the colour value of them all to White on Black.
		i++;  // And move on.
	}
    csr_x = 0;
    csr_y = 0;
    move_csr();
};



// This function will print character to the screen by copying them to the video memory.
void k_printline(unsigned char c)
{
    unsigned short *where;
    unsigned att = attrib << 8;

    /* Handle a backspace, by moving the cursor back one space */
    if(c == 0x08)
    {
        if(csr_x != 0) csr_x--;
    }
    /* Handles a tab by incrementing the cursor's x, but only
    *  to a point that will make it divisible by 8 */
    else if(c == 0x09)
    {
        csr_x = (csr_x + 8) & ~(8 - 1);
    }
    /* Handles a 'Carriage Return', which simply brings the
    *  cursor back to the margin */
    else if(c == '\r')
    {
        csr_x = 0;
    }
    /* We handle our newlines the way DOS and the BIOS do: we
    *  treat it as if a 'CR' was also there, so we bring the
    *  cursor to the margin and we increment the 'y' value */
    else if(c == '\n')
    {
        csr_x = 0;
        csr_y++;
    }
    /* Any character greater than and including a space, is a
    *  printable character. The equation for finding the index
    *  in a linear chunk of memory can be represented by:
    *  Index = [(y * width) + x] */
    else if(c >= ' ')
    {
        where = textmemptr + (csr_y * 80 + csr_x);
        *where = c | att;	/* Character AND attributes: color */
        csr_x++;
    }

    /* If the cursor has reached the edge of the screen's width, we
    *  insert a new line in there */
    if(csr_x >= 80)
    {
        csr_x = 0;
        csr_y++;
    }

    /* Scroll the screen if needed, and finally move the cursor */
    scroll();
    move_csr();
}

void k_printstr(unsigned char *text)
{
    int i;

    for (i = 0; i < strlen(text); i++)
    {
        k_printline(text[i]);
    }
}

unsigned int k_printold(char *message, unsigned char colour)
{
	char *vidmem = (char *) 0xb8000;
	unsigned int i=0;
	unsigned int currentLine = lastLine+1;

	i=(currentLine*80*2);
	
	while(*message!=0)
	{
		if(*message=='\n')
		{
			currentLine++;
			i=(currentLine*80*2);
			*message++;
		} else {
			vidmem[i]=*message;
			*message++;
			i++;
			vidmem[i]= colour;
			i++;
		};
	};
	lastLine = currentLine;
	return(1);
}


//KERNEL FUNCTIONS:

void *memcpy(void *dest, const void *src, size_t count) //Basic copy function
{
    const char *sp = (const char *)src;
    char *dp = (char *)dest;
    for(; count != 0; count--) *dp++ = *sp++;
    return dest;
}

void *memset(void *dest, char val, size_t count)
{
    char *temp = (char *)dest;
    for( ; count != 0; count--) *temp++ = val;
    return dest;
}

unsigned short *memsetw(unsigned short *dest, unsigned short val, size_t count)
{
    unsigned short *temp = (unsigned short *)dest;
    for( ; count != 0; count--) *temp++ = val;
    return dest;
}

size_t strlen(const char *str) //Caluclates the length of a string
{
    size_t retval;
    for(retval = 0; *str != '\0'; str++) retval++;
    return retval;
}

unsigned char inportb (unsigned short _port) //Major Input function.
{
    unsigned char rv;
    __asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (_port));
    return rv;
}

void outportb (unsigned short _port, unsigned char _data) //Major Output function.
{
    __asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
}



//////////////////
// MAIN KERNEL  //
/////////////////////////////////////////////////////////
// Everything should explain itself here, all functions//
// are commented further on in the program, we simply  //
// execute them here.                                  //
/////////////////////////////////////////////////////////

// Kernel Entry File jumps here:
void main()
{
	k_clear_screen();
	gdt_install();
	idt_install();
	isrs_install();
	irq_install();
	k_printstr("Descriptors up\n");
	k_printstr("No way! The mammoth is alive!\n");
        k_printstr("MAMMOTH 0.0.1 Build 26\nBy Callum Fare\n");
	//k_printstr("Should this work I shall go woohoo\n");
	//k_printstr("Best. Function. EVER!\n");
	//k_printstr("\n \n Now we need a keyboard driver!\n");
	//k_printstr("Hello?\n");
	keyboard_install();
	__asm__ __volatile__ ("sti");
	k_printstr("\n\n~~> ");

//	timer_install();
//	beep(410, 1);

} //if we reach here then main exits, and the loader regains control and halts the CPU.


//   end



















/*
unsigned int k_printf(char *message, unsigned int line, unsigned char colour)  // Set the variables for the message, line number and colour/blink settings.
{
	char *vidmem = (char *) 0xb8000; //Set video memory location.
	unsigned int i=0; //And an integer

	i=(line*80*2); // i is the byte number of the specified line (2 bytes to a char, 80 chars to a line)

	while(*message!=0) //While there's still stuff to print.
	{
		if(*message=='\n') //Check if a new line command has been reached
		{
			line++; // shift down a line.
			i=(line*80*2); //then recalculate i
			*message++; 
		} else { // When that new line has been processed we can push the message into the memory.
			vidmem[i]=*message; //Push the message into the memory (Current character is stored in 'i')
			*message++; //Next char.
			i++; //Move to the next char memory location
			vidmem[i]= colour; //And set colour data.
			i++; // And move on
		};
	};

	return(1); //Repeat this function until every single letter has been processed (when message == 0 )
};
*/







