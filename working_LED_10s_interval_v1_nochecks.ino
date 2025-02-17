#define ANALOG_MAX 1023         // ESP8266 ADC is 10-bit (0–1023)
#define SENSOR_PIN A0           // LED sensor on A0
#define INDICATOR_PIN D2        // Secondary (indicator) LED

const unsigned long BASELINE_INTERVAL = 10000; // 10 seconds for baseline update
const unsigned long PRINT_INTERVAL    = 500;   // Print debug info every 500ms
const int NUM_SAMPLES                 = 5;     // Number of readings to average
const float THRESHOLD                 = 0.20;  // 20% drop triggers indicator

unsigned long lastBaselineUpdate = 0;
unsigned long lastPrintTime      = 0;
int baseline = 0;

void setup() {
  pinMode(INDICATOR_PIN, OUTPUT);
  digitalWrite(INDICATOR_PIN, LOW);
  
  Serial.begin(115200);
  Serial.println("\nStarting LED Light Sensor...");
  delay(1000);
  
  // Test indicator LED
  Serial.println("Testing indicator LED...");
  digitalWrite(INDICATOR_PIN, HIGH);
  delay(1000);
  digitalWrite(INDICATOR_PIN, LOW);
  Serial.println("Indicator LED test complete.");
  
  // Calibrate initial baseline using the maximum of several readings
  Serial.println("Calibrating baseline...");
  int maxReading = 0;
  for (int i = 0; i < 20; i++) {
    int reading = analogRead(SENSOR_PIN);
    if (reading > maxReading) {
      maxReading = reading;
    }
    Serial.print("Calibration reading ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(reading);
    delay(100);
  }
  baseline = maxReading;
  Serial.print("Initial baseline set to: ");
  Serial.println(baseline);
  
  lastBaselineUpdate = millis();
  lastPrintTime = millis();
}

void loop() {
  unsigned long currentMillis = millis();
  
  // Average multiple sensor readings
  long sum = 0;
  for (int i = 0; i < NUM_SAMPLES; i++) {
    sum += analogRead(SENSOR_PIN);
    delay(2);
  }
  int avgValue = sum / NUM_SAMPLES;
  float percentDrop = (baseline - avgValue) * 1.0 / baseline;
  
  // If the sensor reading is at least 20% below the baseline, turn the LED on
  if (percentDrop >= THRESHOLD) {
    digitalWrite(INDICATOR_PIN, HIGH);
  } else {
    digitalWrite(INDICATOR_PIN, LOW);
  }
  
  // Recalibrate the baseline every 10 seconds with the most recent average
  if (currentMillis - lastBaselineUpdate >= BASELINE_INTERVAL) {
    baseline = avgValue;
    lastBaselineUpdate = currentMillis;
    Serial.print("Updated baseline: ");
    Serial.println(baseline);
  }
  
  // Print status debug info every PRINT_INTERVAL milliseconds
  if (currentMillis - lastPrintTime >= PRINT_INTERVAL) {
    Serial.println("\n--- Status Update ---");
    Serial.print("Avg Reading: ");
    Serial.print(avgValue);
    Serial.print(" | Baseline: ");
    Serial.print(baseline);
    Serial.print(" | Percent Drop: ");
    Serial.print(percentDrop * 100);
    Serial.println("%");
    lastPrintTime = currentMillis;
  }
}
