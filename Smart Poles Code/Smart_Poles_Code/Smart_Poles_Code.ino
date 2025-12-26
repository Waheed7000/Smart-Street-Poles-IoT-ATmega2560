#include "DFRobot_AirQualitySensor.h"
DFRobot_AirQualitySensor pmSensor(&Wire, 0x19);


// ================= PIN MAP =================
const int LDR_PIN    = 22;
const int PIR_PIN    = 23;
const int SOUND_PIN  = A0;
const int LM35_PIN   = A1;
const int MQ2_PIN    = A2;
const int HIH_PIN    = A3;
const int FLAME_PIN  = 24;

const int LED_PWM    = 45;
const int S_MOSFET   = 27;
const int L_MOSFET   = 26;
const int BUZER_PIN  = 25;

// ================= TIMERS =================
unsigned long lastSoundTime = 0;
unsigned long lastSendTime  = 0;


const unsigned long lampHoldTime = 700;
const unsigned long sendInterval  = 400;   // 1 second

// ================ OBJECTS ==================

// Cached DHT values
float humidity = 0;
float temperature = 0;

void setup() {
  pinMode(LDR_PIN, INPUT);
  pinMode(PIR_PIN, INPUT);
  pinMode(SOUND_PIN, INPUT);
  pinMode(FLAME_PIN, INPUT);
  pinMode(MQ2_PIN, INPUT);

  pinMode(LED_PWM, OUTPUT);
  pinMode(S_MOSFET, OUTPUT);
  pinMode(L_MOSFET, OUTPUT);
  pinMode(BUZER_PIN, OUTPUT);

  Serial1.begin(9600);
  uint8_t attempts = 0;
  while (!pmSensor.begin()) {
    attempts++;
    if (attempts >= 3) break;   // وقف في simulation, مفيش HALT
    delay(500);
  }


  analogWrite(LED_PWM, 0);
  digitalWrite(S_MOSFET, 0);
  digitalWrite(L_MOSFET, 0);
  digitalWrite(BUZER_PIN, 0);
}

void loop() {

  // ====== SENSOR READINGS ======
  int isDay  = digitalRead(LDR_PIN);
  int flame  = digitalRead(FLAME_PIN);
  int gasVal = analogRead(MQ2_PIN) - 256;
  int sound  = analogRead(SOUND_PIN);
  int tempe  = ((analogRead(LM35_PIN) * 5000.0) / 1023.0) / 10.0;
  int humdty = ((analogRead(HIH_PIN)/1024.0)-0.1515)/0.00636 + 1;

  // ====== LIGHTING CONTROL ======
  if (isDay == HIGH) {
    digitalWrite(S_MOSFET, HIGH);

    int motion = digitalRead(PIR_PIN);

    if (motion == HIGH) {
      analogWrite(LED_PWM, 255);

      if (sound >= 50) {
        digitalWrite(L_MOSFET, HIGH);
        lastSoundTime = millis();
      }
    } else {
      analogWrite(LED_PWM, 100);
      digitalWrite(L_MOSFET, LOW);
    }

    if (millis() - lastSoundTime >= lampHoldTime) {
      digitalWrite(L_MOSFET, LOW);
    }
  }
  else {
    digitalWrite(S_MOSFET, LOW);
    digitalWrite(L_MOSFET, LOW);
    analogWrite(LED_PWM, 0);
  }

  // ====== FIRE ALARM ======
  digitalWrite(BUZER_PIN, (flame == LOW) ? HIGH : LOW);

  uint16_t pm1_0_std = (pmSensor.gainParticleConcentration_ugm3(PARTICLE_PM1_0_ATMOSPHERE));
  uint16_t pm2_5_std = (pmSensor.gainParticleConcentration_ugm3(PARTICLE_PM2_5_ATMOSPHERE));
  uint16_t pm10_std  = (pmSensor.gainParticleConcentration_ugm3(PARTICLE_PM10_ATMOSPHERE));

  // ====== SEND TO UART EVERY 1 SEC ======
  if (millis() - lastSendTime >= sendInterval) {
    Serial1.println();
    Serial1.println("======= SMART POLE NODE DATA =======");

    // Flame
    Serial1.print("Flame: ");
    Serial1.println(!flame ? "FIRE DETECTED!" : "Safe");

    // Gas
    Serial1.print("Gas: ");
    Serial1.println(gasVal > 100 ? "GAS LEAK!" : "Safe");

    // Sound
    Serial1.print("Sound: ");
    Serial1.println(sound >= 80 ? "WEIRD NOISE!" : "Normal");

    // Temperature
    Serial1.print("Temp: ");
    Serial1.print(tempe);
    Serial1.println(" C");

    // Humidity
    Serial1.print("Humidity: ");
    Serial1.print(humdty);
    Serial1.println("%");

    // -------- PM1.0 --------
    Serial1.print("PM1.0: ");
    Serial1.print(pm1_0_std);
    Serial1.print(" ug/m3 - ");
    Serial1.println(
      pm1_0_std <= 12  ? "Safe" :
      pm1_0_std <= 50  ? "Moderate" :
      pm1_0_std <= 100 ? "Unhealthy" :
                        "Hazardous!"
    );

    // -------- PM2.5 --------
    Serial1.print("PM2.5: ");
    Serial1.print(pm2_5_std);
    Serial1.print(" ug/m3 - ");
    Serial1.println(
      pm2_5_std <= 12    ? "Safe" :
      pm2_5_std <= 35.4  ? "Moderate" :
      pm2_5_std <= 55.4  ? "Unhealthy" :
      pm2_5_std <= 150.4 ? "Unhealthy" :
                          "Hazardous!"
    );

    // -------- PM10 --------
    Serial1.print("PM10: ");
    Serial1.print(pm10_std);
    Serial1.print(" ug/m3 - ");
    Serial1.println(
      pm10_std <= 54   ? "Safe" :
      pm10_std <= 154  ? "Moderate" :
      pm10_std <= 254  ? "Unhealthy" :
      pm10_std <= 354  ? "Very Unhealthy" :
                        "Hazardous!"
    );

    Serial1.println("====================================");
    lastSendTime = millis();
  }
}
