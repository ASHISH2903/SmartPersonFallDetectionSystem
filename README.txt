# PersonFallDetectionSystem

#Abstract

Most of people likes living independently at home. Some activity in our daily life is prone to have some accidents, such as falls. 44
Falls can make people in fatal conditions, even death. 
A prototype of fall detection system using accelerometer and gyroscope based on smartphone is presented in this project. 
Accelerometer and gyroscope sensors are embedded in Arduino UNO R3 device to get the result of fall detection more accurately. 
Automatic SMS alert will be sent to family members if someone using smartphone in fatal condition and need some help. 
This project also can distinguish condition of people between falls and activity daily living. 
Several Scenarios were executed in these project.
The result showed that the proposed system could successfully record level of accuracy of the fall detection system till 93.3% in activity daily living and error detected of fall was 2%.


####Keywords - Falling detection, accelerometer, gyroscope, smart phone

#Steps:


1)  First you have to learn GyroScope sensors. For that run GyroscopeLedControll.ino file. 
    To run this file you need MPU6050 library file. So download MPU6050.zip and save at Arduino/Library location and Unzip the folder.
2)  For GSM service, you have to use GSM_SMS.ino file.For that you need GPRS_SIM900-master library file.
    So download GPRS_SIM900-master.zip and save at Arduino/Library location and Unzip the folder.
3)  For GPRS service, you have to use GPS_2.ino file.For that you need TinyGPSPlus-master library file.
    So download TinyGPSPlus-master.zip and save at Arduino/Library location and Unzip the folder.
4)  Final you can use MPU6050_GPS_GSM_FINAL.ino file for whole project.

