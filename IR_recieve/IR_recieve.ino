// ESP32 IR Finish Line Receiver with high-precision timing

const int IR1_RECEIVER_PIN = 16;  // GPIO pin connected to IR receiver 1
const int IR2_RECEIVER_PIN = 17;  // GPIO pin connected to IR reciever 2
const int INDICATOR_LED_PIN = 2; // Onboard LED for visual indication

// Variables to track finish line status
bool beam1Broken = false;
bool beam2Broken = false;
uint64_t startTime = 0;  // Start time in microseconds
uint64_t finishTime = 0;     // Finish time in microseconds
uint64_t speedTime = 0;    // Time used to calculate exit speed
double sensorGap = 6.5;  // Gap between sensors in centimeters

void setup() {
  Serial.begin(115200);
  Serial.println("ESP32 IR Finish Line Receiver Starting...");
  
  pinMode(IR1_RECEIVER_PIN, INPUT);
  pinMode(IR2_RECEIVER_PIN, INPUT);
  pinMode(INDICATOR_LED_PIN, OUTPUT);

  // Reset race timer on startup with microsecond precision
  startTime = esp_timer_get_time(); // High precision timer (microseconds)

  // Convert gap from centimeters to kilometers
  sensorGap = sensorGap / 100000;
  
  // Indicate system is ready
  digitalWrite(INDICATOR_LED_PIN, HIGH);
  delay(500);
  digitalWrite(INDICATOR_LED_PIN, LOW);
}

void loop() {
  // Read IR receivers state
  int ir1State = digitalRead(IR1_RECEIVER_PIN);
   int ir2State = digitalRead(IR2_RECEIVER_PIN);

  // Status update every 3 seconds (without flooding serial)
  static uint64_t lastStatusTime = 0;
  uint64_t currentTime = esp_timer_get_time();
  
  if (currentTime - lastStatusTime > 3000000) { // 3 seconds in microseconds
    //Serial.print("IR1 Signal State: ");
    //Serial.println(ir1State == LOW ? "Beam detected" : "No beam detected");
    //Serial.print("IR2 State: ");
    //Serial.println(ir2State == LOW ? "Beam detected" : "No beam detected");
    lastStatusTime = currentTime;
  }
  
  // Check for beam break (most IR receivers are active LOW)
  // When beam is broken, irState will be HIGH
  if (ir1State == HIGH && !beam1Broken && !beam2Broken) {
    // Beam just broken - finish line crossed
    beam1Broken = true;
    finishTime = esp_timer_get_time(); // Capture finish time with microsecond precision
    
    // Calculate elapsed time with microsecond precision
    uint64_t elapsedTimeMicros = finishTime - startTime;
    double seconds = elapsedTimeMicros / 1000000.0; // Convert to seconds
    
    // Visual indication
    digitalWrite(INDICATOR_LED_PIN, HIGH);
    
    
    // Print finish information with high precision
    Serial.println("===== FINISH LINE CROSSED! =====");
    Serial.print("Time: ");
    Serial.print(seconds, 6); // Display with 6 decimal places (microsecond precision)
    Serial.println(" seconds");
  }


  // Check for second beam break
  if (ir2State == HIGH && !beam2Broken && beam1Broken) {
  // Beam just broken - speed line crossed
  beam2Broken = true;
  speedTime = esp_timer_get_time(); // Capture speed time with microsecond precision

  double exitSpeed = sensorGap / (speedTime - finishTime) * 3600000000.0 ;

  // Print exit speed information
  Serial.println("Exit speed: ");
  Serial.print(exitSpeed);  // Display exit speed
  Serial.print(" km/h \n");
  }

  // Check if beam is restored after being broken
  if (beam1Broken && beam2Broken) {
    beam1Broken = false;
    beam2Broken = false;
    digitalWrite(INDICATOR_LED_PIN, LOW);
    Serial.println("Finish line clear - ready for next detection");
    resetRace();  // Reset times
  }
  
     // In case of error reset
  if(!beam1Broken && beam2Broken) {
    resetRace();
  }

  // Small delay to prevent CPU hogging
  delayMicroseconds(100); // Much smaller delay for higher precision
}


// Function to reset/start a new race
void resetRace() {
  speedTime = 0;
  startTime = esp_timer_get_time(); // Reset with microsecond precision
  Serial.println("Race timer reset - Ready for new race!");
  
  
  // Blink LED to indicate reset
  for (int i = 0; i < 3; i++) {
    digitalWrite(INDICATOR_LED_PIN, HIGH);
    delay(100);
    digitalWrite(INDICATOR_LED_PIN, LOW);
    delay(100);
  }
}