#define DEBUG true
#include<stdio.h>
#include<string.h>

String executarln(String comando,int linha)
{
String conteudo = "";
String resposta[10];
char c;
int i = 0;
delay(1000);
Serial1.println(comando);
delay(1000);
while(Serial1.available()>0)
  {
    c = Serial1.read(); 
    if(c != '\r')
    {
      conteudo+=c;
    }
    if(c=='\n')
    {
      resposta[i] = conteudo;
      conteudo = "";
      i++;
    }
  }
Serial.print(resposta[linha]);
return(resposta[linha]);
delay(1000);
}

String executarsln(String comando1,String comando2,int linha)
{
String conteudo = "";
String resposta[20];
char c;
int i = 0;
delay(1000);
Serial1.print(comando1);
Serial1.println(comando2);

delay(3000);
while(Serial1.available()>0)
  {
    c = Serial1.read(); 
    if(c != '\r')
    {
      conteudo+=c;
    }
    if(c=='\n')
    {
      resposta[i] = conteudo;
      conteudo = "";
      i++;
    }
  }
  int x = 0;
  while(x<10)
  {
    Serial.print(resposta[x]);
    x++;
  }

return(resposta[linha]);
delay(1000);
}
void setup()
{
  String coordenada = "";
  Serial.begin(9600);
  Serial1.begin(9600);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(8,OUTPUT);
  digitalWrite(5, HIGH); 
  digitalWrite(4, LOW); 
  digitalWrite(8, LOW); 
  delay(1000);
  executarln("AT+GPS=1",2);
  executarln("AT+CGATT=1",4);
  executarln("AT+CGDCONT=1,\"IP\",\"cmnet\"",2);
  executarln("AT+CGACT=1,1",2);
  coordenada = executarln("AT+LOCATION=2",2);
  coordenada.remove(20,20);
  Serial1.print("AT+HTTPGET=\"http://apptst.easychopp.com.br/ecohide/notification?status=");
  Serial1.print(coordenada);
  Serial1.println("\"");
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
