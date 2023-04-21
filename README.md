# max30100_PPG_signals_save_SD

In this modified code, we added the SD.h library, declared a File object called dataFile for writing to the SD card, and initialized the SD card in the setup() function. We also added code to open the file for writing and to write the raw data to the file in the loop() function. Finally, we added a call to dataFile.flush() to ensure that the data is written to the SD card and not buffered in memory. Note that you should replace the file name "data.txt" with your desired file name.
