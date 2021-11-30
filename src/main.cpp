#include <Arduino.h>
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "c5862c";
const char* password = "280608396";

const char* mqtt_server = 	"driver.cloudmqtt.com";
const int mqtt_port = 18651;
const char *mqtt_user = "clxxwiek";
const char* mqtt_pass = "PtsiiKX9xhVm"; 

WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;
char msg[50];
int value = 0;

//Este arreglo se refiere a las columnas del cubo (están conectados por por la patita positiva o ánodo)
int anodos[] = {15,2,4,16,17,5,18,19,21,3,1,22,23,13,12,14}; 
//Este segundo arreglo hace referencia a los niveles de nuestro cubo que están conectados por medio del cátodo o la patita negativa
int catodos[] = {32,33,25,26}; 


void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Mensaje recibido bajo el tópico-> ");
  Serial.print(topic);
  Serial.print("\n");

  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }

  if( (char)payload[0] == '0'){
    viborita(500);
  }
  if( (char)payload[0] == '1'){
    radar(500);
  }

  Serial.println();
}


void reconnect()
{
  while (!client.connected())
  {
    Serial.println("Intentando Conexión MQTT");

    String clientId = "iot_1_";
    clientId = clientId + String(random(0xffff), HEX);

    if (client.connect(clientId.c_str(), mqtt_user, mqtt_pass))
    {
      Serial.println("Conexión a MQTT exitosa!!");
      client.publish("gmh", "primer mensaje");
      client.subscribe("entradaGMH");
    }
    else
    {
      Serial.println("Falló la conexión ");
      Serial.print(client.state());
      Serial.print(" Se intentará de nuevo en 5 segundos");
      delay(5000);
    }
  }
}


void setup_wifi()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(10);

  Serial.println("");
  Serial.println("");

  Serial.println("Conectado a -> ");
  Serial.println(ssid);

  //Me conecto a la red WiFi
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.println(".");
    delay(250);
  }

  Serial.println("");
  Serial.println("Conexión Exitosa!");

  Serial.println("Mi ip es -> ");
  Serial.println(WiFi.localIP());
}

