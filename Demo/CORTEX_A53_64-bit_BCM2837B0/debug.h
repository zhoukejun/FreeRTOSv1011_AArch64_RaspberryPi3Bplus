#ifndef DEBUG_H  /* prevent circular inclusions */
#define DEBUG_H  /* by using protection macros */


#define printf print
#if defined(DEBUG) && !defined(NDEBUG)

#ifndef DEBUG_WARNING
#define DEBUG_WARNING
#warning DEBUG is enabled
#endif

#define DBG_DEBUG_ERROR             0x00000001U    /* error  condition messages */
#define DBG_DEBUG_GENERAL           0x00000002U    /* general debug  messages */
#define DBG_DEBUG_ALL               0xFFFFFFFFU    /* all debugging data */

#define dbg_current_types (DBG_DEBUG_GENERAL)


#define dbg_printf(type, ...) (((type) & dbg_current_types) ? printf (__VA_ARGS__) : 0)


#else /* defined(DEBUG) && !defined(NDEBUG) */

#define dbg_printf(type, ...) printf (__VA_ARGS__) 
//#define dbg_printf(...)

#endif /* defined(DEBUG) && !defined(NDEBUG) */

#endif /* DEBUG_H */

