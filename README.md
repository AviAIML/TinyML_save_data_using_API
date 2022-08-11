# TinyML_save_data_using_API
This repository shows how to save data for either microphone or sensor.

Use Arduino_TinyML_Libraries with path https://github.com/AviAIML/Arduino_TinyML_libraries
Set this path in the Sketchbook location of File --> Preferences menu

It is expected that both microphone and sensor based projects are completed with Edge Impulse flow and ei_model.bin (microphone) and ei_model_sensor.bin (sensor) models are already residing in the serial flash. If these files are not present then these can be copied into the uSD card from the extra directory. In this case, you can leave the uSD card plugged in while the Sketch is booting up.

By default the fill will save two 10 seconds Audio files. The audio file will be saved as a CSV file and can be opened in EXCEL.
It can be converted into the wav file using python program saved in the extra directory.

If you uncomment following line, it will save sensor data.
//#define Sensor                                              // Un-comment this line for sensor usage
The 6 axss data is saved in the order gyro_x, gyro_y, gyro_z, acc_x, acc_y and acc_z

The program will wait until Tool-->Serial Monitor is not invoked to open the Serial Monitor.
