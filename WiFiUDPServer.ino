/*
 * Forslag til kommunikationsmodul 
 * En mere 'low level' metode end med den asynkrone UDP Server
 *
 */
#include <WiFi.h>
// #include <WiFiUdp.h>
// #include "esp_wifi.h"

 
// const char* wifi_network_ssid = "yourNetworkName";
// const char* wifi_network_password =  "yourNetworkPassword";
 
const char *ssid_softAP = "MyESP32AP";
const char *password_softAP = "testpassword";
IPAddress local_IP(192, 168, 4, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 0, 0);

/*Definerer maxlængden af indkommen strenge*/
const int incomingPacketLength = 64;
char incomingPacket[incomingPacketLength];  // buffer for incoming packets

/*Definerer namespace for UDP-forbindelsen*/
WiFiUDP udp;

unsigned long t0 = millis();

void setup() {
 
	Serial.begin(115200);
	while (!Serial){ delay(10);}
	
	// 1. Sætter lokal access point op. Formålet er at kommunikere med de andre ESPér
	// WiFi.mode(WIFI_MODE_APSTA);
	WiFi.mode(WIFI_AP_STA);

	if (!WiFi.config(local_IP, gateway, subnet)) {
        Serial.println("STA Failed to configure");
    }
	// Serial.print("Setting StaticIP configuration ... ");
	// Serial.println(WiFi.softAPConfig(staticIP, gateway, subnet) ? "Ready" : "Failed!");

	Serial.print("Start soft-AP access point ... ");
	Serial.println(WiFi.softAP(ssid_softAP, password_softAP) ? "Ready" : "Failed!");

	Serial.print("Soft-AP IP address = ");
	Serial.println(WiFi.softAPIP());
	
	// WiFi.begin(wifi_network_ssid, wifi_network_password);
	// while (WiFi.status() != WL_CONNECTED) {
	//   delay(500);
	//   Serial.println("Connecting to WiFi..");
	// }
 
	// WiFi.begin(ssid_softAP, password_softAP);

	udp.begin(1234);//lytter på port 
}

void loop()
{
    //Lytter efter indkommende data-pakker i hvert tick
    int packetSize = udp.parsePacket();//listen for incomming packets
    if (packetSize>0)
    {	
        //int len = udp.read(incomingPacket, incomingPacketLength);
        int len = udp.read(incomingPacket, packetSize);
        Serial.println("læser");
		
        if (len > 0)
        {	
            incomingPacket[len] = '\0';
            Serial.printf("UDP packet contents: %s, %d, %d \n", incomingPacket,len,packetSize);
        }
    }

    //sender besked. Anvender non-bloking delay - 
    if(millis()-t0 > 2345){
        udp.beginPacket("192.168.4.2",12345);
        udp.printf("Besked fra Esp32 nr 2");
        udp.endPacket();
        t0 = millis();//opdaterer
        Serial.println("besked");
		
    }
}



