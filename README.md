Project Execution Instructions

Arduino Environment Setup

Using Arduino IDE:

Complete the basic setup of the Arduino IDE.

Open the Library Manager and download the following libraries:

Adafruit MPU6050

Adafruit Unified Sensor

Blynk

ESP8266WiFi

Wire

Update the Blynk and Wi-Fi credentials in the code.

Select the correct port for your device.

Verify the code.

Using Arduino Cloud:

Install the Arduino Cloud Agent.

Insert the code into the Arduino Cloud interface.

Compile and verify the code.

Blynk Setup

Log in to your Blynk account.

Create a new template with 13 virtual pins to display readings and exercise counts.

Create a device from the template.

Connections from NodeMCU to MPU6050

VCC to 3V

GND to GND

SCL to D1

SDA to D2

Final Steps

Upload the code to the NodeMCU module.

Connect the module to Wi-Fi.

Once the online status is displayed in Blynk, view your outputs in the app.
