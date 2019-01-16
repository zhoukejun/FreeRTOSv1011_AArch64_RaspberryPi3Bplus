#include "types.h"
#include "exception.h"
#include "debug.h"
/************************** Constant Definitions ****************************/

/**************************** Type Definitions ******************************/

typedef struct {
	ExceptionHandler Handler;
	void *Data;
} Exc_VectorTableEntry;

/***************** Macros (Inline Functions) Definitions ********************/

/************************** Function Prototypes *****************************/
static void ExceptionNullHandler(void *Data);
/************************** Variable Definitions *****************************/
/*
 * Exception vector table to store handlers for each exception vector.
 */
Exc_VectorTableEntry Exc_VectorTable[EXCEPTION_ID_LAST + 1] =
{
        {ExceptionNullHandler, NULL},
        {SyncAbortHandler, NULL},
        {ExceptionNullHandler, NULL},
        {ExceptionNullHandler, NULL},
        {SErrorAbortHandler, NULL},

};

/*****************************************************************************/

/****************************************************************************/
/**
*
* This function is a stub Handler that is the default Handler that gets called
* if the application has not setup a Handler for a specific  exception. The
* function interface has to match the interface specified for a Handler even
* though none of the arguments are used.
*
* @param	Data is unused by this function.
*
* @return	None.
*
* @note		None.
*
*****************************************************************************/
static void ExceptionNullHandler(void *Data)
{
	(void) Data;
DieLoop: goto DieLoop;
}

/****************************************************************************/
/**
* @brief	The function is a common API used to initialize exception handlers
*			across all supported arm processors. For ARM Cortex-A53, Cortex-R5,
*			and Cortex-A9, the exception handlers are being initialized
*			statically and this function does not do anything.
* 			However, it is still present to take care of backward compatibility
*			issues (in earlier versions of BSPs, this API was being used to
*			initialize exception handlers).
*
* @param	None.
*
* @return	None.
*
* @note		None.
*
*****************************************************************************/
void ExceptionInit(void)
{
	return;
}

/*****************************************************************************/
/**
* @brief	Register a handler for a specific exception. This handler is being
*			called when the processor encounters the specified exception.
*
* @param	exception_id contains the ID of the exception source and should
*			be in the range of 0 to XIL_EXCEPTION_ID_LAST.
*			See xil_exception.h for further information.
* @param	Handler to the Handler for that exception.
* @param	Data is a reference to Data that will be passed to the
*			Handler when it gets called.
*
* @return	None.
*
* @note		None.
*
****************************************************************************/
void ExceptionRegisterHandler(uint32_t Exception_id,
				    ExceptionHandler Handler,
				    void *Data)
{
	Exc_VectorTable[Exception_id].Handler = Handler;
	Exc_VectorTable[Exception_id].Data = Data;
}

/*****************************************************************************/
/**
*
* @brief	Removes the Handler for a specific exception Id. The stub Handler
*			is then registered for this exception Id.
*
* @param	exception_id contains the ID of the exception source and should
*			be in the range of 0 to XIL_EXCEPTION_ID_LAST.
*			See xil_exception.h for further information.
*
* @return	None.
*
* @note		None.
*
****************************************************************************/
void ExceptionRemoveHandler(uint32_t Exception_id)
{
	ExceptionRegisterHandler(Exception_id,
				       ExceptionNullHandler,
				       NULL);
}

/*****************************************************************************/
/**
*
* Default Synchronous abort handler which prints a debug message on console if
* Debug flag is enabled
*
* @param        None
*
* @return       None.
*
* @note         None.
*
****************************************************************************/

void SyncAbortHandler(void *CallBackRef){
	(void) CallBackRef;
	dbg_printf(DBG_DEBUG_ERROR, "Synchronous abort \n");
	while(1) {
		;
	}
}

/*****************************************************************************/
/**
*
* Default SError abort handler which prints a debug message on console if
* Debug flag is enabled
*
* @param        None
*
* @return       None.
*
* @note         None.
*
****************************************************************************/
void SErrorAbortHandler(void *CallBackRef){
	(void) CallBackRef;
	dbg_printf(DBG_DEBUG_ERROR, "SError abort \n");
	while(1) {
		;
	}
}
