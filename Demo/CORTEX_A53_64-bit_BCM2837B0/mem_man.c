/*
    FreeRTOS V7.2.0 - Copyright (C) 2012 Real Time Engineers Ltd.
	

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS tutorial books are available in pdf and paperback.        *
     *    Complete, revised, and edited pdf reference manuals are also       *
     *    available.                                                         *
     *                                                                       *
     *    Purchasing FreeRTOS documentation will not only help you, by       *
     *    ensuring you get running as quickly as possible and with an        *
     *    in-depth knowledge of how to use FreeRTOS, it will also help       *
     *    the FreeRTOS project to continue with its mission of providing     *
     *    professional grade, cross platform, de facto standard solutions    *
     *    for microcontrollers - completely free of charge!                  *
     *                                                                       *
     *    >>> See http://www.FreeRTOS.org/Documentation for details. <<<     *
     *                                                                       *
     *    Thank you for using FreeRTOS, and thank you for your support!      *
     *                                                                       *
    ***************************************************************************


    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation AND MODIFIED BY the FreeRTOS exception.
    >>>NOTE<<< The modification to the GPL is included to allow you to
    distribute a combined work that includes FreeRTOS without being obliged to
    provide the source code for proprietary components outside of the FreeRTOS
    kernel.  FreeRTOS is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
    or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details. You should have received a copy of the GNU General Public
    License and the FreeRTOS license exception along with FreeRTOS; if not it
    can be viewed here: http://www.freertos.org/a00114.html and also obtained
    by writing to Richard Barry, contact details for whom are available on the
    FreeRTOS WEB site.

    1 tab == 4 spaces!
    
    ***************************************************************************
     *                                                                       *
     *    Having a problem?  Start by reading the FAQ "My application does   *
     *    not run, what could be wrong?                                      *
     *                                                                       *
     *    http://www.FreeRTOS.org/FAQHelp.html                               *
     *                                                                       *
    ***************************************************************************

    
    http://www.FreeRTOS.org - Documentation, training, latest information, 
    license and contact details.
    
    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool.

    Real Time Engineers ltd license FreeRTOS to High Integrity Systems, who sell 
    the code with commercial support, indemnification, and middleware, under 
    the OpenRTOS brand: http://www.OpenRTOS.com.  High Integrity Systems also
    provide a safety engineered and independently SIL3 certified version under 
    the SafeRTOS brand: http://www.SafeRTOS.com.
*/


/*
 * The simplest possible implementation of pvPortMalloc().  Note that this
 * implementation does NOT allow allocated memory to be freed again.
 *
 * See heap_2.c, heap_3.c and heap_4.c for alternative implementations, and the 
 * memory management pages of http://www.FreeRTOS.org for more information.
 */
#include <stdlib.h>

#include "FreeRTOS.h"
#include "task.h"

void *memcpy(void *dest, const void *src, size_t count)
{
        unsigned long *dl = (unsigned long *)dest, *sl = (unsigned long *)src;
        char *d8, *s8;

        if (src == dest)
                return dest;

        /* while all data is aligned (common case), copy a word at a time */
        if ( (((unsigned long)dest | (unsigned long)src) & (sizeof(*dl) - 1)) == 0) {
                while (count >= sizeof(*dl)) {
                        *dl++ = *sl++;
                        count -= sizeof(*dl);
                }
        }
        /* copy the reset one byte at a time */
        d8 = (char *)dl;
        s8 = (char *)sl;
        while (count--)
                *d8++ = *s8++;

        return dest;
}

void *memset(void * s,int c,size_t count)
{
        unsigned long *sl = (unsigned long *) s;
        char *s8;

        unsigned long cl = 0;
        int i;

        /* do it one word at a time (32 bits or 64 bits) while possible */
        if ( ((unsigned long)s & (sizeof(*sl) - 1)) == 0) {
                for (i = 0; i < sizeof(*sl); i++) {
                        cl <<= 8;
                        cl |= c & 0xff;
                }
                while (count >= sizeof(*sl)) {
                        *sl++ = cl;
                        count -= sizeof(*sl);
                }
        }
	/* fill 8 bits at a time */
        s8 = (char *)sl;
        while (count--)
                *s8++ = c;

        return s;
}

char *strncpy(char *dest, const char *src, size_t count)
{
        char *tmp = dest;

        while (count-- && (*dest++ = *src++) != '\0')
                /* nothing */;

        return tmp;

}

size_t strlen(const char * s)
{
        const char *sc;

        for (sc = s; *sc != '\0'; ++sc)
                /* nothing */;
        return sc - s;
}

