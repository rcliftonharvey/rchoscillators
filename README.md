# RCH::Oscillators
#### A library of fast DSP oscillators in pure C++

-------------------------------------------------------------------------------------------------------

## Index

1. [Introduction](https://github.com/rcliftonharvey/rchoscillators/blob/master/README.md#introduction)
2. [How to use](https://github.com/rcliftonharvey/rchoscillators/blob/master/README.md#how-to-use)
3. [Example project](https://github.com/rcliftonharvey/rchoscillators/blob/master/README.md#example-project)
4. [Notes](https://github.com/rcliftonharvey/rchoscillators/blob/master/README.md#notes)
5. [License](https://github.com/rcliftonharvey/rchoscillators/blob/master/README.md#license)
6. [Credits](https://github.com/rcliftonharvey/rchoscillators/blob/master/README.md#credits)

-------------------------------------------------------------------------------------------------------

## Introduction 

This is a pure C++ library that implements all basic oscillator types (sine, triangle, saw rising, saw falling, square and pulse) in fast and easy to use classes, both as single-channel per-sample "templates", as well as multi-channel capable processors (with mono signal) than can handle entire sample buffers in one call.

This library is compatible with frameworks like [JUCE](https://juce.com/) or [WDL](https://www.cockos.com/wdl/), but it was written with no dependencies, which means you can use it by itself and in any application, without being bound to JUCE, WDL or any such 3rd party frameworks at all.

I have done my best to comment the sources as much as I could (or made sense to me), so by reading through the headers you should get a good enough idea of what things can do or what you need to do with them.

The original repository for RCH::Oscillators is right here:<br>
[https://github.com/rcliftonharvey/rchoscillators]

-------------------------------------------------------------------------------------------------------

## How to use

Include the [library](https://github.com/rcliftonharvey/rchoscillators/tree/master/library) folder in your project, and include the main header file into your own main project header(s).

If you use JUCE, add all the files from the library folder into a group in your JUCE project. The easiest way to do this is via the "add existing files" option in the files pane of the Projucer app.

Depending on your compiler and workspace settings, you will probably have to adjust the include path for this file. But once the include path is sorted out, this is the only include line you should need:
```c++
#include "rchosc.h"
```

To avoid possible collisions with other libraries you may be using in your projects, all the classes in this library reside in the **RCH::** namespace by default. (Unless you changed it in the rchosc.h file.)

**IMPORTANT** *Since I remember struggling with this myself in the early years...<br>
These oscillators are **stateful**. This means one oscillator instance stays alive all the time, and no matter how many sample blocks you need to fill, you always do it with the same oscillator instance. If you want to have a continuous oscillator wave without any distortion or phase jumps, do not instantiate these in your per-block processing methods, but instantiate them as lasting variables in your header files.* :)

To get started, go ahead and instantiate a simple mono sine oscillator class. On JUCE, you would do this in the **PluginProcessor.h** file:
```c++
RCH::Oscillators::Templates::Sine oscSine;
```

This will create an oscillator that can produce a continuous sine wave for a single channel of audio, and you will have to poll it on a per-sample basis.

Now that your oscillator is instantiated, you need to inform it about essential values, like the sample rate to operate at and the center frequency to generate the sine wave at, otherwise the oscillator can't process correctly. Set the oscillator up somewhere in your per-block processing call, in JUCE that would be in the file **PluginProcessor.cpp**, maybe at ***prepareToPlay*** or (more likely) in the ***processBlock*** method:
```c++
oscSine.setSampleRate(sampleRateInHz);
oscSine.setFrequency(frequencyInHz);
oscSine.setAmplitude(volumeInFloatGain);   // if you use float gain factors (type float) or
oscSine.setVolume(volumeInDoubleDecibels); // if you use Decibel values (type double)
```

The SAW and PULSE oscillators have special properties that are not handled by the regular setup function. It is not required to set these up to get a tone out of the oscillators, but these parameters do offer extended control over the sound, so it's worth mentioning them.
```c++
oscSaw.setDirection(-1.0); // wave falls from high to low
oscSaw.setDirection(1.0);  // wave rises from low to high

oscPulse.setWidth(0.125);  // very narrow pulses
oscPulse.setWidth(0.85);   // nearly square wave
```

Finally, run a loop that fetches one tick from the oscillator for every sample in a channel array.
```c++
for (unsigned int sample=0; sample<NumSamples; ++sample)
{
  channel[sample] =  oscSine.tick();   // This will entirely replace the incoming signal with the sine wave, alternatively...
  channel[sample] += oscSine.tick();   // This will add the sine wave on top of the incoming signal.
}
```

Your channel of samples will now be filled with a sine wave, and that's all you need to get going.

Now, I'm a lazy guy, so I don't always want to cycle through all the channels and all the samples of a buffer by hand, that just takes up time and space. So I've included a set of wrapper classes that deal with this pesky task automatically. You just pass a C array of **double**s or **float**s into the oscillator, as well as two **unsigned int**s with the number of channels and samples in that C array, and the oscillator will handle it from there.

In the current state, these wrapper classes will generate a **single mono oscillator**, and insert or add that mono signal onto **all** the buffer's channels equally. Theoretically, there is no limitation to the number of channels that could be processed, a buffer block could have a single channel, or over 200 channels, the oscillators don't care.

To instantiate an example triangle wave oscillator that offers this simple processing method, just instantiate it in your header file like this:
```c++
RCH::Oscillators::Triangle oscTriangle;
```

Then set it up somewhere in your processing methods...
```c++
oscTriangle.setup(sampleRateInHz,frequencyInHz,amplitudeInFloatGain);   // if you use float gain factors (type float) or
oscTriangle.setup(sampleRateInHz,frequencyInHz,volumeInDoubleDecibels); // if you use Decibel values (type double)
```

...and send a buffer of **float** or **double** samples into it:
```c++
oscTriangle.fill(buffer,numChannels,numSamples);  // if you want to completely overwrite the buffer
oscTriangle.add (buffer,numChannels,numSamples);  // if you want to add the triangle to the buffer
```

The **fill** or **add** methods expect an array of channels, where each channel is an array of samples. In native C++ lingo, this means the buffer needs to be of type **float**** or **double****, the most common types of sample & channel arrays.

If you're using JUCE, your **processBlock** method will present you with a variable named **buffer** in a JUCE specific datatype, the **AudioBuffer**. Despite being an abstraction, it still offers direct access to its channels and samples in the format required for the **fill** and **add** methods of these oscillators. Since the oscillator writes directly into the passed buffer, just make sure you go for WritePointers, not ReadPointers:
```c++
oscTriangle.fill(buffer.getArrayOfWritePointers(),numChannels,numSamples); // JUCE specific in processBlock
```

And that's just about all there's to it... :)

By the way: *don't worry about constantly pushing values into the setup() methods, e.g. in **processBlock**. The oscillators are designed to only ever bother with updating their settings internally when the incoming values actually change. If you send identical sample rate, center frequency and volume values into a setup() method 200 times, the oscillators won't care. If one of the values changes on the 201st time, then the oscillators will update.*

-------------------------------------------------------------------------------------------------------

## Example project

For a real world example, check out the included [RCHOscillatorsDemo](https://github.com/rcliftonharvey/rchoscillators/tree/master/demo) project. Yes, it's JUCE based, so you'd need that to build it. But it's also fine to just look into the [PluginProcessor.h](https://github.com/rcliftonharvey/rchoscillators/tree/master/demo/Source/PluginProcessor.h) and [PluginProcessor.cpp](https://github.com/rcliftonharvey/rchoscillators/tree/master/demo/Source/PluginProcessor.cpp) files, they should give you a good enough idea of how easy it is to work with this library.

-------------------------------------------------------------------------------------------------------

## License

This source code is provided under the [MIT License](https://en.wikipedia.org/wiki/MIT_License).<br>
A copy of the MIT license in written form comes in the download of this library.

The JUCE framework itself (required for the demo project) is shipped and licensed separately, see the [JUCE website](https://juce.com) for more information.

-------------------------------------------------------------------------------------------------------

## Credits

The oscillators in this library were inspired by Alan Wolfe's example code at Demofox.org, but were significantly extended, improved and simplified by me. Still, check Alan's blog out, it's full of incredibly useful DSP resources, both for graphics and audio processing.<br>
[http://demofox.org/]

-------------------------------------------------------------------------------------------------------

Enjoy!

Rob Clifton-Harvey
