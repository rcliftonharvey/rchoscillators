# RCH::Oscillators
#### A library of DSP oscillators in pure C++

-------------------------------------------------------------------------------------------------------

## Index

1. [Introduction](https://github.com/rcliftonharvey/rchoscillators/blob/master/README.md#introduction)
2. [Included oscillators](https://github.com/rcliftonharvey/rchoscillators/blob/master/README.md#included-oscillators)
3. [A note on band-limiting](https://github.com/rcliftonharvey/rchoscillators/blob/master/README.md#a-note-on-band-limiting)
4. [How to use](https://github.com/rcliftonharvey/rchoscillators/blob/master/README.md#how-to-use)
5. [Simplified use](https://github.com/rcliftonharvey/rchoscillators/blob/master/README.md#simplified-use)
6. [Example project](https://github.com/rcliftonharvey/rchoscillators/blob/master/README.md#example-project)
7. [Notes](https://github.com/rcliftonharvey/rchoscillators/blob/master/README.md#notes)
8. [License](https://github.com/rcliftonharvey/rchoscillators/blob/master/README.md#license)
9. [Credits](https://github.com/rcliftonharvey/rchoscillators/blob/master/README.md#credits)

-------------------------------------------------------------------------------------------------------

## Introduction 

This is a pure C++ library that implements all sorts of *naive* oscillator types, as well as their *band-limited* flavours, in fast and easy to use classes.

This library is compatible with frameworks like [JUCE](https://juce.com/) or [WDL](https://www.cockos.com/wdl/), but it was written with no dependencies, which means you can use it by itself and in any application, without being bound to any such 3rd party frameworks at all.

I have done my best to comment the sources as much as I could (or made sense to me), so by reading through the headers you should get a good enough idea of what things can do or what you need to do with them.

The original repository for RCH::Oscillators is right here:<br>
[https://github.com/rcliftonharvey/rchoscillators]

-------------------------------------------------------------------------------------------------------

## Included oscillators

Included *naive* oscillators:
+ sine
+ triangle
+ saw (rising and falling)
+ square
+ square-pulse (square with variable width)
+ pulse

Included *band-limited* oscillators:
+ triangle
+ saw (rising and falling)
+ square
+ square-pulse (square with variable width)
+ pulse

I will probably be adding more oscillators and generation methods to this library over time...

The included oscillators come both as single-channel per-sample "templates", as well as multi-channel capable processors that can fill entire sample buffers with a sound wave in just one short line of code.

-------------------------------------------------------------------------------------------------------

## A note on band-limiting

It's probably worth mentioning that since the *band-limited* oscillators are created by progressively generating and adding multiple sine waves, they **can** come at a price.

While they are guaranteed to be aliasing-free, their accuracy depends on how many harmonic sine waves are calculated. For some wave shapes you can get away with generating just a few harmonic overtones to get a fairly smooth oscillation, and for others it's necessary to calculate quite a lot of harmonics until they start looking close to what they should look like.

> A *harmonic overtone* is essentially a multiple of the base frequency. The lower the frequency of the wave, the more harmonics have to be generated to fill the spectrum, and the higher the frequency of the wave, the less harmonics have to be generated. This means that a simple saw wave could hardly touch the CPU at really high frequencies, but possibly bring your machine to a near halt at 1 Hz.

**Example**<br>
A saw wave should oscillate at 1.000 Hz. With a sample rate of 44.100 Hz, the highest frequency that can be calculated is 22.050 Hz. Since saw waves contain even and odd harmonics, to get an accurate band-limited saw wave, an additional sine wave at every multiple of the base frequency needs to be generated and added. One at 2.000 Hz (2x 1.000 Hz), one at 3.000 Hz (3x 1.000 Hz), one at 4.000 Hz (4x 1.000 Hz), and so on until it reaches the 1/2 sample rate nyquist limit at, in this case, 22.050 Hz.

> Overall, including the sine wave at the base frequency, it's necessary to calculate and add **22** sine waves to get a clean band-limited saw wave at 1.000 Hz.

Now, if the saw wave were to oscillate at 10 Hz, at the same sample rate of 44.100 Hz and the same nyquist cutoff at 22.050 Hz, things would get tricky. The same rule applies: to generate a clean saw wave, every multiple of the base frequency up to the 1/2 sample rate nyquist limit has to be calculated and added in. This means we need an additional sine wave at 20 Hz (2x 10 Hz), one at 30 Hz (3x 10 Hz), one at 40 Hz (4x 10 Hz) and so on, again until we hit the 22.050 Hz limit.

> Overall, including the sine wave at the base frequency, it's necessary to calculate and add **2.205** sine waves to get a clean band-limited saw wave at 10 Hz.

2.205 in contrast to 22 before... quite a jump.

Since triangle and square waves consist of only odd-order harmonics, it's sufficient to calculate only every second harmonic on top of the base frequency. So these wave shapes will probably be a bit lighter on CPU, as they only have to calculate and add half the amount of harmonic sine waves compared to, say, a sawtooth.

I built an accuracy setting into the band-limited oscillators, check the [skeleton_bl.h](https://github.com/rcliftonharvey/rchoscillators/tree/master/library/helpers/skeleton_bl.h) header file for the exact limit definitions. An accuracy of 7 will guarantee that all harmonics up to 22.050 Hz will be calculated for even a 1 Hz wave. You may want to go higher than that, since a project sample rate of 192 kHz could handle frequencies up to 96.000 Hz and stopping at 22.050 Hz could look (and sound) truncated... eventually, it all depends on how accurate you want it to be.

But for most applications, you should probably be able to get away with an accuracy setting of 5, making 88.2 Hz the lowest frequency to still get harmonics up to 22.050 Hz nyquist. As a reference: the lowest (useful) MIDI note is A0, equivalent to a frequency of 27.5 Hz. Using the 22.050 Hz nyquist limit for 44.100 Hz sample rate, that would mean generating 801 sine waves, which is the case if you set accuracy to 6.

Because of this difficulty, I would recommend using these additively band-limited oscillators for synthesis only, and falling back to naive oscillators for modulation applications like LFOs.

-------------------------------------------------------------------------------------------------------

## How to use

Include the [library](https://github.com/rcliftonharvey/rchoscillators/tree/master/library) folder in your project, and include the main header file into your own main project header(s).

If you use JUCE, add all the files from the library folder into a group in your JUCE project. The easiest way to do this is via the "add existing files" option in the files pane of the Projucer app.

Depending on your compiler and workspace settings, you will probably have to adjust the include path for this file. But once the include path is sorted out, this is the only line you should need:
```c++
#include "rch_oscillators.h"
```

To avoid possible collisions with other libraries you may be using in your projects, all the classes in this library reside in the **RCH::** namespace by default. You're of course welcome to change the namespace in the [rch_oscillators.h](https://github.com/rcliftonharvey/rchoscillators/tree/master/library/rch_oscillators.h) main include.

> **IMPORTANT** *Since I remember struggling with this myself in the early years...<br>
> These oscillators are **stateful**. This means one oscillator instance stays alive all the time, and no matter how many sample blocks you need to fill, you always do it with the same oscillator instance. If you want to have a continuous oscillator wave without any distortion or phase jumps, do not instantiate these in your per-block processing methods, but instantiate them as lasting variables in your header files.* :)

To get started, go ahead and instantiate a simple mono sine oscillator class. In JUCE, you would do this in the **PluginProcessor.h** file, for WDL it would be in your main project .h file.
```c++
RCH::Oscillators::Templates::Sine oscSine;
```

This will create an oscillator that can produce a continuous sine wave for a single channel of audio, and you will have to poll it on a per-sample basis.

Now that your oscillator is instantiated, you need to inform it about essential values, like the sample rate to operate at and the center frequency to generate the sine wave at, otherwise the oscillator can't process correctly. Set the oscillator up somewhere in your per-block processing call, in JUCE it would be the **processBlock** method in the **PluginProcessor.cpp** file, in WDL it would be the **ProcessDoubleReplacing** method in the main project .cpp file:
```c++
oscSine.setSampleRate(sampleRateInHz);
oscSine.setFrequency(frequencyInHz);
oscSine.setAmplitude(volumeInFloatGain);   // if you use float gain factors (type float)
/* --- or --- */
oscSine.setVolume(volumeInDoubleDecibels); // if you use Decibel values (type double)
```

The *SAW* and *PULSE* oscillators have special properties that are not handled by the regular setup routine. It is not required to set these up to get sound out of the oscillators, but these parameters do offer extended control over the sound, so it's worth mentioning them.
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
  channel[sample] =  oscSine.tick();   // This will replace the incoming signal with the sine wave
  /* --- or --- */
  channel[sample] += oscSine.tick();   // This will add the sine wave on top of the incoming signal
}
```

Your channel of samples will now be filled with a sine wave, and that's all you need to get going.

-------------------------------------------------------------------------------------------------------

## Simplified use

Now, I'm a lazy guy, so I don't always want to cycle through all the channels and all the samples of a buffer by hand, that just takes up time and space. So I've included a set of wrapper classes that deal with this pesky task automatically. You just pass a C array of **double**s or **float**s into the oscillator, as well as two **unsigned int**s with the number of channels and samples in that C array, and the oscillator will handle it from there.

In their current state, these wrapper classes will generate a **single mono oscillator**, and insert or add that mono signal onto **all** the buffer's channels equally. I see no need to have a different oscillator for each channel, it would just make this library needlessly complex to handle.

Theoretically, there is no limitation to the number of channels that could be processed, a buffer block could have a single channel, or over 200 channels, the oscillators don't care. Your CPU might, at some point. :)

To create an example triangle wave oscillator that offers this simple processing method, just instantiate it in your header file like this:
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

If you're using WDL, your **ProcessDoubleReplacing** will provide you a variable named **input** that's already in the correct format.

If you're using JUCE, your **processBlock** method will present you with a variable named **buffer** in a JUCE specific datatype, the **AudioBuffer**. Despite being an abstraction, it still offers direct access to its channels and samples in the format required for the **fill** and **add** methods of these oscillators. Since the oscillator writes directly into the passed buffer, just make sure you go for WritePointers, not ReadPointers:
```c++
oscTriangle.fill(buffer.getArrayOfWritePointers(),numChannels,numSamples); // JUCE specific in processBlock
```

And that's just about all there's to it... :)

By the way: *don't worry about constantly pushing values into the setup() methods, e.g. in **processBlock**. The oscillators are designed to only ever bother with updating their settings internally if the incoming values actually change. If you send identical sample rate, center frequency and volume values into a setup() method 200 times, the oscillators won't care. If one of the values changes on the 201st time, then the oscillators will update.*

-------------------------------------------------------------------------------------------------------

## Example project

For a real world example, check out the included [RCHOscillatorsDemo](https://github.com/rcliftonharvey/rchoscillators/tree/master/demo) project. Yes, it's JUCE based, so you'd need that to build it. But it's also fine to just look into the [PluginProcessor.h](https://github.com/rcliftonharvey/rchoscillators/tree/master/demo/Source/PluginProcessor.h) and [PluginProcessor.cpp](https://github.com/rcliftonharvey/rchoscillators/tree/master/demo/Source/PluginProcessor.cpp) files, they should give you a good enough idea of how easy it is to work with this library.

-------------------------------------------------------------------------------------------------------

## License

This source code is provided under the [MIT License](https://en.wikipedia.org/wiki/MIT_License).<br>
A copy of the MIT license in written form comes in the download of this library.

The JUCE framework itself is shipped and licensed separately, see the [JUCE website](https://juce.com) for more information.<br>
JUCE is only required to compile the demo project, nothing more.

-------------------------------------------------------------------------------------------------------

## Credits

The oscillators in this library were inspired by **Alan Wolfe**'s example code at [Demofox.org](http://demofox.org/). By *inspired*, I don't mean that I merely copied his code snippets 1:1. I much rather used them to understand how they work, and then heavily optimized, extended and also in places simplified his concepts. Without reading the comments in my source code, you probably wouldn't notice much of a relation.

-------------------------------------------------------------------------------------------------------

Enjoy!

Rob Clifton-Harvey
