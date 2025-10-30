int tr = 12;
int ec = 13;
float dur = 0;
float dis = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(tr, OUTPUT);
  pinMode(ec, INPUT);
}

void loop() {
  // kirim gelombang ultrasonic
  digitalWrite(tr, LOW);
  delayMicroseconds(20);
  digitalWrite(tr, HIGH);

  // terima durasi HIGH dan hitung jarak
  dur = pulseIn(ec, HIGH);
  dis = dur/58;

  // Tampilin ke serial monitor
  Serial.print("Distance: ");
  Serial.print(dis);
  Serial.println(" Cm");
  delay(500);
}
