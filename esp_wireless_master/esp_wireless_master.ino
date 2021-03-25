// WITHOUT YELLOW ESP 32 - 24:6F:28:B5:3C:EC
// WITH YELLOW ESP 32 - F0:08:D1:C9:1D:30
// NODEMCU - 98:F4:AB:B3:D4:81


#include <esp_now.h>
#include <WiFi.h>

#include <TFMPlus.h>  // Include TFMini Plus Library v1.4.1
TFMPlus tfmP;

#include <MPU6050_tockn.h>
#include <Wire.h>
MPU6050 mpu6050(Wire);


// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] =  {0x98, 0xF4, 0xAB, 0xB3, 0xD4, 0x81};
uint8_t broadcastAddress2[] = {0x24, 0x6F, 0x28, 0xB5, 0x3C, 0xEC};

typedef struct struct_message {
  float x;
  float y;
  float z;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
//  Serial.print("\r\nLast Packet Send Status:\t");
//  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
int16_t tfDist = 0;
void setup() {
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
  // Init Serial Monitor
  Serial.begin(115200);
  //  Serial2.begin(115200);
  //  tfmP.begin(&Serial2);
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  memcpy(peerInfo.peer_addr, broadcastAddress2, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  // Set values to send
  //  if ( tfmP.getData( tfDist)) // Get data from the device.
  //  {
  //    Serial.println(tfDist);
  //    myData.x = tfDist;// display distance,
  //  }
  mpu6050.update();
  myData.x = mpu6050.getAngleX();
  myData.y = mpu6050.getAngleY();
  myData.z = mpu6050.getAngleZ();

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
  esp_err_t result2 = esp_now_send(broadcastAddress2, (uint8_t *) &myData, sizeof(myData));

//  if (result == ESP_OK) {
//    Serial.println("Sent with success in 1");
//  }
//  else {
//    Serial.println("Error sending the data");
//  }
//  if (result2 == ESP_OK) {
//    Serial.println("Sent with success in 2");
//  }
//  else {
//    Serial.println("Error sending the data");
//  }
delay(50);
}
