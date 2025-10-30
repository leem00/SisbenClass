int sw = 13;  // button rotary encoder
int a = 12;   // ini pin CLK
int b = 14;   // ini pin DT

int aFirst = 0;   // ini kondisi awal a
int aSecond = 0;  // ini kondisi akhir a

int counter = 0;  // ini buat menampung counter

void setup() {
  // Setting awal
  Serial.begin(115200);

  pinMode(sw, INPUT);
  pinMode(a, INPUT);
  pinMode(b, INPUT);

  // Sebelum masuk kode, tentuin kondisi akhir a dulu
  aSecond = digitalRead(a);
}


void loop() {
  // selalu baca kondisi awal a
  aFirst = digitalRead(a);

  // kalau ternyata a ada perubahan
  if (aFirst != aSecond){

    // sekarang kalau b sama dengan a apa yang terjadi
    if (digitalRead(b) != aFirst){
      counter++;
    } else {
      counter--;
    }
  }

  // update kondisi akhir a
  aSecond = aFirst;

  // kalau sw di tekan, balik ke nol  
  if (digitalRead(sw) == 0){
    counter = 0;
  }

  Serial.println(counter);
}
