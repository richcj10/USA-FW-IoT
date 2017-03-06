/* 
  WiFiTelnetToSerial - Example Transparent UART to Telnet Server for esp8266

  Copyright (c) 2015 Hristo Gochkov. All rights reserved.
  This file is part of the ESP8266WiFi library for Arduino environment.
 
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include <ESP8266WiFi.h>

//how many clients should be able to telnet to this ESP8266
#define MAX_SRV_CLIENTS 1
const char* ssid = "USAFirm_a2";
const char* password = "art_FW_experts";

int x = 0;
char data[14];
char str[50];
int light = 0;
int value = 0;
WiFiServer server(21);
WiFiClient serverClients[MAX_SRV_CLIENTS];

void ControlOut();

void setup() {
  pinMode(5,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(2,OUTPUT);
  pinMode(15,OUTPUT);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("\nConnecting to "); Serial.println(ssid);
  uint8_t i = 0;
  while (WiFi.status() != WL_CONNECTED && i++ < 20) delay(500);
  if(i == 21){
    Serial.print("Could not connect to"); Serial.println(ssid);
    while(1) delay(500);
  }
  server.begin();
  server.setNoDelay(true);
  
  Serial.print("Ready! Use 'telnet ");
  Serial.print(WiFi.localIP());
  Serial.println(" 21' to connect");
}

void loop() {
  uint8_t i;
  //check if there are any new clients
  if (server.hasClient()){
    for(i = 0; i < MAX_SRV_CLIENTS; i++){
      //find free/disconnected spot
      if (!serverClients[i] || !serverClients[i].connected()){
        if(serverClients[i]) serverClients[i].stop();
        serverClients[i] = server.available();
        Serial.print("New client: "); Serial.print(i);
        continue;
      }
    }
    //no free/disconnected spot so reject
    WiFiClient serverClient = server.available();
    serverClient.stop();
  }
  //check clients for data
  if (serverClients[0].connected()){
    if(serverClients[0].available()){
      //get data from the telnet client and push it to the UART
      while(serverClients[0].available()){
        data[x] = serverClients[0].read();
        x++;
      }
      x = 0;
      if (data[0] == '$'){
        Serial.println("got start");
        light = data[1] -48;
        value = data[2] -48; 
        Serial.print("Light ");
        Serial.print(light);
        if(value == 1){
          Serial.println(" ON");
        }
        else if(value == 0){
          Serial.println(" OFF");
        }
        ControlOut();
      }
      else if (data[0] == '*'){
        Serial.println("got summon");
        serverClients[0].print("VD#1#123\n");
      }
    }
   }
}

void ControlOut(){
  switch (light){
    case 1: 
      digitalWrite(5,value);
    case 2: 
      digitalWrite(4,value);
    case 3: 
      digitalWrite(2,value);
    case 4: 
      digitalWrite(15,value);     
  }
}
