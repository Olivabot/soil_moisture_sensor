#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED display width and height
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET     -1
#define SCREEN_ADDRESS 0x3C  // I2C address for OLED

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Soil moisture values
const int AirValue = 3000;   // Replace this with your actual air value
const int WaterValue = 1200; // Replace this with your actual water value
int intervals = (AirValue - WaterValue) / 3;
int soilMoistureValue = 0;

void setup() {
  Serial.begin(9600);  // Initialize Serial for debugging

  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);  // Don't proceed, loop forever if initialization fails
  }

  display.clearDisplay();  // Clear initial screen
  display.display();
}

void loop() {
  // Read the soil moisture value
  soilMoistureValue = analogRead(A0);

  // Determine the soil condition
  String moistureLevel;
  if (soilMoistureValue > WaterValue && soilMoistureValue < (WaterValue + intervals)) {
    moistureLevel = "Very Wet";
  } else if (soilMoistureValue > (WaterValue + intervals) && soilMoistureValue < (AirValue - intervals)) {
    moistureLevel = "Wet";
  } else if (soilMoistureValue < AirValue && soilMoistureValue > (AirValue - intervals)) {
    moistureLevel = "Dry";
  } else {
    moistureLevel = "Unknown";
  }

  // Print to Serial (for debugging)
  Serial.print("Soil Moisture Value: ");
  Serial.println(soilMoistureValue);
  Serial.println(moistureLevel);

  // Display on OLED
  display.clearDisplay();  // Clear the previous content on the display
  display.setTextSize(1);  // Set text size
  display.setTextColor(SSD1306_WHITE);  // Set text color

  // Display the soil moisture value
  display.setCursor(0, 0);  // Set cursor to top-left corner
  display.print("Soil Moisture Value: ");
  display.setCursor(0, 10);  // Move cursor down
  display.print(soilMoistureValue);

  // Display the moisture level
  display.setCursor(0, 30);  // Move cursor further down
  display.print("Moisture Level: ");
  display.setCursor(0, 40);
  display.print(moistureLevel);

  // Show everything on the display
  display.display();

  delay(1000);  // Delay before the next reading
}
