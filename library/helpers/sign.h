#pragma once
#ifndef RCH_HELPERS_SIGN_H_INCLUDED
#define RCH_HELPERS_SIGN_H_INCLUDED
// ---- MODULE CODE STARTS BELOW ---- //


/** Returns 1.0 if the input Value is >= 0.
    Returns -1.0 if the input Value is < 0. */
template <typename TYPE>
const TYPE Sign (const TYPE& Value)
{
//    const TYPE zero     = static_cast<TYPE>(0.0);
//    const TYPE positive = static_cast<TYPE>(1.0);
//    const TYPE negative = static_cast<TYPE>(-1.0);
//
//    return (Value >= zero) ? positive : negative;
//
//  Below method should be more performant since there's no conditional branching
    
    return static_cast<TYPE>(1) - (Value < static_cast<TYPE>(0)) * static_cast<TYPE>(2);
}


// ---- MODULE CODE ENDS ABOVE ---- //
#endif // #ifndef RCH_HELPERS_SIGN_H_INCLUDED
