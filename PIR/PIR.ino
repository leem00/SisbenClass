int pir = 18;
int status;

void setup() {
  Serial.begin(115200);
  pinMode(pir, INPUT);
}

void loop() {
  status = digitalRead(pir);

  if (status == 0){
    Serial.println("Tidak ada gerakan");
  } else{
    Serial.println("Gerakan terdeteksi");
  }
  
  delay(200);
}