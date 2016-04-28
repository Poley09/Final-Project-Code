#include "DataCoder.h"
#include <VirtualWire.h>
#include "ServoTimer2.h"

const int rx_pin = 12; // Pin Attached to receiver
const int led_pin = 13; // Pin Attached to LED
const int baudRate = 2000; // Transfer rate of the Receiever
#define PanPin  9  // Pin Attached to Pan Servo
#define TiltPin 10 // Pin Attached to Tilt Servo

ServoTimer2 servoPan; 
ServoTimer2 servoTilt;

void setup()
{
  delay(1000);
  Serial.begin(9600);
  pinMode(led_pin,OUTPUT);
  digitalWrite(led_pin,LOW);
  SetupRFDataRxnLink(rx_pin, baudRate);
  servoPan.attach(PanPin);  // Attach Pan Servo
  servoTilt.attach(TiltPin);   // Attach Pan Servo

}

void loop()
{
	
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  
  union RFData inDataSeq;//To store incoming data
  
  float inArray[RF_DATA_ARRAY_SIZE];//To store decoded information
  
  
  if(RFLinkDataAvailable(buf, &buflen))
  {
        digitalWrite(led_pin, HIGH); // Turn LED on at start of Signal Received
        
        for(int i =0; i< buflen; i++)
        {
          inDataSeq.s[i] = buf[i];
        }
        
        Serial.println();
        
        digitalWrite(led_pin, LOW); // Turn LED off at end of Signal Received
      
        DecodeRFData(inArray, inDataSeq); // Decode the signal received

       
       // Serial.print("Incoming Yaw, Pitch:");
        
        for (int i=0; i < RF_DATA_ARRAY_SIZE; i++) // Display decoded array value, i represents the amount of values sent, in this case it is 2
          {
            Serial.print(inArray[i]);
            Serial.print(", ");
            
          }
          
        inArray[0] = map(inArray[0], -90, 90, 2500, 544); // Map -90 degree to 90 Degree to 544 - 2500 mS pulse for Servo
        
        Serial.println(); // New Line in Serial Monitor
        Serial.println("Yaw in mS for Pan:");
        Serial.print(inArray[0]);// Print Yaw value in mS
        
        servoPan.write(inArray[0]); // Send the calculated Yaw mS pulse to Pan Servo

  
        inArray[1] = map(inArray[1], -90, 90, 544, 2500); // Map -90 degree to 90 Degree to 544 - 2500 mS pulse for Servo
        
        Serial.println(); // New Line in Serial Monitor
        Serial.println("Pitch in mS for Tilt:");
        Serial.print(inArray[1]); // Print Pitch value in mS
        
        servoTilt.write(inArray[1]); // Send the calculated Pitch mS pulse to Tilt Servo
  
  }
  
}

