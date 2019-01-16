/**
 *	IRQ
 *
 *	
 **/
#include "regs.h"
#include "irq.h"
#include "types.h"

static BCM_BASIC_INTERRUPT_VECTOR Bcm_Basic_Interrupt_VectorTable[BCM_BASIC_INTERRUPT_TOTAL];

static BCM_PERIPHERALS_INTERRUPT_VECTOR Bcm_Peripherals_Interrupt_VectorTable[BCM_PERIPHERALS_INTERRUPT_TOTAL];

/* irq functions for Basic interrupts*/
int bcm_basic_irq_disable(BCM_BASIC_INTERRUPT_T Int_Id)
{
	if (Int_Id < BCM_BASIC_INTERRUPT_TOTAL)	
	{
		iowrite32((0x1 << Int_Id), DISABLE_BASIC_IRQ);
		return 0;
	}
	else
		return -1;
}
int bcm_basic_irq_enable(BCM_BASIC_INTERRUPT_T Int_Id)
{
	if (Int_Id < BCM_BASIC_INTERRUPT_TOTAL)	
	{
		iowrite32((0x1 << Int_Id) | ioread32(ENABLE_BASIC_IRQ), ENABLE_BASIC_IRQ);

		//iowrite32((0x1 << Int_Id), ENABLE_BASIC_IRQ);

		return 0;
	}
	else
	{
		return -1;
	}
}

int bcm_basic_irq_unregister(BCM_BASIC_INTERRUPT_T Int_Id)
{
	
	if (Int_Id < BCM_BASIC_INTERRUPT_TOTAL)
	{
		Bcm_Basic_Interrupt_VectorTable[Int_Id].pHandler = NULL;
		Bcm_Basic_Interrupt_VectorTable[Int_Id].pParam = 0;

		return 0;
	}
	else
		return -1;
}

int bcm_basic_irq_register(BCM_BASIC_INTERRUPT_T Int_Id, BCM_BASIC_INTERRUPT_HANDLER pHandler, void *pParam)
{
	if (Int_Id < BCM_BASIC_INTERRUPT_TOTAL)
	{
		Bcm_Basic_Interrupt_VectorTable[Int_Id].pHandler = pHandler;
		Bcm_Basic_Interrupt_VectorTable[Int_Id].pParam = pParam;

		return 0;
	}
	else
		return -1;

}

/* irq functions for Peripherals interrupts*/
int bcm_peripherals_irq_disable(BCM_PERIPHERALS_INTERRUPT_T Int_Id)
{
	if (Int_Id < 32)
	{
		iowrite32((0x1 << Int_Id), DISABLE_IRQ_1);
	}
	else if (Int_Id < BCM_PERIPHERALS_INTERRUPT_TOTAL)	
	{
		iowrite32((0x1 << (Int_Id - 32)), DISABLE_IRQ_2);
		return 0;
	}
	else
		return -1;
}
int bcm_peripherals_irq_enable(BCM_PERIPHERALS_INTERRUPT_T Int_Id)
{
	if (Int_Id < 32)
	{
		//iowrite32((0x1 << Int_Id) | ioread32(ENABLE_IRQ_1), ENABLE_IRQ_1);
		iowrite32((0x1 << Int_Id), ENABLE_IRQ_1);

		return 0;
	}
	else if (Int_Id < BCM_PERIPHERALS_INTERRUPT_TOTAL)	
	{
		//iowrite32((0x1 << Int_Id) | ioread32(ENABLE_IRQ_2), ENABLE_IRQ_2);
		iowrite32((0x1 << (Int_Id - 32)), ENABLE_IRQ_2);

		return 0;
	}
	else
		return -1;
}

int bcm_peripherals_irq_unregister(BCM_PERIPHERALS_INTERRUPT_T Int_Id)
{
	
	if (Int_Id < BCM_PERIPHERALS_INTERRUPT_TOTAL)
	{
		Bcm_Peripherals_Interrupt_VectorTable[Int_Id].pHandler = NULL;
		Bcm_Peripherals_Interrupt_VectorTable[Int_Id].pParam = 0;

		bcm_peripherals_irq_disable(Int_Id);
		return 0;
	}
	else
		return -1;
}

int bcm_peripherals_irq_register(BCM_PERIPHERALS_INTERRUPT_T Int_Id, BCM_PERIPHERALS_INTERRUPT_HANDLER pHandler, void *pParam)
{
	if (Int_Id < BCM_PERIPHERALS_INTERRUPT_TOTAL)
	{
		Bcm_Peripherals_Interrupt_VectorTable[Int_Id].pHandler = pHandler;
		Bcm_Peripherals_Interrupt_VectorTable[Int_Id].pParam = pParam;

		bcm_peripherals_irq_enable(Int_Id);
		return 0;
	}
	else
		return -1;

}

