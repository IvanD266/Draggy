#include <Arduino.h>
#include <cc1101.h>

using namespace CC1101;

uint8_t data[4];

Radio radio(5, 4); // CS = 5, GDO0 = 4

void setup() {
  Serial.begin(9600);
  delay(1000);
  Serial.println("Starting ...");
  delay(3000);

  if (radio.begin() == STATUS_CHIP_NOT_FOUND) {
    Serial.println("Chip not found!");
    for (;;);
  }

  radio.setModulation(MOD_2FSK);
  radio.setFrequency(433.0);
  radio.setDataRate(10);
  radio.setOutputPower(10);

  radio.setPacketLengthMode(PKT_LEN_MODE_FIXED, sizeof(data));
  radio.setAddressFilteringMode(ADDR_FILTER_MODE_NONE);
  radio.setPreambleLength(4);
  radio.setSyncWord(0xD391);
  radio.setSyncMode(SYNC_MODE_16_16);
  radio.setCrc(false);
}

void loop() {
  Serial.println("Receiveing ...");
  
  radio.receive(data, sizeof(data));

    Serial.print("Received: ");
    for (int i = 0; i < 4; i++) {
      Serial.print("0x"); Serial.print(data[i], HEX); Serial.print(" ");
    }
    Serial.println();
  
  delay(1000);
}
