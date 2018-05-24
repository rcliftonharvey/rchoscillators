#ifndef RCHOSC_H_INCLUDED
#define RCHOSC_H_INCLUDED
// ---- MODULE CODE STARTS BELOW ---- //


// Some things in here need C++ vectors
#include <vector>


/** RCH::Oscillators are in here */
namespace RCH
{
    /** Assorted supporting functionality */
    namespace Helpers
    {
        #include "helpers/andor.h"
        #include "helpers/constants.h"
        #include "helpers/max.h"
        #include "helpers/sign.h"
        #include "helpers/decibels.h"
        
        #include "helpers/skeleton.h"
        #include "helpers/skeleton_bl.h"
        #include "helpers/wrapper.h"
        
    } // end namespace RCH::Helpers
    
    /** Multi-channel oscillator classes and per-sample template classes */
    namespace Oscillators
    {
        /** Basic mono per-sample template oscillator classes */
        namespace Templates
        {
            #include "oscillators/templates/sine.h"
            #include "oscillators/templates/triangle.h"
            #include "oscillators/templates/saw.h"
            #include "oscillators/templates/square.h"
            #include "oscillators/templates/squarepulse.h"
            #include "oscillators/templates/pulse.h"
            
            /** Band-limited mono per-sample template oscillator classes */
            namespace Bandlimited
            {
                #include "oscillators/templates/bandlimited/saw.h"
                #include "oscillators/templates/bandlimited/square.h"
                #include "oscillators/templates/bandlimited/triangle.h"
                
            } // end namespace RCH::Oscillators::Templates::Bandlimited
            
        } // end namespace RCH::Oscillators::Templates
        
        // Multi-channel enabled (mono) oscillators
        #include "oscillators/sine.h"
        #include "oscillators/triangle.h"
        #include "oscillators/saw.h"
        #include "oscillators/square.h"
        #include "oscillators/squarepulse.h"
        #include "oscillators/pulse.h"
        
        /** Multi-channel enabled band-limited oscillator classes */
        namespace Bandlimited
        {
            #include "oscillators/bandlimited/saw.h"
            #include "oscillators/bandlimited/square.h"
            #include "oscillators/bandlimited/triangle.h"
            
        } // end namespace RCH::Oscillators::Bandlimited
        
    } // end namespace RCH::Oscillators
    
} // end namespace RCH


// If you don't want to use the RCH:: prefix everywhere, un-comment the next line
//using namespace RCH;


// ---- MODULE CODE ENDS ABOVE ---- //
#endif // #ifndef RCHOSC_H_INCLUDED
