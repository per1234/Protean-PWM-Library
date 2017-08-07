# Protean PWM Library
This library is designed to be used in conjunction with the [Protean PWM Logger](http://www.protean.io/product/pwm-logger). The logger can decode and generate R/C servo signals, this library facilitates its use with an Arduino.

![](http://www.protean.io/imgs/tutorials/pwm-logger/28-logger-demo.gif)

For a hookup guide, please go checkout the [product page](http://www.protean.io/product/pwm-logger).

## Installation

Open up the Arduino IDE, make sure the sketch that was opened has been saved. After that click **Sketch -> Show Sketch Folder**. This will bring you to your sketch's location on your computer.
![Sketch folder](http://www.protean.io/imgs/tutorials/pwm-logger/31-find-sketch-folder.png)

Next, navigate back one directory. This will bring you to your sketchbook directory. It should contain a folder called libraries. Navigate into libraries then use git to clone Protean-PWM-Library into the libraries directory. Once that's done, restart the Arduino IDE. You will then be able to include the library in your sketch like so.

```c++
#include <PWM.h>
```

## API Examples

The PWM class is contained in the namespace **Protean**, so defining a PWM instance could look something like this.

```c++
Protean::PWM myLogger;
```

### Configure...

Reboot the logger board by calling **reset()**.
```c++
myLogger.reset();
```

Disable or enable echo mode. Echo mode passes the PWM signals from the receiver to the servo when it's enabled. When it's disabled it the last timing written-to or decoded for that channel is generated.
```c++
myLogger.setEcho(true);    // Enable echo mode

myLogger.setEcho(false);   // Disable echo mode
```

### Read...
Perform a simple read on channel 1.
```c++
int timing = myLogger.readTimingRaw(1);
```

Read 3 consecutive timings. Starting on channel 1 but ending with 3. The results are stored in the array **timings**, where index 0 is the first channel read.
```c++
int timings[3];
const int count = 3;
const int startChannel = 1;
myLogger.readTimingsRaw(startChannel, count, timings);
```

### Write...

Perform a single write to channel 1. **Note:** You must first have disabled echo mode.
```c++
int someRawTiming = 117;
myLogger.writeTimingRaw(1, someRawTiming);
```

Write 3 consecutive timings. Starting on channel 1 but ending with 3. The timings written are **someRawTimings**, where index 0 is the first channel written.
```c++
int someRawTimings[3] = { 90, 117, 130 };
const int count = 3;
const int startChannel = 1;
myLogger.writeTimingsRaw(startChannel, count, someRawTimings);
```
