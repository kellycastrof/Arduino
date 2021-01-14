#include <Keypad.h>
#include  <Wire.h>
#include  <LiquidCrystal_I2C.h>

short led1=26, led2=32, led3=30;
short btn1 = 38, btn2 = 36, btn3 = 13, btnMain=9;
char pulsacion;

boolean stateBtn1=true, stateBtn2=true, stateBtn3=true, acum=true, fin=false;
short jugadores=0, jugador=-1;
int puntajes[3]={0};
const byte Filas = 4;          //KeyPad de 4 filas
const byte Cols = 4;           //y 4 columnas
byte Pins_Filas[] = {7, 6, 5, 4};     //Pines Arduino para las filas.
byte Pins_Cols[] = { 3, 2, 1, 0};
char Teclas [ Filas ][ Cols ] =
    {
        {'1','2','3','A'},
        {'4','5','6','B'},
        {'7','8','9','C'},
        {'*','0','#','D'}
     };
void initArray(){
  for(int p=0; p<3; p++){
    puntajes[p]=0;
  }
}
     
Keypad Teclado1 = Keypad(makeKeymap(Teclas), Pins_Filas, Pins_Cols, Filas, Cols);

LiquidCrystal_I2C lcd(0x27,20,4);

void aumentarPuntaje(int jugador, int puntaje){
  puntajes[jugador]=puntajes[jugador]+puntaje;
}

String msgFinalJugador(int ganador){
  String s= "Ganador Jug "+String(ganador);
  return s;
}
String msgFinalPuntaje(int puntaje){
  String s= String(puntaje) + " puntos";
  return s;
}

void msgFinal(){
  int maxP=getMax();
  String s1=msgFinalPuntaje(maxP);
  int ganador=getIndexMax(maxP);
  String s2=msgFinalJugador(ganador +1);
  lcd.setCursor(1, 0);
  if(maxP==0){
    lcd.print(" No hay ");
    lcd.setCursor(1,1);
    lcd.print(" ganador ");
  }
  else{
    lcd.print(s2);
    lcd.setCursor(1, 1);
    lcd.print(s1);  
  }
  
  
}

void mostrarPuntaje(int jugador, int puntaje){
  lcd.clear();
  lcd.setCursor(1, 0);
  String s2= "Jugador "+String(jugador+1);
  lcd.print(s2);
  lcd.setCursor(1, 1);
  String s1= "Puntaje "+String(puntaje);
  lcd.print(s1);
}

int leerTeclado(char pulsacion){
   if(pulsacion=='1'){
      return 1;
   }
    return 0;
}

boolean finalizar(){
  if(pulsacion!=0 && pulsacion=='*'){
    return true;
  }
  return false;
}

int getMax(){
  int maxP=puntajes[0];
  for(int j=1;j<3;j++){
    if(puntajes[j]>maxP){
      maxP=puntajes[j];
    }
  }
  return maxP;
}

int getIndexMax(int maxP){
  for(int j=0; j<3; j++){
    if(puntajes[j]==maxP){
      return j;
    }
  }

  return -1;
}

void again(){
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  stateBtn1=true; acum=true;
  stateBtn2=true;
  stateBtn3=true;
  jugadores=0; jugador=-1;
}

void msgInicial(){
  
  lcd.setCursor(1, 0);
  lcd.print("Master Wings      ");
  lcd.setCursor(4, 1);
  lcd.print("Asked!           ");  
}

void setup() {
  pinMode(btn1, INPUT);
  pinMode(btn2, INPUT);
  pinMode(btn3, INPUT);
  
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
   // Inicializar el LCD
  lcd.init();
  
  //Encender la luz de fondo.
  lcd.backlight();
  
  // Escribimos el Mensaje inicial.
  msgInicial();
  
  Serial.begin(9600);
}

void loop() {
  
  pulsacion = Teclado1.getKey() ;
  if(digitalRead(btn1) == 0 && stateBtn1){
    jugadores= led1;
    stateBtn2=false;
    stateBtn3=false;
    jugador=0;
  }

  if(digitalRead(btn2) == 0 && stateBtn2){
    jugadores = led2; 
    stateBtn1=false;
    stateBtn3=false;
    jugador=1;
     
  }
  
  if(digitalRead(btn3) == 0 && stateBtn3){
    jugadores = led3; 
    stateBtn1=false;
    stateBtn2=false;
    jugador=2;
  }

  if(jugadores!=0){
    digitalWrite(jugadores, HIGH);
  }

  if(jugador!=-1 && pulsacion!=0 && acum){
    int valor= leerTeclado(pulsacion);
    Serial.println(valor);
    aumentarPuntaje(jugador, valor)  ;  
    acum=false;
    mostrarPuntaje(jugador, puntajes[jugador]);
  }
  
  if(digitalRead(btnMain) == 0){
    again();
    lcd.clear();
    msgInicial();
    
  }

  if(finalizar()){
    lcd.clear();
      msgFinal();

      initArray();
  }
  if(pulsacion=='5'){
    lcd.noBacklight();
    initArray(); 
  }

  if(pulsacion=='8'){
    lcd.clear();
    lcd.backlight();
    msgInicial();
    
  }

  
}
