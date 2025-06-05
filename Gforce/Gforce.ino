#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <cc1101.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Library for ST7789 driver

// Define the pins you're using
#define TFT_CS        15    // Chip select control pin
#define TFT_RST       4    // Reset pin
#define TFT_DC        2    // Data/Command control pin
#define TFT_MOSI     23    // MOSI pin (SDA)
#define TFT_SCLK     18    // Clock pin (SCL)

Adafruit_MPU6050 mpu;

// Create the display object
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

//	Variables
float accelX, accelY, accelZ, gForce;
int prevX = -1, prevY = -1;

void setup(void) {
	Serial.begin(115200);

	// Try to initialize!
	if (!mpu.begin()) {
		Serial.println("Failed to find MPU6050 chip");
		while (1) {
		  delay(10);
		}
	}
	Serial.println("MPU6050 Found!");

	// set accelerometer range to +-2G
	mpu.setAccelerometerRange(MPU6050_RANGE_2_G);

	// set gyro range to +- 250 deg/s
	mpu.setGyroRange(MPU6050_RANGE_250_DEG);

	// set filter bandwidth to 21 Hz
	mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

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
	
	// Label for 0.5G ring
	tft.setCursor(160 + 55, 120);  // slightly outside the circle
	tft.setTextColor(ST77XX_WHITE);
	tft.setTextSize(1);
	tft.print("0.5G");

	// Label for 1.0G ring
	tft.setCursor(160 + 105, 120);
	tft.print("1G");	

	delay(100);
}

// Function to map G-force (-2G to +2G) to screen coordinates
int mapG(float g, int center, float gRange, int maxRadius) {
  // Map from -gRange..+gRange to center-maxRadius..center+maxRadius
  return center + (int)(g * (maxRadius / gRange));
}



void loop() {
	/* Get new sensor events with the readings */
	sensors_event_t a, g, temp;
	mpu.getEvent(&a, &g, &temp);
  
	// Convert to G
  accelX = a.acceleration.x / 9.81;
  accelY = a.acceleration.y / 9.81;
  gForce = sqrt(accelX * accelX + accelY * accelY);

	 // Map G-force to screen position
	int dotX = mapG(-accelX, 160, 1.0, 100); // Invert X, center at 160, ±1.0G = 100px
	int dotY = mapG(accelY, 120, 1.0, 100); // Invert Y, center at 120, ±1.0G = 100px


	// Erase previous dot
  if (prevX >= 0 && prevY >= 0) {
    tft.fillCircle(prevX, prevY, 5, ST77XX_BLUE);
  }

	 // Draw new dot
  tft.fillCircle(dotX, dotY, 5, ST77XX_RED);
  prevX = dotX;
  prevY = dotY;

 // Debug output
  Serial.print("accelX: "); Serial.print(accelX);
  Serial.print(" accelY: "); Serial.print(accelY);
  Serial.print(" gForce: "); Serial.println(gForce);

	delay(10);
}