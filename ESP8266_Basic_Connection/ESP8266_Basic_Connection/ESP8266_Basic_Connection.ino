/*
 Name:		ESP8266_Basic_Connection.ino
 Created:	11/3/2016 7:38:13 PM
 Author:	Tim Harry
*/
#include <SoftwareSerial.h>
#include <string.h>

SoftwareSerial esp8266(2, 3); //setups a UART comport with pin 2 as RX. pin 3 as TX.

//static variables
String ssid = "\"riversedge\"";
String pwd = "\"riversedge\"";

void sendCmd(String in, int x = 50)
{
	Serial.println(in);
	esp8266.println(in.c_str());
	esp8266.flush();
	delay(x);
}

// the setup function runs once when you press reset or power the board
void setup() 
{
	//Setup arduino serial connection
	Serial.begin(9600);
	while (!Serial)
	{
		//waits for the serial port to actually make a connection
	}
	Serial.println("Arduino connection is successful.");

	//Setup esp8266 port
	esp8266.begin(9600);
	Serial.println("Resetting module...");
	sendCmd("AT+RST", 2000);
	while (esp8266.available() || Serial.available())
	{
		Serial.println(char(esp8266.read()));
		esp8266.println(char(Serial.read()));
	}

	if (esp8266.find("OK"))
	{
		delay(5000);
		Serial.println("Module is ready");
	}
	Serial.println("Setting mode to client and AP");
	sendCmd("AT+CWMODE=3");
	while (esp8266.available() || Serial.available())
	{
		Serial.println(char(esp8266.read()));
		esp8266.println(char(Serial.read()));
	}
	Serial.println("Connecting to router...");
	String connect = "AT+CWJAP=" + ssid + "," + pwd;
	sendCmd(connect, 3000);
	while (esp8266.available() || Serial.available())
	{
		Serial.println(char(esp8266.read()));
		esp8266.println(char(Serial.read()));
	}

	sendCmd("AT+CWJAP?");
	while (esp8266.available() || Serial.available())
	{
		Serial.println(char(esp8266.read()));
		esp8266.println(char(Serial.read()));
	}
	char* wifiName = const_cast<char *>(ssid.substring(1, ssid.length() - 1).c_str());
	if (esp8266.find("OK"))
		Serial.println("Wifi is connected.");

	esp8266.flush();

}

// the loop function runs over and over again until power down or reset
void loop() 
{
	if (esp8266.available())
		Serial.write(esp8266.read());
	if (Serial.available())
		esp8266.write(Serial.read());

}
