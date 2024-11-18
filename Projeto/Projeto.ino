#include <TimerOne.h>

const int pino_led_status =  6;
const int pino_sensor_lixeira1 =  2;
const int pino_sensor_lixeira2 =  3;
const int pino_sensor_lixeira_aberta =  7;
String servidor_producao= "https://app.ecohide.com.br/";
String servidor_teste= "https://apptst.ecohide.com.br/";
String servidor_eco= "http://apptst.easychopp.com.br/ecohide/notification?status=teste";
String chamada_wakeup = "API/Wakeup?";
String chamada_status = "API/Notification?";
String data_status_lixeira1;
String data_status_lixeira2;
String data_status_lixeira_aberta;
String comando_at_ok="";
String IMEI="";
String IMSI="";
String data_hora="";
String local;
String rhttp;
String sinal_gsm;
String status_protocolo = "";
String response_protocolo = "";
String command_protocolo = "";
String smsnumber_protocolo = "";
int pooling_protocolo;
int potencia_sinal_gsm;
long int time3;



String enviarcomando(String comando,int linha, const int timeout)
{
    String conteudo = "";
    String resposta [100];
    char caractere;
    int i=0;
   
    Serial1.println(comando); 
    long int time = millis();
    while( (time+timeout) > millis())
    {
      while(Serial1.available())
      {   
        // Lê byte da serial
        caractere = Serial1.read();
        // Ignora caractere de retorno de linha
        if (caractere != '\n')
        {
          if (caractere != '\r')
          {
            // Concatena valores
            conteudo.concat(caractere);
          }
        }
        // aguarda quebra de linha e salva a linha atual no vetor
        if (caractere == '\n')
        {
          resposta[i] = conteudo;
          //Serial.println(resposta[i]);
          conteudo = "";
          i++;
        }
        // Aguarda buffer serial ler próximo caractere
        //delay(1);
      }  
    }
    if (linha == 99)
    {    
      resposta[99]=resposta[i-1];
    }
    return resposta[linha];
}


String coordenada()
{
  int  passo=0;
  String at_ok = "";
  String localizacao = "";
  long int time2 = millis();
  
  while((time2+120000) > millis()) //Tempo para realizar o fix do GPS
  {
    if (passo == 0)
    {
      at_ok = enviarcomando("AT+GPS=1",2,3000);  //Ativa a função GPS
      //Serial.println(passo);
      if (at_ok == "OK")
      {
        passo++;
        at_ok = "";      
      }
    }
    if (passo == 1)
    {
      at_ok = enviarcomando("AT+CGATT=1",4,10000);  //Habilita a rede GPRS.
      //Serial.println(passo);
      if (at_ok == "OK")
      {
        passo++;
        at_ok = "";      
      }
    }
/*    if (passo == 2)
    {
      at_ok = enviarcomando("AT+CGDCONT=1,\"IP\",\"cmnet\"",2,3000);  //Cria um perfil PDP. Primeiro parametro habilita, segundo parametro tipo de conexão e por ultimo a APN
      //Serial.println(passo);
      if (at_ok == "OK")
      {
        passo++;
        at_ok = "";      
      }
    }
    if (passo == 3)
    {
      at_ok = enviarcomando("AT+CGACT=1,1",2,3000);  //Ativa o perfil PDP 1
      //Serial.println(passo);
      if (at_ok == "OK")
      {
        passo++;
        at_ok = "";      
      }
    }
*/  if (passo == 2)
    {
      at_ok = enviarcomando("AT+LOCATION=2",4,3000);  //Solicitar localização e espera o OK do comando;
      //Serial.println(passo);
      if (at_ok == "OK")
      {
        passo++;
        at_ok = "";
        localizacao = "";
      }
    }
    if (passo == 3)
    {
      localizacao = enviarcomando("AT+LOCATION=2",2,3000);  //Captura as coordenadas de localização;
      //Serial.println(passo);
      if (localizacao != "")
      {
        passo++;
        at_ok = "";
        //Serial.println(passo);
      }
    }
    if (passo == 4)
    {
      at_ok = enviarcomando("AT+GPS=0",2,3000);  //Desativa a função GPS
      //Serial.println(passo);
      if (at_ok == "OK")
      {
        passo++;
        at_ok = "";
        break;      
      }
    }
  }
  if (localizacao == "")
  {
    localizacao = "F"; //caso não consiga a localização escreve F
  }
  return localizacao;
}  

