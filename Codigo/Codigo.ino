int SensorLixo1 = 6;
int SensorLixo2 = 7;
int SensorStatus = 9;
int bateria = A0;
#include<stdio.h>
#include<string.h>
#define DEBUG true
char location;
char script;
char r=0;
String reg;
String sendData(String command, const int timeout, boolean debug)
{
    String response = "";    
    Serial1.println(command); 
    long int time = millis();
    while( (time+timeout) > millis())
    {
      while(Serial1.available())
      {       
        char c = Serial1.read(); 
        response+=c;
      }  
    }    
    if(debug)
    {
      Serial.print(response);
    }    
    return response;
}

void setup()
{
  Serial.begin(115200);  //BaudRate
  Serial1.begin(115200);  //BaudRate
  pinMode(4, OUTPUT);  //Habiltar como saída
  pinMode(5, OUTPUT);  //Habiltar como saída
  pinMode(8,OUTPUT);  //Habiltar como saída
  digitalWrite(5, HIGH); // A9 Acordado
  digitalWrite(4, LOW);  // A9 Ligado
  digitalWrite(8, LOW);  // A9 Ligado
  sendData("AT+CGMI",1000,DEBUG);  //Solicitar dados básicos
  sendData("AT+CGMM",1000,DEBUG);  //Solicitar dados básicos
  sendData("AT+CGMR",1000,DEBUG);  //Solicitar dados básicos
  sendData("Entrando em modo de baixo consumo",1000,DEBUG);
  digitalWrite(5, LOW); // A9 Dormindo
  digitalWrite(4, HIGH);  // A9 Desligado
  digitalWrite(8, HIGH);  // A9 Desligado
 }
void loop()
{
  if(digitalRead(SensorLixo1)==1) // Quando o sensor 1 for acionado
  {
    digitalWrite(5, HIGH); // A9 Acordado
    digitalWrite(4, LOW);  // A9 Ligado
    digitalWrite(8, LOW);  // A9 Ligado
    sendData("AT+CREG?",1000,DEBUG); //Consulta o status da rede
    reg = Serial1.read(); //Guardar status da rede
    if(reg == "+ CREG: 1,1")
    {
      sendData("AT+GPS=1",1000,DEBUG); //Ativar GPS
      sendData("AT+CGATT=1",1000,DEBUG); //Conecta-se a rede
      sendData("AT+CGDCONT=1,"IP","cmnet"",1000,DEBUG); //Define os parametros do PDP
      sendData("AT+CGACT=1,1",1000,DEBUG); //Ativar o PDP
      sendData("AT+LOCATION=2",1000,DEBUG); //Obter Localização
      location = Serial1.read(); //Guardar localização
      sendData("AT + HTTPGET = "http://wiki.ai-thinker.com/gprs_download""); //Conectar-se ao http
    }
    else
    {
    }
  }
