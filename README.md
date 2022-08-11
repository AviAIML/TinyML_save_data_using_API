# TinyML_save_data_using_API
This repository shows how to save data for either microphone or sensor.

Use Arduino_TinyML_Libraries with path https://github.com/AviAIML/Arduino_TinyML_libraries

By default the fill will save two 10 seconds Audio files. The audio file will be saved as a CSV file and can be opened in EXCEL.
It can be converted into the wav file using python program saved in the extra directory.

If you uncomment following line, it will save sensor data.
//#define Sensor                                              // Un-comment this line for sensor usage
The 6 axss data is saved in the order gyro_x, gyro_y, gyro_z, acc_x, acc_y and acc_z, 