void capturar_data_hora ()
{
    enviarcomando("at+ctzu=1",2,2000);
    data_hora = enviarcomando("AT+CCLK?",2,2000);  //Pega data e hora
    data_hora = data_hora.substring(8,25); // transforma (+CCLK: “00/01/01,00:04:04+00″) em (00/01/01,00:04:04)
    data_hora.replace(',',' '); // transforma (00/01/01,00:04:04) em (00/01/01 00:04:04)
    data_hora.replace('/','-'); // transforma (00/01/01 00:04:04) em (00-01-01 00:04:04)
    data_hora = "20" + data_hora; // transforma (00-01-01 00:04:04) em (2000-01-01 00:04:04)
}

void antena_gsm ()
{
          /*Value| RSSI(dBm)| Condition
            2    | -109     | Marginal
            3    | -107     | Marginal
            4    | -105     | Marginal
            5    | -103     | Marginal
            6    | -101     | Marginal
            7    | -99      | Marginal
            8    | -97      | Marginal
            9    | -95      | Marginal
            10   | -93      | OK
            11   | -91      | OK
            12   | -89      | OK
            13   | -87      | OK
            14   | -85      | OK
            15   | -83      | Good
            16   | -81      | Good
            17   | -79      | Good
            18   | -77      | Good
            19   | -75      | Good
            20   | -73      | Excellent
            21   | -71      | Excellent
            22   | -69      | Excellent
            23   | -67      | Excellent
            24   | -65      | Excellent
            25   | -63      | Excellent
            26   | -61      | Excellent
            27   | -59      | Excellent
            28   | -57      | Excellent
            29   | -55      | Excellent
            30   | -53      | Excellent*/


    sinal_gsm = enviarcomando("AT+CSQ",2,2000);  //Pega a potencia do sinal gsm
    sinal_gsm = sinal_gsm.substring(6,9);     //Transforma o retorno de +CSQ: 16,0 para 16
    potencia_sinal_gsm = sinal_gsm.toInt();   //Transforma o string de retorno para inteiro
    if (potencia_sinal_gsm > 1 && potencia_sinal_gsm < 10) //Qualifica a potência do sinal
    {
      sinal_gsm = "Ruim";
    }
    if (potencia_sinal_gsm > 9 && potencia_sinal_gsm < 15) //Qualifica a potência do sinal
    {
      sinal_gsm = "Ok";
    }
    if (potencia_sinal_gsm > 14 && potencia_sinal_gsm < 20) //Qualifica a potência do sinal
    {
      sinal_gsm = "Bom";
    }
    if (potencia_sinal_gsm > 19) //Qualifica a potência do sinal
    {
      sinal_gsm = "Excelente";
    }
    if (potencia_sinal_gsm < 2 || potencia_sinal_gsm > 30) //Qualifica a potência do sinal
    {
      sinal_gsm = "Falha";
    }
}

void pulsa_led_status()
{
  digitalWrite(pino_led_status, digitalRead(pino_led_status) ^ 1);  //muda o estado de tensão do LED de status
}

String ler_protocolo (String inStr,String needParam)
{
  if(inStr.indexOf("||" + needParam)>0)
  {
    int CountChar=needParam.length();
    int indexStart=inStr.indexOf("=");
    int indexStop= inStr.indexOf("||");  
    return inStr.substring(indexStart+CountChar+2, indexStop);
  }
  
}

void inicializar ()
{
  
  Timer1.initialize(500000);    //pulsa led a cada 0,5 segundos
  while (comando_at_ok != "OK") //Verifica se o A9 está respondendo;
  {
    comando_at_ok = enviarcomando("AT",2,2000);  //Verifica se o A9 está respondendo;
    Serial.println(comando_at_ok);
  }

  Timer1.initialize(1000000);    //pulsa led a cada 1 segundos
  while (IMEI == "") //Pega o IMEI do A9
  {
    IMEI = enviarcomando("AT+CGSN",2,2000);  //Pega o IMEI do A9
    Serial.println(IMEI);
  }

  Timer1.initialize(1500000);    //pulsa led a cada 1,5 segundos
  while (IMSI == "") //Pega o IMSI (International Mobile Subsciber Identity) do CHIP
  {
    IMSI = enviarcomando("AT+CIMI",2,2000);  //Pega o IMSI do A9
    Serial.println(IMSI);
  }

  Timer1.initialize(2000000);    //pulsa led a cada 2 segundos   
  while (data_hora == "") //Pega data e hora no formato 2000/01/01 00:04:04
  {
    capturar_data_hora ();      //Pega data e hora no formato 2000/01/01 00:04:04
    Serial.println(data_hora);
  }


  Timer1.initialize(2500000);    //pulsa led a cada 2,5 segundos
  while (sinal_gsm == "") //Pega a potencia do sinal gsm
  {
    antena_gsm();         //Pega a potencia do sinal gsm
    Serial.println(potencia_sinal_gsm);
    Serial.println(sinal_gsm);
  }

  Timer1.initialize(3000000);    //pulsa led a cada 3 segundos
  while (local == "") //Pega a localização pelo GPS
  {
    local = coordenada();     //Pega a localização pelo GPS
    Serial.println(local);
  }

  Timer1.initialize(3500000);    //pulsa led a cada 3,5 segundos  
  while (rhttp == "") //Envia protocolo de wakeup via http e coleta as respostas
  //exemplo resposta = ||Status=OK||Response=||Command=NOTF||Pooling=3600||SMSNumber=+5585991425169||
  
  {
  /*rhttp = enviarcomando("AT+HTTPGET=\"" + servidor_teste + chamada_wakeup + "||" + IMEI + "||\"",99,15000);  //Envia protocolo de wakeup via http
    status_protocolo = ler_protocolo(rhttp,"Status");  //armazena o retorno do metódo em variáveis
    response_protocolo = ler_protocolo(rhttp,"Response"); //armazena o retorno do metódo em variáveis
    command_protocolo = ler_protocolo(rhttp,"Command"); //armazena o retorno do metódo em variáveis
    pooling_protocolo = ler_protocolo(rhttp,"Pooling").toInt(); //armazena o retorno do metódo em variáveis
    smsnumber_protocolo = ler_protocolo(rhttp,"SMSNumber"); //armazena o retorno do metódo em variáveis
  */rhttp = enviarcomando("AT+HTTPGET=\"" + servidor_eco + "\"",99,15000);  //Envia protocolo de wakeup via http  
    Serial.println(rhttp);
  }
}

