int speaker = 10;
volatile char currentchar='A';
volatile int bit;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void zero() {
  noTone(speaker);
  tone(speaker, 500);
  delay(1000);
}

void one() {
  noTone(speaker);
  tone(speaker, 330);
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  while(Serial.available()){
    currentchar=Serial.read();

    zero();
    for(int i=0; i<8; i++){
      bit=currentchar & 1;
      if(bit){
        one();
      }else{
        zero();
      }
  
      currentchar=currentchar>>1;
    }
    one();
  }
}
