#ifndef EXCEPTION_H /* prevent circular inclusions */
#define EXCEPTION_H /* by using protection macros */

/***************************** Include Files ********************************/


/************************** Constant Definitions ****************************/

#define EXCEPTION_FIQ	REG_CPSR_FIQ_ENABLE
#define EXCEPTION_IRQ	REG_CPSR_IRQ_ENABLE
#define EXCEPTION_ALL	(REG_CPSR_FIQ_ENABLE | REG_CPSR_IRQ_ENABLE)

#define EXCEPTION_ID_FIRST		0U
#define EXCEPTION_ID_SYNC_INT		1U
#define EXCEPTION_ID_IRQ_INT		2U
#define EXCEPTION_ID_FIQ_INT		3U
#define EXCEPTION_ID_SERROR_ABORT_INT	4U
#define EXCEPTION_ID_LAST		5U

/*
 * EXCEPTION_ID_INT is defined for all Xilinx processors.
 */
#define EXCEPTION_ID_INT	EXCEPTION_ID_IRQ_INT

/**************************** Type Definitions ******************************/

/**
 * This typedef is the exception handler function.
 */
typedef void (*ExceptionHandler)(void *data);
typedef void (*InterruptHandler)(void *data);

/***************** Macros (Inline Functions) Definitions ********************/

/****************************************************************************/
/**
* @brief	Enable Exceptions.
*
* @param	Mask: Value for enabling the exceptions.
*
* @return	None.
*
* @note		If bit is 0, exception is enabled.
*			C-Style signature: void ExceptionEnableMask(Mask)
*
******************************************************************************/
#if defined (__GNUC__) || defined (__ICCARM__)
#define ExceptionEnableMask(Mask)	\
		mtcpsr(mfcpsr() & ~ ((Mask) & EXCEPTION_ALL))
#else
#define ExceptionEnableMask(Mask)	\
		{								\
		  register u32 Reg __asm("cpsr"); \
		  mtcpsr((Reg) & (~((Mask) & EXCEPTION_ALL))); \
		}
#endif
/****************************************************************************/
/**
* @brief	Enable the IRQ exception.
*
* @return   None.
*
* @note     None.
*
******************************************************************************/
#define ExceptionEnable() \
		ExceptionEnableMask(EXCEPTION_IRQ)

/****************************************************************************/
/**
* @brief	Disable Exceptions.
*
* @param	Mask: Value for disabling the exceptions.
*
* @return	None.
*
* @note		If bit is 1, exception is disabled.
*			C-Style signature: ExceptionDisableMask(Mask)
*
******************************************************************************/
#if defined (__GNUC__) || defined (__ICCARM__)
#define ExceptionDisableMask(Mask)	\
		mtcpsr(mfcpsr() | ((Mask) & EXCEPTION_ALL))
#else
#define ExceptionDisableMask(Mask)	\
		{									\
		  register uintptr_t Reg __asm("cpsr"); \
		  mtcpsr((Reg) | ((Mask) & EXCEPTION_ALL)); \
		}
#endif
/****************************************************************************/
/**
* Disable the IRQ exception.
*
* @return   None.
*
* @note     None.
*
******************************************************************************/
#define ExceptionDisable() \
		ExceptionDisableMask(EXCEPTION_IRQ)

/************************** Variable Definitions ****************************/

/************************** Function Prototypes *****************************/

extern void ExceptionRegisterHandler(uint32_t Exception_id,
					 ExceptionHandler Handler,
					 void *Data);

extern void ExceptionRemoveHandler(uint32_t Exception_id);

extern void ExceptionInit(void);
void SyncAbortHandler(void *CallBackRef);
void SErrorAbortHandler(void *CallBackRef);

#endif /* EXCEPTION_H */
/**
* @} End of "addtogroup arm_exception_apis".
*/
