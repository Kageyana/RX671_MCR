#ifndef C_CPP_INTELLISENSE_HELPER_H
#define C_CPP_INTELLISENSE_HELPER_H

#ifdef __INTELLISENSE__
#define __CDT_PARSER__ /* This might be a bad practice... */

/*
Tips to make IntelliSense work with code written for Renesas CC-RX compiler
*/

/*
4.2.1 Macro Names
http://tool-support.renesas.com/autoupdate/support/onlinehelp/csp/V8.06.00/CS+.chm/Compiler-CCRX.chm/Output/ccrx04c0201y.html
http://tool-support.renesas.com/autoupdate/support/onlinehelp/ja-JP/csp/V8.06.00/CS+.chm/Compiler-CCRX.chm/Output/ccrx04c0201y.html
*/
#define __GNUC__
#define __RX 1
#define __RX_LITTLE_ENDIAN__

#define __RX_FPU_INSNS__
// #define __RXv1__
// #define __RXv2__
#define __RXv3__
#define __RX_DFPU_INSNS__

/*
4.2.2 Keywords
http://tool-support.renesas.com/autoupdate/support/onlinehelp/csp/V8.06.00/CS+.chm/Compiler-CCRX.chm/Output/ccrx04c0202y.html
http://tool-support.renesas.com/autoupdate/support/onlinehelp/ja-JP/csp/V8.06.00/CS+.chm/Compiler-CCRX.chm/Output/ccrx04c0202y.html
*/
#pragma diag_suppress 661
#define __evenaccess

/*
4.2.6 Intrinsic Functions
http://tool-support.renesas.com/autoupdate/support/onlinehelp/csp/V8.06.00/CS+.chm/Compiler-CCRX.chm/Output/ccrx04c0206y.html
http://tool-support.renesas.com/autoupdate/support/onlinehelp/ja-JP/V8.06.00/CS+.chm/Compiler-CCRX.chm/Output/ccrx04c0206y.html
*/
#include "builtin.h"

/*
4.2.7 Section Address Operators
http://tool-support.renesas.com/autoupdate/support/onlinehelp/csp/V8.06.00/CS+.chm/Compiler-CCRX.chm/Output/ccrx04c0207y.html
http://tool-support.renesas.com/autoupdate/support/onlinehelp/ja-JP/csp/V8.06.00/CS+.chm/Compiler-CCRX.chm/Output/ccrx04c0207y.html
*/
#define __sectop( secname )  ( ( void * ) 0U )
#define __secend( secname )  ( ( void * ) 0U )
#define __secsize( secname ) ( 0UL )

#endif /* __INTELLISENSE__ */

#endif /* C_CPP_INTELLISENSE_HELPER_H */