/* Interrupt Handler */
void bcm_irq_Handler(void)
{
	//Check the Basic Pending register is enough. No provision to check the Basic Interrupt enable register.
	uint32_t ulBasicPendingStatus = ioread32(IRQ_BASIC_PENDING);	
  
	// Bits 7 ~ 0 in IRQBASIC_PENDING represent interrupts :
	if (ulBasicPendingStatus & 0xFF)
	{
		uint32_t Int_Id = 31 - __builtin_clz (ulBasicPendingStatus);
		BCM_BASIC_INTERRUPT_HANDLER pHandler;
		pHandler = (Bcm_Basic_Interrupt_VectorTable[Int_Id].pHandler);

		void *pParam = Bcm_Basic_Interrupt_VectorTable[Int_Id].pParam;
		if (pHandler)
		{
			pHandler();
		}
	}
	// Bit 8 in IRQ_BASSIC_PENDING indicates interrupts in Pending1 (interrupts 31-0):
	else if (ulBasicPendingStatus & (1UL << 8))
	{
		// Check the Peripherals Pending1 register is enough. No provision to check the Peripheral Interrupt Enable register 1.
		uint32_t ulPerPending_1 = ioread32(IRQ_PENDING_1);
		uint32_t Int_Id = 31 - __builtin_clz (ulPerPending_1);
		BCM_PERIPHERALS_INTERRUPT_HANDLER pHandler = Bcm_Peripherals_Interrupt_VectorTable[Int_Id].pHandler;
		void *pParam = Bcm_Peripherals_Interrupt_VectorTable[Int_Id].pParam;
		// No provision to check the Peripheral Interrupt enable register.
		if (pHandler)
		{
			pHandler(Int_Id, pParam);
		}
	}
	// Bit 9 in IRQ_BASIC_PENDING indicates interrupts in Pending2 (interrupts 63-32):
	else if (ulBasicPendingStatus & (1UL << 9))
	{
		// Check the Peripherals Pending2 register is enough. No provision to check the Peripheral Interrupt Enable register 2.
		uint32_t ulPerPending_2 = ioread32(IRQ_PENDING_2);
		uint32_t Int_Id = 32 + 31 - __builtin_clz (ulPerPending_2);
		BCM_PERIPHERALS_INTERRUPT_HANDLER pHandler = Bcm_Peripherals_Interrupt_VectorTable[Int_Id].pHandler;
		void *pParam = Bcm_Peripherals_Interrupt_VectorTable[Int_Id].pParam;
		if (pHandler)
		{
			pHandler(Int_Id, pParam);
		}
	}
	// Bits 20 ~ 10 in IRQ_BASIC_PENDING indicate GPU IRQs:
	else if (ulBasicPendingStatus & 0x1FFC00)
	{
		/* GPU IRQs */
		return;
	}
	else
	{
		return;
	}
	return;
}



void bcm_irq_check_pending(void)
{
	//Check the Basic Pending register is enough. No provision to check the Basic Interrupt enable register.
	uint32_t ulBasicPendingStatus = ioread32(IRQ_BASIC_PENDING);	
  
	// Bits 7 ~ 0 in IRQBASIC_PENDING represent interrupts :
	if (ulBasicPendingStatus & 0xFF)
	{
		print("IRQBASIC_PENDING is");
		hexstring(ulBasicPendingStatus);
	}
	// Bit 8 in IRQ_BASSIC_PENDING indicates interrupts in Pending1 (interrupts 31-0):
	else if (ulBasicPendingStatus & (1UL << 8))
	{
		uint32_t ulPerPending_1 = ioread32(IRQ_PENDING_1);
		print("IRQ_PENDING_1 is");
		hexstring(ulPerPending_1);
	}
	// Bit 9 in IRQ_BASIC_PENDING indicates interrupts in Pending2 (interrupts 63-32):
	else if (ulBasicPendingStatus & (1UL << 9))
	{
		uint32_t ulPerPending_2 = ioread32(IRQ_PENDING_2);
		print("IRQ_PENDING_2 is");
		hexstring(ulPerPending_2);
	}
	// Bits 20 ~ 10 in IRQ_BASIC_PENDING indicate GPU IRQs:
	else if (ulBasicPendingStatus & 0x1FFC00)
	{
		/* GPU IRQs */

		print("IRQBASIC_PENDING is");
		hexstring(ulBasicPendingStatus);
		return;
	}
	else
	{
		print("bcm_irq_check_pending() nothing in pending");
		return;
	}
	return;
}
