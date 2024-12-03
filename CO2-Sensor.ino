int sensorIn = A0;

void setup() {
  Serial.begin(9600);
  analogReference(DEFAULT);

}

void loop() {
  int sensorValue = analogRead(sensorIn);
  float tegangan = sensorValue * (5000 / 1024.0);
  if (tegangan == 0) {
    Serial.println("vault");
  }
  else if (tegangan < 400) {
    Serial.println("preheating");
  }
  else {
    int voltage_difference = tegangan - 400;
    float concentration = voltage_difference * 50.0 / 16.0;
    String voltageStatus = "Voltage =" + String(tegangan);
    String concentrationStatus = "CO2 concentration =" + String(concentration);
    Serial.println(voltageStatus);
    Serial.println(concentrationStatus);
  }

}
