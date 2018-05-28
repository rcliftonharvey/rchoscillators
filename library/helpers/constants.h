#pragma once
#ifndef RCH_HELPERS_CONSTANTS_H_INCLUDED
#define RCH_HELPERS_CONSTANTS_H_INCLUDED
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

    #ifdef M_1_PI
    #undef M_1_PI
    #endif                  /* 1/PI           */
    #define M_1_PI          0.318309886183790671537767526745028724068919291480912898

    #ifdef M_2_PI
    #undef M_2_PI
    #endif                  /* 2/PI           */
    #define M_2_PI          0.636619772367581343075535053490057448137838582961825795

    #ifdef M_4_PI
    #undef M_4_PI
    #endif                  /* 4/PI           */
    #define M_4_PI          1.27323954473516268615107010698011489627567716592365

    #ifdef M_8_PIPI
    #undef M_8_PIPI
    #endif                  /* 8 / (PI * PI)        */
    #define M_8_PIPI        0.81056946913870217155103570567782111123487019737797


// ---- MODULE CODE ENDS ABOVE ---- //
#endif // #ifndef RCH_HELPERS_CONSTANTS_H_INCLUDED
