# low_power_sensor
Low power node  - ATMega328p standalone based environment sensor

 low_power_sensor.ino is dedicated to outside house measurement it measures humidity, temperature, hygrometry and battery voltage
 Tutorial: http://1technophile.blogspot.fr/2016/08/low-cost-low-power-6ua-garden-433mhz.html
 
 low_power_sensor_inside.ino is dedicated to inside house measurement it measures humidity, temperature and battery voltage
 Tutorial: http://1technophile.blogspot.fr/2016/12/low-cost-low-power-room-sensor-with.html
 
  This program enables to send sensor data with low power:
 - send sensor data to a 433Mhz gateway
 - with 3 AA batteries measured at 5V the current consumption in sleep mode is around 6uA

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
