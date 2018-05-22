#ifndef RCHOSC_H_INCLUDED
#define RCHOSC_H_INCLUDED
// ---- MODULE CODE STARTS BELOW ---- //


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
        
        // Generic skeleton and wrapper classes to base
        // templates and oscillators on.
        #include "helpers/skeleton.h"
        #include "helpers/wrapper.h"
        
    } // end namespace RCH::Helpers
    
    /** Multi-channel oscillator classes and per-sample template classes */
    namespace Oscillators
    {
        /** Basic per-sample template oscillator classes */
        namespace Templates
        {
            // Oscillator templates
            #include "oscillators/templates/sine.h"
            #include "oscillators/templates/triangle.h"
            #include "oscillators/templates/saw.h"
            #include "oscillators/templates/square.h"
            #include "oscillators/templates/pulse.h"
            
        } // end namespace RCH::Oscillators::Templates
        
        // Multi-channel enabled (mono) oscillators
        #include "oscillators/sine.h"
        #include "oscillators/triangle.h"
        #include "oscillators/saw.h"
        #include "oscillators/square.h"
        #include "oscillators/pulse.h"
        
    } // end namespace RCH::Oscillators
    
} // end namespace RCH


// Un-comment the next line if you don't want
// to use the RCH:: prefix everywhere:
//using namespace RCH;


// ---- MODULE CODE ENDS ABOVE ---- //
#endif // #ifndef RCHOSC_H_INCLUDED
