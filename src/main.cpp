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
//Nota: El "cuerpo de la vibora" ocupará 4 leds
void viborita(int tiempo){

  //Los valores para estos índices se eligieron de acuerdo a la fila dónde aparece la "vibora",
  //para este caso es la  fila 4
  //indiceA, se ocupara para apagar leds, indiceB se ocupará para prender leds
  int indiceA = 3;
  int indiceB = 3;
  int fila = 4;

  prenderNiveles();
  delay(tiempo);
  //Este for, inicia la entrada de la "vibora sobre el cubo"
  //prende los primeros 3 leds de la fila 4, es decir {3,7,11}
  for(int i = 0; i < 3; i++) {
    digitalWrite(anodos[indiceB], 1);
    delay(tiempo);
    indiceB += 4;
  }

  indiceB = 14;
  //Este for genera el movimiento de la "vibora"
  for(int i = 0; i < 3; i++){
    
    prenderFila(fila);
    delay(tiempo);
    for(int j = 0; j < 3; j++){
      //Vamos apagando los leds que están en la "cola" y prendiendo los que están en la "cabeza"
      digitalWrite(anodos[indiceA], 0);
      digitalWrite(anodos[indiceB], 1);
      delay(tiempo);

      //Debido a que el movimiento no es en un solo sentido, sino que puede ser a la derecha o izquierda
      //se uso 'i' para saber como modificar los indices
      if( i%2 == 0) {
        //Se podría decir que en este if la vibora se mueve a la iz
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
//Este patron genera un efecto de una vibora por cada nivel, se repetira 4 veces, una por cada nivel
void viboritaPorNivel(int tiempo){

  for (int i = 0; i < 4; i++)
  {
    int indiceA = 3;
    int indiceB = 3;
    int fila = 4;

    //Dependiendo de i es el nivel que prenderemos
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
      digitalWrite(anodos[indiceB], 1);
      delay(tiempo);
      indiceB += 4;
    }

    indiceB = 14;
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
      //Se decrementan la fila para mover la "vibora" a la siguiente fila
      fila--;
      //Se decrementan los indices para posicionarlos en filas posteriores
      indiceA--;
      indiceB--;
      
    }
    //Prendemos la primer fila del cubo
    prenderFila(1);
    delay(tiempo);
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
  
  //Igual que el patrón anterior estos indices se ocupan para apagar y prender leds
  //pero en este caso tanto indiceA o como indiceB sirven para prender leds
  //y anteriorA, anteriorB para apagarlos
  int indiceA = 5;
  int anteriorA = 9;
  int indiceB = 0;
  int anteriorB = 4;
  //Nota: indiceA sirve para controlar los cuatro leds interiores y indiceB para controlar los leds exteriores
  prenderNiveles();

  for(int i = 0; i < 4; i++ ) { 
    
    digitalWrite(anodos[anteriorA], 0 );
    digitalWrite(anodos[indiceA], 1);
    //Parece que sin el delay se ve más fluido el movimiento
    //delay(tiempo);
     for(int j = 0; j < 3; j++) {
        digitalWrite(anodos[anteriorB], 0 );
        digitalWrite(anodos[indiceB], 1);
        delay(tiempo);

        //Asignamos a anteriorB el índice actual
        anteriorB = indiceB;
        indiceB = moverIndiceLed(indiceB, i);
     }

     
      anteriorA = indiceA;
      indiceA = moverIndiceLed(indiceA, i);

  }

  //Función para apagar los leds, en caso de que algunos queden prendidos
  apagarAnodosCompletos();
}

//PATRÓN
//Función que genera la forma de un moño
void mono(int tiempo) {

  //indice: variable para prender leds
   int indice = 0;
  prenderNiveles();

  //Debido a que el efecto prende 12 leds en total, el primer ciclo tiene 4 iteraciones y el segundo 3
  for(int i = 0; i < 4; i++) {

    for(int j = 0; j < 3; j++) {
      
      digitalWrite(anodos[indice], 1);
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

  //Para este patrón se ocuparon muchos ciclos, pero al primero es de 16 iteraciones para recorrer
  //secuencialmente cada columna del cubo
  for(int i = 0; i < 16; i++) {
     
      digitalWrite(anodos[i], 1);

    //Este for va prediendo un nivel más en cada iteración, iniciando desde el primer nivel
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
    

    //Prendemos todas las columnas para posteriormente prender cada nivel, comenzando desde el nivel inferior
    //es decir, desde el cuarto nivel
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

    //Apagamos los leds(columnas) para que en la siguiente iteración no haya algún led prendido
    apagarAnodosCompletos();
  }
  apagarAnodosCompletos();

}

//PATRÓN
//Como el nombre del patrón lo dice, la función genera un efecto intercalado, es decir, prende una columna
//pero la siguiente no
//El efecto intercalado se presenta tanto en filas como columnas
void intercalado(int tiempo){
  
  //Prende la primer y tercer columna
  controlarAnodos(1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0);
  delay(tiempo);
  //Prende la segunda y terce columna
  controlarAnodos(0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1);
  delay(tiempo);
  //Prende la primer y tercer fila
  controlarAnodos(1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0);
  delay(tiempo);
  //Prende la segunda y cuarta fila
  controlarAnodos(0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1);
  delay(tiempo);
 
 apagarAnodosCompletos();
}

//PATRÓN
//Este patrón prende un led aleatoriamente, iniciando en el primer nivel y terminando en el último,
//generando un efecto de "caida"
void gravedad(int tiempo) {

  //Generamos un número aleatorio entre 0 y 15, de acuerdo a los indices del arreglo anodos[]
  int nodo = random(0,15);

  digitalWrite(anodos[nodo], 1);

  //Prendemos solo un nivel cada cierto 'tiempo'
  prenderPrimerNivel();
  delay(tiempo);

  prenderSegundoNivel();
  delay(tiempo);

  prenderTercerNivel();
  delay(tiempo);

  prenderCuartoNivel();
  delay(tiempo);
  digitalWrite(anodos[nodo], 0);

}

//PATRÓN
//Se llama expansión porque el movimiento se genera desde el centro y se va extendiendo a los laterales del cubo
void expansion(int tiempo) {

  //indice: lo ocuparemos para prender los leds, como será a partir del centro seleccionamos el 5
  int indiceA = 5;
  prenderNiveles();
  
  //Con este for prendemos los cuatro leds de adentro, usamos nuevamente la función moverIndiceLed()
  for(int i = 0; i < 4; i++) {
    
    digitalWrite(anodos[indiceA], 1);
    indiceA = moverIndiceLed(indiceA, i);
    delay(tiempo);
  }

  //Se asigna a 1, debido a que en el siguiente for intentamos generar una forma de cruz
  //esto se podrá apreciar más en el cubo
  indiceA = 1;
  for(int i = 0; i < 4; i++) {
    //Dado que se prenderan 8 leds, se ópto por usar  estas iteraciones
    for(int j = 0; j < 2; j++){
      digitalWrite(anodos[indiceA], 1);
      //Posicionamos indiceA en el siguiente led que queremos prender
      indiceA = moverIndiceLed(indiceA, i);
      delay(tiempo);
    }
    //Posicionamos en indice para la siguiente iteración y para esto sumamos '1' a i para cambiar el sentido
    indiceA = moverIndiceLed(indiceA, i+1);
    
  }
  //En este último for, prendemos los leds que se encutran en las esquinas
  indiceA = 0;
  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 3; j++) {
      digitalWrite(anodos[indiceA], 1);
      indiceA = moverIndiceLed(indiceA, i);
      delay(tiempo / 3);
    }
  }
  apagarAnodosCompletos();
}


void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Mensaje recibido bajo el tópico-> ");
  Serial.print(topic);
  Serial.print("\n");

  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }

  if( (char)payload[0] == '1'){
    viborita(500);
  }
  if( (char)payload[0] == '2'){
    viboritaPorNivel(500);
  }
  if( (char)payload[0] == '3'){
    radar(500);
  }
  if( (char)payload[0] == '4'){
    relampago(500);
  }
  if( (char)payload[0] == '5'){
    intercalado(500);
  }
  if( (char)payload[0] == '6'){
    expansion(500);
  }
  if( (char)payload[0] == '7'){
    gravedad(500);
  }
  if( (char)payload[0] == '8'){
    mono(500);
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

void loop() {

  if (client.connected() == false)
  {
    reconnect();
  }
  client.loop();
}

