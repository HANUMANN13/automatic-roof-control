#define ena 5
#define in1 6
#define in2 7
#define r_lvl 2

unsigned long m_start = 0;
const unsigned long m_thresh = 2000;

bool m_Running = false;
bool r_Detected = false;
bool roof_Closed = false;  // Track the state of the roof

void setup() {
  Serial.begin(9600);
  pinMode(ena, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(r_lvl, INPUT);

  // Ensure the motor is stopped initially
  analogWrite(ena, LOW);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);

  // Read the initial state of the rain sensor
  int r = digitalRead(r_lvl);
  if (r == HIGH) {
    r_Detected = false;
    roof_Closed = false;
    Serial.println("Initial state: No Rain detected, keeping roof open.");
  } else {
    r_Detected = true;
    roof_Closed = true;
    Serial.println("Initial state: Rain detected, keeping roof closed.");
  }
}

void loop() {
  int r = digitalRead(r_lvl);
  Serial.print("Rain LVL: ");
  Serial.println(r);

  // When rain is detected and roof is open
  if (r == LOW && !r_Detected) {
    m_start = millis();
    m_Running = true;
    r_Detected = true;
    roof_Closed = true;
    Serial.println("Rain detected, closing the roof.");
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
  }

  // When no rain is detected and roof is closed
  if (r == HIGH && r_Detected) {
    m_start = millis();
    m_Running = true;
    r_Detected = false;
    roof_Closed = false;
    Serial.println("No Rain detected, opening the roof.");
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
  }

  // If the motor is running, keep it running for the threshold time
  if (m_Running && millis() - m_start < m_thresh) {
    analogWrite(ena, 255);
  } else if (m_Running) {
    analogWrite(ena, LOW);
    m_Running = false;
    if (roof_Closed) {
      Serial.println("Roof closed, waiting for rain to stop.");
    } else {
      Serial.println("Roof opened, waiting for rain to start.");
    }
  }

  delay(1000);
}
