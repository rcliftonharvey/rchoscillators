#ifndef RCHOSC_HELPERS_MAX_H_INCLUDED
#define RCHOSC_HELPERS_MAX_H_INCLUDED
// ---- MODULE CODE STARTS BELOW ---- //

    
/** Returns the larger of 2 passed values. */
template <typename TYPE>
TYPE Max (const TYPE& A, const TYPE& B)
{
    return (A > B) ? A : B;
}


// ---- MODULE CODE ENDS ABOVE ---- //
#endif // #ifndef RCHOSC_HELPERS_MAX_H_INCLUDED
