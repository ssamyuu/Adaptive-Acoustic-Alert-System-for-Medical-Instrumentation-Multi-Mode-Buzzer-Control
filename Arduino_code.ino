const int buzzerPin = 9;    // PWM-capable pin
String mode = "OFF";

void setup() {
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("===>  Multi-Mode Buzzer Ready - Modes: ALERT, SIREN, POT, FREQ, OFF");
}

void loop() {
  if (Serial.available()) {
    mode = Serial.readStringUntil('\n');
    mode.trim();
    mode.toUpperCase();
    Serial.print(">> Mode set to: ");
    Serial.println(mode);
  }

  if (mode == "ALERT") {
    playAlertPattern();
  } else if (mode == "SIREN") {
    playSiren();
  } else if (mode == "POT") {
    playPotTone();
  } else if (mode == "FREQ") {
    variableFrequency();
  } else {
    noTone(buzzerPin);
  }
}

// 🟢 ALERT: Dynamic beep length varying over time (no pot needed)
void playAlertPattern() {
  static int onTime = 100;
  static int offTime = 100;
  static int step = 10;
  static bool increasing = true;

  beepPattern(onTime, offTime);

  if (increasing) {
    onTime += step;
    offTime += step;
    if (onTime >= 600) {
      increasing = false;
    }
  } else {
    onTime -= step;
    offTime -= step;
    if (onTime <= 100) {
      increasing = true;
    }
  }
}

void beepPattern(int onTime, int offTime) {
  tone(buzzerPin, 2000);
  delay(onTime);
  noTone(buzzerPin);
  delay(offTime);
}

// 🔵 SIREN: Sweep frequency up/down
void playSiren() {
  for (int i = 1000; i <= 3000; i += 50) {
    tone(buzzerPin, i);
    delay(5);
  }
  for (int i = 3000; i >= 1000; i -= 50) {
    tone(buzzerPin, i);
    delay(5);
  }
}

// 🟡 POT: Smoothly vary frequency tone up/down to differentiate
void playPotTone() {
  static int freq = 500;
  static int step = 20;
  static bool increasing = true;

  tone(buzzerPin, freq);
  delay(100);

  if (increasing) {
    freq += step;
    if (freq >= 1500) increasing = false;
  } else {
    freq -= step;
    if (freq <= 500) increasing = true;
  }
}

// 🔴 FREQ: Play fixed frequency tone (simulate frequency mode)
void variableFrequency() {
  tone(buzzerPin, 1500);
  delay(200);
  noTone(buzzerPin);
  delay(200);
}