#include <SPI.h>
#include <LoRa.h>
#include "SSD1306.h" // alias for `#include "SSD1306Wire.h"`

// GPIO5  -- SX1278's SCK
// GPIO19 -- SX1278's MISO
// GPIO27 -- SX1278's MOSI
// GPIO18 -- SX1278's CS
// GPIO14 -- SX1278's RESET
// GPIO26 -- SX1278's IRQ(Interrupt Request)

//OLED pins to ESP32 0.96OLEDGPIOs via this connecthin:
//OLED_SDA -- GPIO4
//OLED_SCL -- GPIO15
//OLED_RST -- GPIO16

#define SS      18
#define RST     14
#define DI0     26
#define BAND    433E6

SSD1306  display(0x3c, 4, 15);

void setup() {

  pinMode(16, OUTPUT);

  digitalWrite(16, LOW);    // set GPIO16 low to reset OLED

  delay(50);

  digitalWrite(16, HIGH); // while OLED is running, must set GPIO16 in high

  Serial.begin(115200);

  while (!Serial); //if just the the basic function, must connect to a computer

  delay(1000);

  Serial.println("LoRa Receiver");

  SPI.begin(5, 19, 27, 18);

  LoRa.setPins(SS, RST, DI0);

  if (!LoRa.begin(BAND)) {

    Serial.println("Starting LoRa failed!");

    while (1);
  }

  // Initialising the UI will init the display too.

  display.init();

  display.flipScreenVertically();

  display.setFont(ArialMT_Plain_16);
}

void loop() {

  // try to parse packet

  int packetSize = LoRa.parsePacket();

  if (packetSize) {

    // received a packet

    Serial.print("Received packet '");

    // read packet

    String receiveStr = "";

    while (LoRa.available()) {

      char myChar = (char)LoRa.read();

      receiveStr += myChar;
      
      Serial.print(myChar);
    }

    display.clear();

    display.setTextAlignment(TEXT_ALIGN_CENTER);
    
    display.drawString(64, 24, receiveStr);

    display.display();

    // print RSSI of packet

    Serial.print("' with RSSI ");

    Serial.println(LoRa.packetRssi());
  }
}
