// Biblioteca Wi-Fi library
#include <ESP8266WiFi.h>

// Credenciais de acesso à rede
const char* ssid     = "AndroidAP";
const char* password = "ihtv0248";

// Número da porta (80) do servidor web
WiFiServer server(80);

void setup() {
  Serial.begin(115200);

  // Definir GPIO2
  pinMode(4, OUTPUT);
  digitalWrite(4, 0);
  
  // Conectar à rede WiFi
  Serial.print("A ligar a: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Ligado");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  
}

void loop() {
  // Verificar se o cliente se conectou
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  // Espera até que o cliente envie alguns dados
  Serial.println("Novo Cliente.");  
  while(!client.available()){
    delay(1);
  }
  
  // Lê a primeira linha do pedido
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  
  // Corresponde ao pedido
  int val;
  if (req.indexOf("/gpio/0") != -1)
    val = 0;
  else if (req.indexOf("/gpio/1") != -1)
    val = 1;
  else {
    Serial.println("invalid request");
    client.stop();
    return;
  }

  // Defini o GPIO2 de acordo com o pedido
  digitalWrite(4, val);
  
  client.flush();

  // Prepare a resposta
  //String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nLED  ";
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\nLED  ";
  s += (val)?"Ligado":"Desligado";
 // s += "</html>\n";

  // Envie a resposta para o cliente
  client.print(s);
  delay(1);
  Serial.println("Cliente desligado.");

  // O cliente será realmente desconectado 
  // quando a função retornar e o objeto 'cliente' for destruído
}