void data_lixeira1()
{
  capturar_data_hora ();
  data_status_lixeira1 = data_hora;
}

void data_lixeira2()
{
  capturar_data_hora ();
  data_status_lixeira2 = data_hora;
}

void data_lixeira_aberta()
{
  capturar_data_hora ();
  data_status_lixeira_aberta = data_hora;
}


void setup()
{
  Timer1.attachInterrupt(pulsa_led_status);  //Configura o timer 1 como interrupção
  Serial.begin(115200);                   //Inicializa a Serial
  Serial1.begin(115200);                  //Inicializa a Serial 1
  pinMode(pino_led_status, OUTPUT);     //Configura o borne para ser uma saída digital 
  pinMode(4, OUTPUT);                   //reset A9
  pinMode(5, OUTPUT);                   //Se 0V liga modo economia de energia A9
  pinMode(8, OUTPUT);                   //Botão liga A9 (pressionar 2s)
  digitalWrite(5, HIGH);                //Desaciona modo economia de energia A9
  digitalWrite(4, LOW);                 //Desaciona o botão reset A9
  digitalWrite(8, HIGH);                //Aciona o botão liga A9
  delay(3000);                          //Espera 3s com o botão liga acionado
  digitalWrite(8, LOW);                 //Desaciona o botão liga A9
  Serial.println("A7 Ligado!");         //Escreve "A7 Ligado!" na serial
  delay(30000);                         //Espera iniciar o SIM e o A9
  inicializar ();                       //Wakeup do dispositivo
  time3 = millis();                     //Seta contador para envio de notificação de status
  Timer1.initialize(10000000);           //pulsa led a cada 10 segundos
  attachInterrupt(pino_sensor_lixeira1,data_lixeira1,RISING);
  attachInterrupt(pino_sensor_lixeira2,data_lixeira2,RISING);
  attachInterrupt(pino_sensor_lixeira_aberta,data_lixeira_aberta,RISING);
}

void loop()
{
  if (pooling_protocolo == 0)
  {
    Timer1.initialize(500000);           //pulsa led a cada 0,5 segundos
  }
  if(((pooling_protocolo * 1000) + time3) < millis())
  {
  /*time3 = millis();
    rhttp = enviarcomando("AT+HTTPGET=\"" + servidor_eco + chamada_status + "||" + IMEI + "||" + digitalRead(pino_sensor_lixeira1) + "||" + data_status_lixeira1 + "||" + digitalRead(pino_sensor_lixeira2) + "||" + data_status_lixeira2 + "||" + digitalRead(pino_sensor_lixeira_aberta) + "||" + data_status_lixeira_aberta + "||" + "BATERIA" + "||" + local + "||" + "||\"",99,15000);  //Envia protocolo de wakeup via http
    status_protocolo = ler_protocolo(rhttp,"Status");  //armazena o retorno do metódo em variáveis
    response_protocolo = ler_protocolo(rhttp,"Response"); //armazena o retorno do metódo em variáveis
    command_protocolo = ler_protocolo(rhttp,"Command"); //armazena o retorno do metódo em variáveis
  */rhttp = enviarcomando("AT+HTTPGET=\"" + servidor_eco + "\"",99,15000);  //Envia protocolo de wakeup via http
    Serial.println(rhttp);
  }
}
