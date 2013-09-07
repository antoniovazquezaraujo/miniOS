/*                   Mammoth Kernel Experiment 0.2                 */
/*  Kernel Experiment, development path for Mammoth 0.0.1 Alpha    */
/*                         By Callum Fare                          */
/*    Created 14/01/09, last significant modification 14/01/09     */
#include "system.h"
#define whitetxt 0x0F

void kernelAbort() {
	k_clear_screen();
	k_printlineN("MAMMOTH SYSTEM EXCEPTION", whitetxt);
	k_printlineN("You bastard, you made it crash. What was that for? Hmm?", greytxt);
	k_printlineN("Pff, restart the computer or something.", greytxt);
	k_printlineN(" (One of the kernel functions terminated unexpectedly) ", greytxt);
	return;
}








