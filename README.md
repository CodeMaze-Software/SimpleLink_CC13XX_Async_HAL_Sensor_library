

---
# CC1310 Async HAL Sensor Library

---

## Demo Summary

Sample application showing library operation in asynchronous mode for any external sensor.
The demo has been tested on the CC1310 LaunchPad and uses built-in LEDs - red and green which indicate the 
current operation of the driver.

## Peripherals Exercised

- RED LED - informs about every workflow of the scheduler.
- GREEN LED - informs about receiving data from the sensor in asynchronous mode.

UART works with 115200 bauds.

## Application Design Details

Driver files:

main.c
sensor_api.c
sensor_api.h
sensor_api_user_defines.c (here we refer to the user's library.h)
sensor_api_user_defines.h
user library c
user library h
sensor_i2c.c
sensor_i2c.h

The library allows you to easily add any sensor via I2C or SPI with your own sensor driver implementation. Changing 
the communication interface (I2C, SPI, UART etc.) or changing the sensor driver does not change the logic of the main 
program. 

The library enables simultaneous operation of two sensors for redundancy, with automatic switching of addresses on 
the I2C line. A simple scheduler automatically switches between Sensor1 and Sensor2 and is responsible for waking 
up and putting each sensor to the sleep. The application based on of an internal timer, so once launched, it works all 
the time in the background. 

The logger operates on the basis of callback, each time it transmits the measurement result and information in case of 
exceeding the threshold. The type of the threshold overflow is distinguished - whether the lower or upper threshold has 
been exceeded. Callback can handle print on UART, both in synchronous and asynchronous mode.

## Driver usage

main.c -> API -> [user defined sensor library] -> communication (I2C)


The software requires initialization of the sensor and entering the address for the first and second sensors as below:

sensorConfig_t mySensor;

mySensor.min = -10;
mySensor.max = 25;
mySensor.priAddress = 0x48; 
mySensor.secAddress = 0x49; 
mySensor.threshold_cnt = 10; // (10 cycles * 100 mS timer tick) = 1 Sec threshold

sensorApi_Init(&mySensor);

After initialization, it is possible to change the previously set thresholds:

sensorApi_SetThresholds(&mySensor);

Then start the automatic operation of the controller. From this point on, the program runs in the background, it is 
possible to execute other instructions:

 sensorApi_AutoPollingEnable(true);
 
It is possible to stop the automatic mode and manually access the sensor by:

sensorApiStatus_t st;
st = sensorApi_singleRead(mySensor.priAddress);
st = sensorApi_singleSleep(mySensor.priAddress, true);

Finally, you need to register a callback for our application which will be performed after each measurement and together with the log-information in case of exceeding the value and time of the threshold.

 sensorApi_RegisterLogCallback(sensorApi_Callback);

Test
The software was tested on the LIS3DH accelerometer in I2C mode and the AS6212 thermometer - especially sensors of completely different types.

Sensor library replacement

The driver can support any user library thanks to the files sensor_api_user_defines.c and sensor_api_user_defines.h. The predefined functions should contain references to your own sensor library. The sensor library should register the I2C callback (or other, e.g. SPI in asynchronous mode) and refer to the api function:

sensorApi_onResult (result);

giving it the result of the operation.

## Todo
- adding user_defines_timer to make timer independent from CC1310 (full HAL)
- adding other peripherals, e.g. SPI
