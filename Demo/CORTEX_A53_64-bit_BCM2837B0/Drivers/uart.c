/**
 *	UART
 *
 *	
 **/
#include "regs.h"


//-------------------------------------------------------------------
void uart_send ( unsigned int c )
{
	while(1)
	{
		if(ioread32(AUX_MU_LSR_REG) & 0x20)
			break;
	}
	iowrite32(c, AUX_MU_IO_REG);
}
//-------------------------------------------------------------------
void uart_flush ( void )
{
	while(1)
	{
		if((ioread32(AUX_MU_LSR_REG) & 0x100) == 0)
			break;
	}
}
//-------------------------------------------------------------------
void hexstrings ( unsigned int d )
{
    //unsigned int ra;
    unsigned int rb;
    unsigned int rc;

    rb=32;
    while(1)
    {
        rb-=4;
        rc=(d >> rb) & 0xF;
        if(rc > 9)
		rc += 0x37;
       	else rc+=0x30;
        	uart_send(rc);
        if(rb==0) break;
    }
    uart_send(0x20);
}
//-------------------------------------------------------------------
void hexstring ( unsigned int d )
{
    hexstrings(d);
    uart_send(0x0D);
    uart_send(0x0A);
}

void print( char* ch)
{
	while (*ch!=0)
	{
		uart_send((unsigned int)*ch);
		ch++;
	}
	uart_send(0x0D);
	uart_send(0x0A);
}
//-------------------------------------------------------------------
void uart_init ( void )
{
	unsigned int ra;

	
	iowrite32(ioread32(AUX_ENABLES)|1, AUX_ENABLES);
//	iowrite32(0, AUX_MU_IER_REG);
//	iowrite32(0, AUX_MU_CNTL_REG);
	iowrite32(3, AUX_MU_LCR_REG);
//	iowrite32(0, AUX_MU_MCR_REG);

//	iowrite32(0, AUX_MU_IER_REG);
	//iowrite32(0x5, AUX_MU_IER_REG); //For UART interrupt.
	 
//	iowrite32(0xC6, AUX_MU_IIR_REG);
	iowrite32(270, AUX_MU_BAUD_REG);
	ra = ioread32(GPFSEL1);
	iowrite32(ioread32(GPFSEL1) & (~((7 << 12) | (7 << 15))), GPFSEL1);
	iowrite32(ioread32(GPFSEL1) | ((2 << 12) | (2 < 15)), GPFSEL1);//GPIO14 GPIO15 ALT5
	iowrite32(0, GPPUD);
	for(ra = 0; ra < 150; ra++)
		asm volatile ("nop");
	iowrite32((2 << 14) | (2 << 15), GPPUDCLK0);
	for(ra = 0; ra < 150; ra++)
		asm volatile ("nop");
	iowrite32(0, GPPUDCLK0);
	iowrite32(3, AUX_MU_CNTL_REG);
}

