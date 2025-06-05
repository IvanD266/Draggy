// ESP32 IR Transmitter with continuous 38kHz emission for finish line

const int IR1_LED_PIN = 4;       // GPIO pin connected to IR LED 1
const int IR2_LED_PIN = 16;       //GPIO pin connected to IR LED 2
const int VISIBLE_LED_PIN = 2;  // Onboard LED for visual confirmation
bool systemActive = true;       // Flag to control emission (could be toggled via button)

void setup() {
  Serial.begin(115200);
  Serial.println("ESP32 IR Finish Line Transmitter Starting...");
  
  pinMode(IR1_LED_PIN, OUTPUT);
  pinMode(IR2_LED_PIN, OUTPUT);
  pinMode(VISIBLE_LED_PIN, OUTPUT);
  
  // Turn on indicator LED to show system is active
  digitalWrite(VISIBLE_LED_PIN, HIGH);
  
  Serial.println("IR Finish Line active - continuously emitting 38kHz signal");
}

// Function to generate continuous 38kHz signal
void generate38kHz() {
  while (systemActive) {
    
    // 38kHz = period of ~26.3 microseconds (13.15μs high, 13.15μs low)
    digitalWrite(IR1_LED_PIN, HIGH);
    delayMicroseconds(13);
    digitalWrite(IR1_LED_PIN, LOW);
    delayMicroseconds(13);


    digitalWrite(IR2_LED_PIN, HIGH);
    delayMicroseconds(13);
    digitalWrite(IR2_LED_PIN, LOW);
    delayMicroseconds(13);
    
    // Every ~10,000 cycles, briefly check for serial data or other tasks
    // This prevents the system from being locked in this tight loop
    static unsigned long cycleCount = 0;
    if (++cycleCount >= 10000) {
      cycleCount = 0;
      // This tiny delay allows the ESP32 to handle other tasks
      delayMicroseconds(1);
    }
  }
}

void loop() {
  
  // Keep generating 38kHz signal continuously
  generate38kHz();
  
  

}