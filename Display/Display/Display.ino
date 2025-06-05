#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Library for ST7789 driver

// Define the pins you're using
#define TFT_CS        15    // Chip select control pin
#define TFT_RST       4    // Reset pin
#define TFT_DC        2    // Data/Command control pin
#define TFT_MOSI     23    // MOSI pin (SDA)
#define TFT_SCLK     18    // Clock pin (SCL)

// Create the display object
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

void setup() {
  // Initialize the display with correct dimensions
  tft.init(240, 320);               // Use the correct resolution
  tft.setRotation(3);               // Set rotation (adjust as needed)
  tft.fillScreen(ST77XX_BLUE);     // Clear screen to black

  // Set text settings
  tft.setTextSize(2);                         // Set text size
  tft.setTextColor(ST77XX_WHITE);            // Set text color
  tft.setCursor(10, 10);                      // Set cursor position
  tft.drawCircle(160, 120, 50, ST77XX_WHITE);  // Draw white outline around the circle
  tft.drawCircle(160, 120, 100, ST77XX_WHITE);  // Draw white outline around the circle

}

void loop() {
  // Your main code here
}
