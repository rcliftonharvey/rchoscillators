#pragma once
#ifndef RCH_HELPERS_DECIBELS_H_INCLUDED
#define RCH_HELPERS_DECIBELS_H_INCLUDED
// ---- MODULE CODE STARTS BELOW ---- //

    
namespace Decibels
{
    /** Turns Decibels into float gain factor */
    inline const float ToGain (const double& dB)
    {
        return static_cast<float>(std::exp(dB * M_20_LN10));
    }
    
    /** Turns float gain factor into Decibels */
    inline const double FromGain (const float& Gain)
    {
        if (Gain < 0.0000000298023223876953125f)
        {
            return -150.0;
        }
        
        const double dB = std::log(Gain) * M_LN10_20;
        
        return static_cast<double>(Helpers::Max(dB,-150.0));
    }
    
} // end namespace RCH::Helpers::Decibels


// ---- MODULE CODE ENDS ABOVE ---- //
#endif // #ifndef RCH_HELPERS_DECIBELS_H_INCLUDED