void setup() {
    // put your setup code here, to run once:

  for (int i = 0; i < 16; i++) {
    pinMode(anodos[i], OUTPUT);
  }
  for (int i = 0; i < 4; i++) { 
    pinMode(catodos[i], OUTPUT);
  }

  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void controlarCatodos (int n1, int n2, int n3, int n4) {
   digitalWrite (catodos[0], n1);
   digitalWrite (catodos[1], n2);
   digitalWrite (catodos[2], n3);
   digitalWrite (catodos[3], n4);
} 



void controlarAnodos (int val1, int val2, int val3, int val4, int val5, int val6, int val7, int val8, int val9, int val10, int val11, int val12, int val13, int val14, int val15, int val16) {
 digitalWrite (anodos[0], val1);
 digitalWrite (anodos[1], val2);
 digitalWrite (anodos[2], val3);
 digitalWrite (anodos[3], val4);
 digitalWrite (anodos[4], val5);
 digitalWrite (anodos[5], val6);
 digitalWrite (anodos[6], val7);
 digitalWrite (anodos[7], val8);
 digitalWrite (anodos[8], val9);
 digitalWrite (anodos[9], val10);
 digitalWrite (anodos[10], val11);
 digitalWrite (anodos[11], val12);
 digitalWrite (anodos[12], val13);
 digitalWrite (anodos[13], val14);
 digitalWrite (anodos[14], val15);
 digitalWrite (anodos[15], val16);
}

void prenderPrimerNivel(){
  //En este caso como los catodos deben recibir tierra, se envia un 0 al primer nivel
  controlarCatodos(0,1,1,1);
}

void prenderSegundoNivel(){
  //En este caso como los catodos deben recibir tierra, se envia un 0 al segundo nivel
  controlarCatodos(1,0,1,1);
}

void prenderTercerNivel(){
  //En este caso como los catodos deben recibir tierra, se envia un 0 al tercer nivel
  controlarCatodos(1,1,0,1);
}

void prenderCuartoNivel(){
  //En este caso como los catodos deben recibir tierra, se envia un 0 al cuarto nivel
  controlarCatodos(1,1,1,0);
}
void prenderNiveles(){
  controlarCatodos(0,0,0,0);
}

void prenderNivelesExtremos(){
  controlarCatodos(0,1,1,0);
}
void prenderNivelesInternos(){
  controlarCatodos(1,0,0,1);
}

//Con esta función prendemos todos los anodos, es decir, todos las columnas
void prenderAnodosCompletos(){
  controlarAnodos(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1);
}

void apagarAnodosCompletos(){
  controlarAnodos(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
}


//Función para prender una fila en especifico, dependera de la vista en que se este mirando el cubo
//Pero la fila más cercana sería la primera
void prenderFila(int fila) {
  if( fila == 1 ) {
    controlarAnodos(1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0);
  }
  if( fila == 2 ) {
    controlarAnodos(0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0);
  }
  if( fila == 3 ) {
    controlarAnodos(0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0);
  }
  if( fila == 4 ) {
    controlarAnodos(0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1);
  }
}


//PATRÓN
//Este patron genera un efecto de una vibora que recorre el cubo sobre todos los niveles
void viborita(int tiempo){
  int indiceA = 3;
  int indiceB = 14;
  int fila = 4;

  prenderNiveles();
  delay(tiempo);
  //Este for, inicia la entrada de la "vibora sobre el cubo"
  for(int i = 0; i < 3; i++) {
    digitalWrite(anodos[indiceA], 1);
    delay(tiempo);
    indiceA += 4;
  }

  indiceA = 3;
  //Este for genera el movimiento de la "vibora"
  for(int i = 0; i < 3; i++){
    prenderFila(fila);
    delay(tiempo);
    for(int j = 0; j < 3; j++){
      digitalWrite(anodos[indiceA], 0);
      digitalWrite(anodos[indiceB], 1);
      delay(tiempo);
    
      if( i%2 == 0) {
        indiceA += 4;
        indiceB -= 4;
      }
      else {
        indiceA -= 4;
        indiceB += 4;
      }
 
    }
    fila--;
    indiceA--;
    indiceB--;
    
  }
  //Prendemos la primer fila del cubo
  prenderFila(1);

  //for para generar la salida de la "vibora" en el cubo
  for(int i = 0; i < 4; i++) {
    digitalWrite( anodos[indiceA] , 0 );
    delay(tiempo);
    indiceA -= 4;
  }

}

//PATRÓN
//Este patron genera un efecto de una vibora que recorre el cubo sobre todos los niveles
void viboritaPorNivel(int tiempo){

  for (int i = 0; i < 4; i++)
  {
    int indiceA = 3;
    int indiceB = 14;
    int fila = 4;

    if( i == 0 )
      prenderPrimerNivel();
    if( i == 1 )
      prenderSegundoNivel();
    if( i == 2 )
      prenderTercerNivel();
    if( i == 3)
      prenderCuartoNivel();
    delay(tiempo);
    //Este for, inicia la entrada de la "vibora sobre el cubo"
    for(int i = 0; i < 3; i++) {
      digitalWrite(anodos[indiceA], 1);
      delay(tiempo);
      indiceA += 4;
    }

    indiceA = 3;
    //Este for genera el movimiento de la "vibora"
    for(int i = 0; i < 3; i++){
      prenderFila(fila);
      delay(tiempo);
      for(int j = 0; j < 3; j++){
        digitalWrite(anodos[indiceA], 0);
        digitalWrite(anodos[indiceB], 1);
        delay(tiempo);
      
        if( i%2 == 0) {
          indiceA += 4;
          indiceB -= 4;
        }
        else {
          indiceA -= 4;
          indiceB += 4;
        }
  
      }
      fila--;
      indiceA--;
      indiceB--;
      
    }
    //Prendemos la primer fila del cubo
    prenderFila(1);

    //for para generar la salida de la "vibora" en el cubo
    for(int i = 0; i < 4; i++) {
      digitalWrite( anodos[indiceA] , 0 );
      delay(tiempo);
      indiceA -= 4;
    }

  }
  

  

}

//Funcion para modificar el indice de anodos[], dependiendo del sentido al que se quiera prender un led
//Se implemento porque ya se estaba repitiendo mucho
int moverIndiceLed(int indice, int i){
   if( i == 0)
     indice += 1;
   if( i == 1)
     indice += 4;
   if( i == 2)
     indice -= 1;
   if( i == 3)
     indice -= 4;
  return indice;
}

//PATRÓN
//Función que simula la forma de un radar
void radar(int tiempo) {
  
  
  int indiceA = 5;
  int anteriorA = 9;
  int indiceB = 0;
  int anteriorB = 4;
  prenderNiveles();

  for(int i = 0; i < 4; i++ ) { 
    
    digitalWrite(anodos[anteriorA], 0 );
    digitalWrite(anodos[indiceA], 1);
    delay(tiempo);
     for(int j = 0; j < 3; j++) {
        digitalWrite(anodos[anteriorB], 0 );
        digitalWrite(anodos[indiceB], 1);
        delay(tiempo);

        anteriorB = indiceB;
       indiceB = moverIndiceLed(indiceB, i);
     }

     
      anteriorA = indiceA;
      indiceA = moverIndiceLed(indiceA, i);

  }
  apagarAnodosCompletos();
}

//PATRÓN
//Función que genera la forma de un moño
void mono(int tiempo) {
   int indice = 0;
  int anterior = 4;

  prenderNiveles();
  for(int i = 0; i < 4; i++) {

    for(int j = 0; j < 3; j++) {
      
  
      digitalWrite(anodos[indice], 1);
      //digitalWrite(anodos[anterior], 0);
      delay(tiempo);
      

      if(i == 0 )
        indice += 1;
      if( i == 1 )
        indice += 3;
      if( i == 2 )
        indice += 1;
      if( i == 3)
        indice -= 5;
    }
    
  }
  apagarAnodosCompletos();
}


//PATRÓN
//Función que genera un efecto de relampago o eso intenté xd
void relampago(int tiempo) {
  int indice = 0;
  int anterior = 4;

  
  for(int i = 0; i < 16; i++) {
      prenderPrimerNivel();
      digitalWrite(anodos[i], 1);

    for(int j = 0; j < 4; j++) {
      if(j == 0)
        prenderPrimerNivel();
      if(j == 1)
        controlarCatodos(0,0,1,1);
      if(j == 2)
        controlarCatodos(0,0,0,1);
      if(j == 3)
        prenderNiveles();

      delay(tiempo);
    }
    
    anterior = i;
    
    prenderAnodosCompletos();
    for(int j = 0;  j < 4; j++){
      if( j == 0 )
        prenderCuartoNivel();
      if( j == 1 )
        prenderTercerNivel();
       if( j == 2 )
        prenderSegundoNivel();
      if( j == 3 )
        prenderPrimerNivel();
        delay(tiempo);
    }
    apagarAnodosCompletos();
  }
  apagarAnodosCompletos();

}

//PATRÓN
//
void intercalado(int tiempo){
  
  controlarAnodos(1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0);
  delay(tiempo);

  controlarAnodos(0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1);
  delay(tiempo);

  controlarAnodos(1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0);
  delay(tiempo);

  controlarAnodos(0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1);
  delay(tiempo);
 
 apagarAnodosCompletos();
}

void gravedad(int tiempo) {

  int nodo = random(0,15);

  prenderPrimerNivel();
  digitalWrite(anodos[nodo], 1);
  delay(tiempo);

  prenderSegundoNivel();
  delay(tiempo);

  prenderTercerNivel();
  delay(tiempo);

  prenderCuartoNivel();
  delay(tiempo);
  digitalWrite(anodos[nodo], 0);

}

void expansion(int tiempo) {
  int indiceA = 5;
  prenderNiveles();
  
  for(int i = 0; i < 4; i++) {
    
    digitalWrite(anodos[indiceA], 1);
    indiceA = moverIndiceLed(indiceA, i);
    delay(tiempo);
  }

  indiceA = 1;
  for(int i = 0; i < 4; i++) {
    
    for(int j = 0; j < 2; j++){
      digitalWrite(anodos[indiceA], 1);
      indiceA = moverIndiceLed(indiceA, i);
      delay(tiempo);
    }
indiceA = moverIndiceLed(indiceA, i+1);
    
  }
  indiceA = 0;
  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 3; j++) {
      digitalWrite(anodos[indiceA], 1);
      indiceA = moverIndiceLed(indiceA, i);
      delay(tiempo);
    }
  }
  apagarAnodosCompletos();
}
void loop() {

  // for (int i = 0; i < 4; i++) { 
  //   viborita(100);
  // }
  // for (int i = 0; i < 4; i++) { 
  //   gravedad(100);
  // }
  // for (int i = 0; i < 4; i++) { 
  //   viboritaPorNivel(100);
  // }
  // for (int i = 0; i < 4; i++) { 
  //   radar(100);
  // }
  // for (int i = 0; i < 4; i++) { 
  //   relampago(100);
  // }
  // for (int i = 0; i < 4; i++) { 
  //   mono(100);
  // }

  // for (int i = 0; i < 4; i++) { 
  //   intercalado(100);
  // }
  // for(int i = 0; i < 4; i++){
  //   expansion(100);
  // }

  if (client.connected() == false)
  {
    reconnect();
  }
  client.loop();
}

