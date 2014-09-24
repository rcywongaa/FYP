int pin = 13;

void setup(){
  pinMode(pin, OUTPUT);
}

void loop(){
  for (int i=0; i<80; i++){
	  digitalWrite(pin, HIGH);
	  delay(100);
	  digitalWrite(pin, LOW);
	  delay(i);
  }
}
