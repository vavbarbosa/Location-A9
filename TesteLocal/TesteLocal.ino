#include<stdio.h>
#include<string.h>
void setup()
{
  Serial.begin(115200);
  Serial1.begin(115200);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(8,OUTPUT);
  digitalWrite(5, HIGH); 
  digitalWrite(4, LOW); 
  digitalWrite(8, LOW); 
  delay(5000);'
  Serial1.println("AT+CREG?");
}

void loop()
{
  if (Serial1.available()>0) {
    Serial.write(Serial1.read());
  }
  if (Serial.available()>0) {
    Serial1.write(Serial.read());
  }
}
