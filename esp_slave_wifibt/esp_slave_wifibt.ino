#include <esp_now.h>
#include <WiFi.h>
//#include "BluetoothSerial.h"
//BluetoothSerial ESP_BT;


typedef struct test_struct {
  float x;
  float y;
  float z;
} test_struct;

//Create a struct_message called myData
test_struct myData;

//callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("x: ");
  Serial.print(myData.x);
  Serial.print("   y: ");
  Serial.print(myData.y);
  Serial.print("   z: ");
  Serial.print(myData.z);
  Serial.println();

//  ESP_BT.print("x : ");
//  ESP_BT.print(myData.x);
//  ESP_BT.print("   y : ");
//  ESP_BT.print(myData.y);
//  ESP_BT.print("   z : ");
//  ESP_BT.println(myData.z);
}

void setup() {
  
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);
//  ESP_BT.begin("ESP Data Point");
}
int pwm = 0;

void loop() {
  pwm = map(myData.z, )
}
