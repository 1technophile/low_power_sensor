/*
  Low power node  - ATMega328p program to send humidity, temperature, hygrometry and battery voltage
 
  This program enables to send sensor data with low power:
 - send sensor data to a 433Mhz gateway
 - with 3 AA batteries measured at 5V the current consumption in sleep mode is around 5uA

  Contributors:
  - 1technophile

  Based on the libraries:
  - RCSwitch
  - LowPower

  Based on the work of:
  Nick Gammon : http://www.gammon.com.au/power
  Rocketscream: https://github.com/rocketscream/Low-Power
  Tinkerit: https://code.google.com/archive/p/tinkerit/wikis/SecretVoltmeter.wiki

  Documentation:
  Project home: https://github.com/1technophile/low_power_sensor
  Blog: https://1technophile.blogspot.com/

Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
and associated documentation files (the "Software"), to deal in the Software without restriction, 
including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, 
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED 
TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF 
CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/
#include "DHT.h"
#include "LowPower.h"
#include <RCSwitch.h>
#include <string.h>

#define DHTTYPE DHT11
    
RCSwitch mySwitch = RCSwitch();

//Time used to wait for an interval before resending data
unsigned long time;

//Pin on which the sensors are connected
const int PresencePin = 8;
const int LedPin = 9;
const int DhtPin = 3;
const int DhtPowerPin = 4;
const int HygPowerPin = 10;
const int EmitPin = 6;
const int EmitPowerPin = 7;

DHT dht(DhtPin,DHTTYPE);

// define humidity variable to hold the final value
int humidity = 0;

//Do we want to see trace for debugging purposes
#define TRACE 1  // 0= trace off 1 = trace on

//Base value for the RF sending, the value of the sensors are added to these
//values so as to differentiate sensors between them
#define HUM   "31000"
#define TEMP  "33000"
#define HYGRO "34000"
#define VOLT  "35000"
#define ERRORCODE  "99999"

// define humidity variable to hold the final value
int hygro = 0;

void setup()
{
  Serial.begin(9600);
  // initialize the input for presence detection
  pinMode(HygPowerPin,INPUT);
  pinMode(DhtPowerPin,INPUT);
  pinMode(EmitPowerPin,INPUT);
  // start led signal to verify at startup that the sensor work
  pinMode(LedPin,OUTPUT);
  digitalWrite(LedPin, HIGH);
  delay(500);
  digitalWrite(LedPin, LOW);
  pinMode(LedPin,INPUT);
  
  // Launch traces for debugging purposes
  trc("Start of the program");
}

void loop()
{

  //Sleep with low power mode
  sleepSeconds(3000);

  //WConfigure sending
  pinMode(EmitPowerPin,OUTPUT);
  digitalWrite(EmitPowerPin, HIGH);
  mySwitch.enableTransmit(EmitPin);  // Using Pin #6
  mySwitch.setRepeatTransmit(10); //increase transmit repeat to avoid lost of rf sendings

  // send battery voltage
  sendData(vccVoltage(), atol(VOLT));
  
  // send temp and hum
  pinMode(DhtPowerPin,OUTPUT);
  digitalWrite(DhtPowerPin, HIGH);
  dht.begin();
  TempAndHum();
  digitalWrite(DhtPowerPin, LOW);
  pinMode(DhtPin,INPUT);//disable the internal pullup resistor enable by dht.begin
  pinMode(DhtPowerPin,INPUT);

  // send soil hygro
  pinMode(HygPowerPin,OUTPUT);
  digitalWrite(HygPowerPin, HIGH);
  Hygro();
  digitalWrite(HygPowerPin, LOW);
  pinMode(HygPowerPin,INPUT);

  //deactivate the transmitter
  mySwitch.disableTransmit();
  //set emit 
  pinMode(EmitPowerPin,INPUT);
}

// sleep function
void sleepSeconds(int seconds)
{
  for (int i = 0; i < seconds; i++) { 
     LowPower.powerDown(SLEEP_1S, ADC_OFF, BOD_OFF); 
  }
}

void TempAndHum(){
    sleepSeconds(1);
    //retreiving value of temperature and humidity from DHT
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    if (isnan(h) || isnan(t)) {
      trc("Failed to read from DHT sensor!");
      sendData(atol(ERRORCODE), atol(HUM));//send error code
      sendData(atol(ERRORCODE), atol(TEMP));//send error code
    } else {
      sendData(int(h*10), atol(HUM));
      sendData(int(t*10), atol(TEMP));
    }
}

void Hygro(){
   // read soil moisture from sensor
    sleepSeconds(1);
    int sensorValue = analogRead(A0);
    sensorValue = constrain (sensorValue, 300,1023);
    int hygro = map (sensorValue, 0, 1023, 100, 0);
    sendData(hygro, atol(HYGRO));
}

void sendData(long dataTosend, long dataType){
  long sum = atol(ERRORCODE);
  
  trc(String(dataTosend));
  trc(String(dataType));
  
  if (dataTosend == sum) {
     // nothing to do sending error code
   } else {
     sum = dataTosend + dataType; // sending value added to topic offset
   }
  
  trc("Sum");
  trc(String(sum));
  
  //sending value by RF
  mySwitch.send(sum,24);
  
}


long vccVoltage() {
  long result = 0;
  // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(1); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA,ADSC));
  result = ADCL;
  result |= ADCH<<8;
  result = 1126400L / result; // Back-calculate AVcc in mV
  return result;
}


//trace function
void trc(String msg){
  if (TRACE) {
  Serial.println(msg);
  }
}
