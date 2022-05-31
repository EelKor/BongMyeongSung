const int pinSwitch = 2;
int ignition = 1;

void setup() {
  // put your setup code here, to run once:
  pinMode(pinSwitch,INPUT_PULLUP);
  pinMode(LED_BUILTIN,OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  ignition = digitalRead(pinSwitch);
  if(ignition == HIGH)
  {
    digitalWrite(13,HIGH);
  }

  else
  {
    digitalWrite(13, LOW);
  }
  Serial.println(ignition);
  delay(1);
} 
