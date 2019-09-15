#include <Wire.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin); //SIM900a object
SoftwareSerial mySerial(9, 10); //MPU6050 object
int sms_send = 0; // Declare a varible for sending 3 sms.
int ledPin = 13; //For check the person falling condition
//Variables
int acc_error = 0;                       //We use this variable to only calculate once the Acc data error
float rad_to_deg = 180 / 3.141592654;    //This value is for pasing from radians to degrees values
float Acc_rawX, Acc_rawY, Acc_rawZ;    //Here we store the raw data read
float Acc_angle_x, Acc_angle_y;          //Here we store the angle value obtained with Acc data
float Acc_angle_error_x, Acc_angle_error_y; //Here we store the initial Acc data error

void setup() {
  Wire.begin();                           //begin the wire comunication
  mySerial.begin(9600);   // Setting the baud rate of GSM Module
  Serial.begin(9600);    // Setting the baud rate of Serial Monitor (Arduino)
  ss.begin(GPSBaud);
  delay(100);
  Wire.beginTransmission(0x68);           //begin, Send the slave adress (in this case 68)
  Wire.write(0x6B);                       //make the reset (place a 0 into the 6B register)
  Wire.write(0x00);
  Wire.endTransmission(true);             //end the transmission

  Wire.beginTransmission(0x68);           //Start communication with the address found during search.
  Wire.write(0x1C);                       //We want to write to the ACCEL_CONFIG register
  Wire.write(0x10);                       //Set the register bits as 00010000 (+/- 8g full scale range)
  Wire.endTransmission(true);
 
  if (acc_error == 0)
  {
    for (int a = 0; a < 200; a++)
    {
      Wire.beginTransmission(0x68);
      Wire.write(0x3B);                       //Ask for the 0x3B register- correspond to AcX
      Wire.endTransmission(false);
      Wire.requestFrom(0x68, 6, true);

      Acc_rawX = (Wire.read() << 8 | Wire.read()) / 4096.0 ; //each value needs two registres
      Acc_rawY = (Wire.read() << 8 | Wire.read()) / 4096.0 ;
      Acc_rawZ = (Wire.read() << 8 | Wire.read()) / 4096.0 ;
      Acc_angle_error_x = Acc_angle_error_x + ((atan((Acc_rawY) / sqrt(pow((Acc_rawX), 2) + pow((Acc_rawZ), 2))) * rad_to_deg));
      Acc_angle_error_y = Acc_angle_error_y + ((atan(-1 * (Acc_rawX) / sqrt(pow((Acc_rawY), 2) + pow((Acc_rawZ), 2))) * rad_to_deg));
      if (a == 199)
      {
        Acc_angle_error_x = Acc_angle_error_x / 200;
        Acc_angle_error_y = Acc_angle_error_y / 200;
        acc_error = 1;
      }
    }
  }
  pinMode(ledPin, OUTPUT);
}

void SendMessage()
{
  float flat, flong;
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\"+91XXXXXXXXXX\"\r"); // Replace x with mobile number
  delay(1000);
  mySerial.print("The Person fallen at this location : ");
  mySerial.print("https://www.google.com/maps/place/");// The SMS text you want to send
  //Serial.print(F("Location: "));

  flat = gps.location.lat();
  //Serial.print(flat);
  flong = gps.location.lng();
  //Serial.print(F(","));
  //Serial.print(flong);
  //mySerial.print(F("Latitude : "));
  mySerial.print(flat, 6);
  //mySerial.print(F("N"));
  //mySerial.print(F("Longitude : "));
  mySerial.print(F(","));
  mySerial.print(flong, 6);
  //mySerial.print(F("W"));
  delay(100);
  mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}

void processing()
{
  while (ss.available() > 0)
    if (gps.encode(ss.read()))
      //displayInfo();

      /* if (millis() > 2000 && gps.charsProcessed() < 10)
        {
         Serial.println(F("No GPS detected: check wiring."));
         while(true);
        }*/
      if (sms_send <= 2)
      {
        delay(10000);
        SendMessage();
        sms_send = sms_send + 1;
      }
      else
      {
        exit(0);
      }
}

void loop() {
  Wire.beginTransmission(0x68);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(0x68, 6, true);
  Acc_rawX = (Wire.read() << 8 | Wire.read()) / 4096.0 ;
  Acc_rawY = (Wire.read() << 8 | Wire.read()) / 4096.0 ;
  Acc_rawZ = (Wire.read() << 8 | Wire.read()) / 4096.0 ;
  Acc_angle_x = (atan((Acc_rawY) / sqrt(pow((Acc_rawX), 2) + pow((Acc_rawZ), 2))) * rad_to_deg) - Acc_angle_error_x;
  //Acc_angle_y = (atan(-1 * (Acc_rawX) / sqrt(pow((Acc_rawY), 2) + pow((Acc_rawZ), 2))) * rad_to_deg) - Acc_angle_error_y;
  Serial.print("AccX angle: ");
  
  Serial.print(Acc_angle_x);
  Serial.println(" ");
 /* Serial.print("AccY angle: ");
  Serial.print(Acc_angle_y);
  Serial.println(" ");*/
  
  
 while (Acc_angle_x >50)
  {
    digitalWrite(ledPin, HIGH);
    processing();
  }

  delay(2000);
}

