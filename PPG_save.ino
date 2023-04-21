#include <Wire.h>
#include "MAX30100.h"
#include <SD.h> // Add the SD library

// Set the chip select pin for your SD card module
const int chipSelect = 10;

File dataFile; // Declare a File object for writing to the SD card

// Sampling is tightly related to the dynamic range of the ADC.
// refer to the datasheet for further info
#define SAMPLING_RATE                       MAX30100_SAMPRATE_100HZ

// The LEDs currents must be set to a level that avoids clipping and maximises the
// dynamic range
#define IR_LED_CURRENT                      MAX30100_LED_CURR_50MA
#define RED_LED_CURRENT                     MAX30100_LED_CURR_27_1MA

// The pulse width of the LEDs driving determines the resolution of
// the ADC (which is a Sigma-Delta).
// set HIGHRES_MODE to true only when setting PULSE_WIDTH to MAX30100_SPC_PW_1600US_16BITS
#define PULSE_WIDTH                         MAX30100_SPC_PW_1600US_16BITS
#define HIGHRES_MODE                        true


// Instantiate a MAX30100 sensor class
MAX30100 sensor;

void setup()
{
    Serial.begin(115200);
    //Wire.begin(SDA_OLED, SCL_OLED); only use for easci products.
    Wire.begin(SDA, SCL);
    Serial.print("Initializing MAX30100..");

    // Initialize the sensor
    // Failures are generally due to an improper I2C wiring, missing power supply
    // or wrong target chip
    if (!sensor.begin()) {
        Serial.println("FAILED");
        for(;;);
    } else {
        Serial.println("SUCCESS");
    }

    // Set up the wanted parameters
    sensor.setMode(MAX30100_MODE_SPO2_HR);
    sensor.setLedsCurrent(IR_LED_CURRENT, RED_LED_CURRENT);
    sensor.setLedsPulseWidth(PULSE_WIDTH);
    sensor.setSamplingRate(SAMPLING_RATE);
    sensor.setHighresModeEnabled(HIGHRES_MODE);

    // Initialize the SD card
    if (!SD.begin(chipSelect)) {
        Serial.println("SD card initialization failed!");
        return;
    }

    // Open a file for writing on the SD card
    dataFile = SD.open("data.txt", FILE_WRITE);
    if (!dataFile) {
        Serial.println("Could not open file for writing!");
        return;
    }
}

void loop()
{
    uint16_t ir, red;

    sensor.update();

    while (sensor.getRawValues(&ir, &red)) {
        // Print the raw values to the serial monitor
        Serial.print(ir);
        Serial.print('\t');
        Serial.println(red);

        // Write the raw values to the SD card
        dataFile.print(ir);
        dataFile.print('\t');
        dataFile.println(red);
    }

    // Flush the data to the SD card to avoid data loss
    dataFile.flush();
}
