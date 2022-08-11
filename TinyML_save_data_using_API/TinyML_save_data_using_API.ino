
// This sketch saves continuous data from inception in uSD card
// This sketch does not trigger on inferences.
#include <NDP.h>                                            // Arduino_library/libraries/NDP/src/
#include <NDP_utils.h>                                      // Arduino_library/libraries/TinyML_NDP_utils/src
#include <Arduino.h>
#include "TinyML_init.h"                                    // Arduino_library/libraries/TinyML_lowpower_lib
#include "SAMD21_init.h"                                    // Arduino_library/libraries/TinyML_lowpower_lib
#include "NDP_init.h"                                       // Arduino_library/libraries/TinyML_lowpower_lib
#include "SdFat.h"                                          // Arduino_library/libraries/SdFat/src
extern SdFat SD;

#define DO_NOT_WAIT_FOR_SERIAL_PORT 0                       // For battery powered autonomus usage, use this option
#define WAIT_FOR_SERIAL_PORT 1                              // For debugging purposes use this option, program will wait until serial port is turned on, Arduino IDE Tools --> Serial Monitor
//#define Sensor                                              // Un-comment this line for sensor usage
#define NDP_MICROPHONE 0                                    // Model type (Audio or Sensor) and this slection MUST match
#define NDP_SENSOR 1                                        // Model type (Audio or Sensor) and this slection MUST match

void saveDataInFile(int fileNumber, int lengthInSeconds){   // fileNumber will generate unique file name automatically, lengthInSeconds is the desired length in seconds
  int s;                                                    // Return error message for the API
  #ifdef Sensor
    const int dataLengthToBeSaved = 72;                     // Use 72 for sensor data. 2X of the data captured between the inferences.
    const int samplesPerSecond = 600;                       // Sampling rate of the sensor is fixed at 100 and with 6 axes
  #else
    const int dataLengthToBeSaved = 768;                    // 768 is 2x of 384 which is collected between inferences
    const int samplesPerSecond = 16000;                     // Audio sampling rate is 16k/s
  #endif  
  unsigned int len = dataLengthToBeSaved;                   // variable len represnts either the length of the captured buffer by the API or available length of the buffer
  int16_t dataBuf[dataLengthToBeSaved];                     // Local buffer to hold the data for saving into uSD card
  int cumulativeDataSaved = 0;                              // This variable keeps track of the cumulative data saved
  int lengthOfData=0;                                       // This is minimum of the data stored in dataBuf or len/2. Len is in bytes where as dataLengthToBeSaved represents 16 bit.
  File myFile;                                              // File
  char dataFileName[100];                                   // String for file name to be saved 
  
  int n = sprintf(dataFileName, "data_%d.csv",fileNumber);  // Generating file name to be saved
  while (!SD.begin(SDCARD_SS_PIN)) {                        // Waiting for uSD card to be inserted. If the card is inserted without having the model bin file, then the code will hang with RGB LED = Blue  
      Serial.println("SD file not opened, insert uSD card. ");                
      delay(1000);                                          // Check if SD card inserted
  }
  myFile = SD.open(dataFileName, FILE_WRITE);               // Opening file in write mode
  myFile.remove();                                          // Deleteting the file if already existed to avoid appending it
  myFile = SD.open(dataFileName, FILE_WRITE);               // Opening file in write mode
  myFile.rewind();                                          // Go to start of file
  Serial.print("SD file opened ");  
  Serial.println(dataFileName);
  
  len = 5000;                                               // This be starting lag before start saving data
  while (len > 4999){                                       // Flushing out buffer leaving length
    len = sizeof(dataBuf);                                  // It is important to keep len to be same or smaller than than the databuf
    s = NDP.extractData((uint8_t *) dataBuf, &len);         // Calling API
    Serial.print("len ");
    Serial.println(len);
  }
  Serial.println("Circular FIFO data flushed out");         
  
  while(cumulativeDataSaved<(lengthInSeconds*samplesPerSecond)) {      // Extracting data and saving into uSD card
    len = sizeof(dataBuf);                                  // It is important to keep len to be same or smaller than than the databuf
    s = NDP.extractData((uint8_t *) dataBuf, &len);         // Calling API
    if ((len/2)>5000) Serial.println(len/2);                // Generating warning if backlog increased beyond 5000 mark, this is twice the starting point (len/2)
    if ( (len/2) >= dataLengthToBeSaved){                   // Finding minmum of len/2 and databuf array depth
      lengthOfData = dataLengthToBeSaved;
    }
    else{
      lengthOfData = len/2;
    }
    cumulativeDataSaved +=  lengthOfData;                   // updating cumultiveDataSaved variable
    for (int i=0; i<lengthOfData; i++){                     // Saving data into uSD card, two bytes equivalent in int form
       myFile.println(dataBuf[i]);
    }
  }
  myFile.close();                                           // Closing file
  Serial.print("Data Saved in file: ");
  Serial.print("SD file opened ");  
  Serial.println(dataFileName);
}

void setup(void) {
  SAMD21_init(WAIT_FOR_SERIAL_PORT);                        // Setting up SAMD21, the program will wait with Red LED
  #ifdef Sensor
    Serial.print("Sensor applicaion");
    NDP_init("ei_model_sensor.bin",NDP_SENSOR);             // Setting up NDP, Sensor model, Sensor=1
  #else
    Serial.print("Microphone Application");
    NDP_init("ei_model.bin",NDP_MICROPHONE);                  // Setting up NDP, Audio model,  microphone=0
  #endif
}

int loopCount=0;                                            // Variable to generate multiple files    
int lengthInSeconds=10;
void loop() {
  if (loopCount<2){                                         // Choose very large number or remove this contraint to save files indefinitely 
    digitalWrite(LED_GREEN, HIGH);                          // RGB = Green indicating file is being saved
    saveDataInFile(loopCount,lengthInSeconds);              // Setting up file number and time for data saving
    digitalWrite(LED_GREEN, LOW);                           // RGB = off indicating file saving is done
  }
  loopCount++; 
}//./
