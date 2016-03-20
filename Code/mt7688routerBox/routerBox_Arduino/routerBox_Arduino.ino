/*
 * File: routerBox_Arduino.ino
 * 
 * Author: Lambor
 * Date: 2016-03-10
 *
 * In this demo, mt7688Duo as a router, its net speed and WiFi users' number 
 * will be determined and save to files("/tmp/netSpeed", "/tmp/wifi_user_number").
 *
 * Mertirials: RGB LED Strip with 10 leds
 *             mt7688Duo
 *             Arduino Breakout for LinkIt Smart 7688Duo
 * 
 */
#include <Process.h>
#include "Adafruit_NeoPixel.h"
#include <string.h>

#define PIXIEL_PIN 6
#define PIXIEL_NUM 10


Adafruit_NeoPixel pixels= Adafruit_NeoPixel(PIXIEL_NUM, PIXIEL_PIN, NEO_GRB + NEO_KHZ800);

enum routerInfo { user_num=0, net_speed=1 };

#define BUF_SIZE  8
char *g_Buffer;
int g_BufferIndex;

volatile uint8_t current_color;
volatile uint8_t last_color;
  
void bridgeCatFile(int routerInfo)
{
  Process p;
  int index;
  
  g_Buffer = (char *)malloc(BUF_SIZE);
  memset(g_Buffer, '\0', BUF_SIZE);
  
  p.begin("cat");
  if(routerInfo == user_num){
    p.addParameter("/tmp/wifi_user_number");    
  } else if (routerInfo == net_speed) {
    p.addParameter("/tmp/netSpeed");
  }
  p.run();
  
  while (p.available() > 0) {
    char c = p.read();
    g_Buffer[index++] += c;
  }
  g_Buffer[index++] += '\0';  
  g_BufferIndex = index;
}

uint16_t getNetSpeed(){
  String data = "";
  int index = 0;
  int number = 0;
  
  bridgeCatFile(net_speed);
  index = g_BufferIndex;
  
  for(int i=0;i< g_BufferIndex-1; i++){
    data += (String)g_Buffer[i];    
  }
  number = data.toInt();
  free(g_Buffer);
  
  Serial.print("Speed: ");
  Serial.println(number);
  
  return number;
}

uint16_t getWiFiUser(){
  String data = "";
  int index = 0;
  int number = 0;
  
  bridgeCatFile(user_num);
  index = g_BufferIndex;
  for(int i=0;i< g_BufferIndex-1; i++){
    data += (String)g_Buffer[i];    
  }
  number = data.toInt();
  free(g_Buffer);  
  
  Serial.print("User: ");
  Serial.println(number);
  
  return number;
  
}

void pixels_Action(uint16_t speed, uint16_t users)
{
  uint16_t maxSpeed = 1024;
  uint16_t minSpeed = 0;
  uint8_t colorRange = 255;
  uint8_t color;
  uint32_t _color;  
  
  current_color = map(speed > maxSpeed ? maxSpeed : speed, 0, maxSpeed, 0, colorRange);
  
  for(color=last_color;
    current_color > last_color ? (color < current_color):(color > current_color);
    current_color > last_color ? (color++):(color--))
  { 
    _color = pixels.Color(color, 0, 255 - color);  
    
    for(int i=6; i < pixels.numPixels(); i++)
    {      
        pixels.setPixelColor(i, _color);
    }              
    pixels.show();
    delay(10);
  }  
  
  for(int i=0; i < 6; i++){    
    if(i < users){
      pixels.setPixelColor(i, 0x00FF00);    
    }else {
      pixels.setPixelColor(i, 0);
    }     
  }  
  pixels.show();
  
  last_color = current_color;
}

void setup() {
  Serial.begin(9600);
  
  // Initialize led strip
  pixels.begin(); 
  pixels.clear();
  pixels.setBrightness(20);

  // Initialize Bridge
  Bridge.begin();
}

void loop() {
  int speed = getNetSpeed();
  int user_num = getWiFiUser();
  
  pixels_Action(speed, user_num);
     
  delay(1000);
}



