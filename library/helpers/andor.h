#pragma once
#ifndef RCH_HELPERS_ANDOR_H_INCLUDED
#define RCH_HELPERS_ANDOR_H_INCLUDED
// ---- MODULE CODE STARTS BELOW ---- //


// Hack to make Visual Studio accept 'and' and 'or' operators in code.
#if _MSC_VER > 1000
    #ifndef and
        #define and &&
    #endif
    #ifndef or
        #define or ||
    #endif
#endif


// ---- MODULE CODE ENDS ABOVE ---- //
#endif // #ifndef RCH_HELPERS_ANDOR_H_INCLUDED
