#pragma once
#ifndef RCH_HELPERS_MAX_H_INCLUDED
#define RCH_HELPERS_MAX_H_INCLUDED
// ---- MODULE CODE STARTS BELOW ---- //

    
/** Returns the larger of 2 passed values. */
template <typename TYPE>
TYPE Max (const TYPE& A, const TYPE& B)
{
    return (A > B) ? A : B;
}


// ---- MODULE CODE ENDS ABOVE ---- //
#endif // #ifndef RCH_HELPERS_MAX_H_INCLUDED
