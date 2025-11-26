//low = GND
//High = 5v
//Para que el rele este prendido el pin debe estar en high

//CTBot v2.1.14
//ArduinoJson 5.13.5

////////////////////////////////////////////// LIBRERIAS
#include <CTBot.h>
#include <CTBotDataStructures.h>
#include <CTBotDefines.h>
#include <CTBotInlineKeyboard.h>
#include <CTBotReplyKeyboard.h>
#include <CTBotSecureConnection.h>
#include <CTBotStatusPin.h>
#include <CTBotWifiSetup.h>
#include <Utilities.h>

#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

//////////////////////////////////////////////

template<class T> inline Print &operator <<(Print &obj, T arg) {
  obj.print(arg);
  return obj;
}

CTBot miBot;
CTBotInlineKeyboard miTeclado;

#include "token.h"

////////////////////////////////////////////// Pines de los Reles

uint8_t rUno = 19;
uint8_t rDos = 18;
uint8_t rTres = 17;
uint8_t rCuatro = 16;

////////////////////////////////////////////// Variables para detectar si estan prendidos o no

int releUno;
int releDos;
int releTres;
int releCuatro;

////////////////////////////////////////////// Conexion wifi

void setup() {

  Serial.begin(115200);
  Serial.println("Iniciando Bot Telegram...");

  miBot.wifiConnect(ssid, password);

  miBot.setTelegramToken(token);

  if (miBot.testConnection()) {
    Serial.println("Bot Conectado");
  }
  else {
    Serial.println("Error con el Bot");
  }

  ////////////////////////////////////////////// Estado de los pines de los reles (Low = reles apagados)

  pinMode(rUno, OUTPUT);
  digitalWrite(rUno, LOW);

  pinMode(rDos, OUTPUT);
  digitalWrite(rDos, LOW);

  pinMode(rTres, OUTPUT);
  digitalWrite(rTres, LOW);

  pinMode(rCuatro, OUTPUT);
  digitalWrite(rCuatro, LOW);

  //////////////////////////////////////////////

}

  ////////////////////////////////////////////// Teclado de botones

void teclado(int64_t chatId){

  CTBotInlineKeyboard t1;
  t1.addRow();
  t1.addButton("Rele 1", "1on", CTBotKeyboardButtonQuery);
  t1.addButton("Rele 2", "2on", CTBotKeyboardButtonQuery);
  t1.addButton("Rele 3", "3on", CTBotKeyboardButtonQuery);
  t1.addButton("Rele 4", "4on", CTBotKeyboardButtonQuery);
  miBot.sendMessage(chatId, "Que rele quiere prender?", t1);
}

//////////////////////////////////////////////

void loop() {

  TBMessage msg;

  ////////////////////////////////////////////// Detecta si estan prendidos o apagados y lo guarda en rUno,Dos...

  int estado;

  estado = digitalRead(rUno);
  if (estado == LOW) {
    releUno = 0;
  } else {
    releUno = 1;
  }

    estado = digitalRead(rDos);
  if (estado == LOW) {
    releDos = 0;
  } else {
    releDos = 1;
  }

    estado = digitalRead(rTres);
  if (estado == LOW) {
    releTres = 0;
  } else {
    releTres = 1;
  }

    estado = digitalRead(rCuatro);
  if (estado == LOW) {
    releCuatro = 0;
  } else {
    releCuatro = 1;
  }

  //////////////////////////////////////////////

  ////////////////////////////////////////////// Detecta si el usuario pide el teclado

  if (miBot.getNewMessage(msg)) {
    if (msg.messageType == CTBotMessageText) {
      if (msg.text.equalsIgnoreCase("rominabe")) {
        int64_t chatId = msg.sender.id;
        teclado(chatId);
      }
      else {
        miBot.sendMessage(msg.sender.id, "ingresar contraseña:");
      }
    } 
    ////////////////////////////////////////////// Detecta la interaccion del usuario y actua

    else if (msg.messageType == CTBotMessageQuery) {
      Serial << "Mensaje: " <<  msg.sender.firstName;

      ////////////////////////////////////////////// On y Off Rele 1

      if (msg.callbackQueryData.equals("1on")) { //El usuario eligio prender el rele 1
        if(releUno == 0){ // Si el rele esta apagado
          Serial.println(" Encender");
          digitalWrite(rUno, HIGH); // Lo prende
          miBot.endQuery(msg.callbackQueryID, "Rele Encendido", true);
        }

        else if(releUno == 1){ // Si ya estaba prendido
          Serial.println(" Apagar");
          digitalWrite(rUno, LOW); // Lo apaga
          miBot.endQuery(msg.callbackQueryID, "Rele apagado", true);
        }
      }

      ////////////////////////////////////////////// On y Off Rele 2

      if (msg.callbackQueryData.equals("2on")) {
        if(releDos == 0){
          Serial.println(" Endender");
          digitalWrite(rDos, HIGH);
          miBot.endQuery(msg.callbackQueryID, "Rele Encendido", true);
        }

        if(releDos == 1){
          Serial.println(" Apagar");
          digitalWrite(rDos, LOW);
          miBot.endQuery(msg.callbackQueryID, "Rele apagado", true);
        }
      }

      ////////////////////////////////////////////// On y Off Rele 3

      if (msg.callbackQueryData.equals("3on")) {
        if(releTres == 0){
          Serial.println(" Endender");
          digitalWrite(rTres, HIGH);
          miBot.endQuery(msg.callbackQueryID, "Rele Encendido", true);
        }

        else if(releTres == 1){
          Serial.println(" Apagar");
          digitalWrite(rTres, LOW);
          miBot.endQuery(msg.callbackQueryID, "Rele Apagado", true);
        }
      }

      ////////////////////////////////////////////// On y Off Rele 4

      if (msg.callbackQueryData.equals("4on")) {
        if(releCuatro == 0){
          Serial.println(" Endender");
          digitalWrite(rCuatro, HIGH);
          miBot.endQuery(msg.callbackQueryID, "Rele Encendido", true);
        }

        else if(releCuatro == 1){
          Serial.println(" Apagar");
          digitalWrite(rCuatro, LOW);
          miBot.endQuery(msg.callbackQueryID, "Rele Apagado", true);
        }
      }
    }
  }
  delay(250);
}