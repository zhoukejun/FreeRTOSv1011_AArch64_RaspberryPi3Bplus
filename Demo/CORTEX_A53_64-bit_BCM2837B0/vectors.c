#include "types.h"
#include "exception.h"
#include "vectors.h"
#include "irq.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

typedef struct {
	ExceptionHandler Handler;
	void *Data;
} Exc_VectorTableEntry;

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Variable Definitions *****************************/

extern Exc_VectorTableEntry Exc_VectorTable[];

/************************** Function Prototypes ******************************/


/*****************************************************************************/
/**
*
* This is the C level wrapper for the FIQ interrupt called from the vectors.s
* file.
*
* @param	None.
*
* @return	None.
*
* @note		None.
*
******************************************************************************/
void FIQInterrupt(void)
{
	print("FIQInterrupt()");
//	Exc_VectorTable[EXCEPTION_ID_FIQ_INT].Handler(Exc_VectorTable[
//					EXCEPTION_ID_FIQ_INT].Data);
}

/*****************************************************************************/
/**
*
* This is the C level wrapper for the IRQ interrupt called from the vectors.s
* file.
*
* @param	None.
*
* @return	None.
*
* @note		None.
*
******************************************************************************/
void IRQInterrupt(void)
{
	print("IRQInterrupt()");
	bcm_irq_Handler();
}


/*****************************************************************************/
/**
*
* This is the C level wrapper for the Synchronous Interrupt called from the vectors.s
* file.
*
* @param	None.
*
* @return	None.
*
* @note		None.
*
******************************************************************************/
void SynchronousInterrupt(void)
{
	print("SynchronousInterrupt()");
	Exc_VectorTable[EXCEPTION_ID_SYNC_INT].Handler(Exc_VectorTable[
					EXCEPTION_ID_SYNC_INT].Data);
}

/*****************************************************************************/
/**
*
* This is the C level wrapper for the SError Interrupt called from the
* vectors.s file.
*
* @param	None.
*
* @return	None.
*
* @note		None.
*
******************************************************************************/
void SErrorInterrupt(void)
{
	print("SErrorInterrupt()");
	Exc_VectorTable[EXCEPTION_ID_SERROR_ABORT_INT].Handler(
		Exc_VectorTable[EXCEPTION_ID_SERROR_ABORT_INT].Data);
}


void FreeRTOS_Abort_Handler(uint32_t X0, uint32_t X1)
{
	print("FreeRTOS_Abort_Handler()");
	print("ELR_EL3 is:");
	hexstring(X0);
	print("Exception class code is:");
	hexstring(X1);
}
