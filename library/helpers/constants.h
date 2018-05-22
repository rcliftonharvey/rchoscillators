#ifndef RCHOSC_HELPERS_CONSTANTS_H_INCLUDED
#define RCHOSC_HELPERS_CONSTANTS_H_INCLUDED
// ---- MODULE CODE STARTS BELOW ---- //


    #ifdef M_LN10_20
    #undef M_LN10_20
    #endif                  /* 20 over LN10 (for volume conversion float to dB) */
    #define M_LN10_20       8.6858896380650365530225783783321

    #ifdef M_20_LN10
    #undef M_20_LN10
    #endif                  /* LN10 over 20 (for volume conversion dB to float) */
    #define M_20_LN10       0.11512925464970228420089957273422

    #ifdef M_PI
    #undef M_PI
    #endif                  /* PI ... om nom nom */
    #define M_PI            3.14159265358979323846264338327950288419716939937510582

    #ifdef M_2PI
    #undef M_2PI
    #endif                  /* 2 * PI ... moar nom */
    #define M_2PI           6.283185307179586476925286766559005768394338798750211642


// ---- MODULE CODE ENDS ABOVE ---- //
#endif // #ifndef RCHOSC_HELPERS_CONSTANTS_H_INCLUDED
