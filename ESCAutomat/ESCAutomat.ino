#include <Servo.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPClient.h>

//Hier ESC-WLAN-Zugangsdaten eingeben
const char* ssid     = "hamaadapter";
const char* password = "12345678";

Servo servoblau;
Servo servorot;
ESP8266WebServer server ( 80 );
bool errorModeEnabled;
bool karteLiegtAuf;
int ledG = 4; //war vorher 12
int ledR = 5; //war vorher 14

int buttonPin = 0;

void setup()
{
  Serial.begin(9600);

  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(buttonPin, INPUT);


  servoblau.attach(12); //D6
  servorot.attach(13); //D7

  setRedLightOff();
  setGreenLightOff();
  
  connectWifi();
  beginServer();
  servorot.write(0);
  errorModeEnabled = false;
  karteLiegtAuf = false;

  
  
  
}

void loop() {
  server.handleClient();
  delay(1000);

  bool buttonState = digitalRead(buttonPin);
  if (buttonState == LOW) // check if the button is pressed
  {
     buttonPressed();
     setGreenLightOn();
  }
  setGreenLightOff();
}

void connectWifi()
{
  WiFi.enableSTA(true);
  delay(2000);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); 
    Serial.print(".");
    redGreenBlink();
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void beginServer()
{
  //server.on ( "/", handleRoot );
  server.on("/chipArgs", handleChipArg); 
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println ( "HTTP server started" );
  getChip();
}

/*void handleRoot() {
  if ( server.hasArg("SERVO") ) {
    handleSubmit();
  } else {
    server.send ( 200, "text/html", getPage() );
  }
  if ( server.hasArg("KARTE") ) {
    handleSubmit();
  } else {
    server.send ( 200, "text/html", getPage() );
  }
  if ( server.hasArg("CHIP") ) {
    handleSubmit();
  } else {
    server.send ( 200, "text/html", getPage() );
  }
}*/

void handleChipArg() { 

  String message = "";

    if (server.arg("IchWillChip") == "getChip"){     //Parameter not found

      message = "Argument = ";
      message += server.arg("IchWillChip"); 
      getChip();  
    
    } else if (server.arg("IchWillChip") == "error"){     //Parameter not found

      message = "Argument = ";
      message += server.arg("IchWillChip Error"); 
      failBlink();  
    
    }
    else {     //Parameter found
      message = "Argument not found";
    }

    server.send(200, "text/plain", message);          //Returns the HTTP response

}

void handleSubmit() {

  /*String SERVOValue;
  SERVOValue = server.arg("SERVO");

  String KARTEValue;
  KARTEValue = server.arg("KARTE");
  
  Serial.println("Set GPIO ");
  Serial.print(SERVOValue);


  if ( KARTEValue == "KarteDrauf" ) {
    setRedLightOff();
    setGreenLightOn();
  }
  
  else if ( KARTEValue == "KarteWeg" ) {
    setGreenLightOff();
    setRedLightOn();
  } else {
    Serial.println("Error KARTE Value");
  }

  if ( SERVOValue == "0" ) {
    servoblau.write(0);
    servorot.write(165);
    server.send ( 200, "text/html", getPage() );
  }
  else if ( SERVOValue == "90" )
  {
    servoblau.write(90);
    servorot.write(90);
    server.send ( 200, "text/html", getPage() );
  }
  else if ( SERVOValue == "180" )
  {
    servoblau.write(165);
    servorot.write(0);
    server.send ( 200, "text/html", getPage() );
  } else
  {
    Serial.println("Error Servo Value");
  }*/
}

void buttonPressed() {
   if (checkKarte() == true) {
     setGreenLightOn();
     delay(1000);
     setGreenLightOff();
     postButtonPressed();
   } else {
     Serial.println("Es wurde keine Karte aufgelegt oder anderer Fehler");
     setGreenLightOff();
     setRedLightOn();
     delay(5000);
     setRedLightOff();
   }
}

void postButtonPressed() {
  if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
 
   HTTPClient http;    //Declare object of class HTTPClient
 
   http.begin("http://192.168.19.100/index2.php");      //Specify request destination
   http.addHeader("Content-Type", "application/x-www-form-urlencoded");  //Specify content-type header
 
   int httpCode = http.POST("call_this5=IchWillChip");   //Send the request
   String payload = http.getString();                  //Get the response payload
 
   Serial.println(httpCode);   //Print HTTP return code
   Serial.println(payload);    //Print request response payload
 
   http.end();  //Close connection
 
 }else{
 
    Serial.println("Error in WiFi connection");   
 
 }
 
  delay(1000);  //Send a request every 1 seconds
 
}

void getChip() {
    servoblau.write(0);
    servorot.write(165);
    delay(1000);
    servoblau.write(165);
    servorot.write(0);
    server.send ( 200, "text/html", getPage() );
    sendAck();
}

bool checkKarte() {
  // Raspbery Pi fragen ob Karte drauf liegt
  //Update: Ist unnötig, da Pi das selbst macht
  karteLiegtAuf = true;
  return true;
} 

void setGreenLightOn() {
   digitalWrite(ledG, HIGH);
}

void setGreenLightOff() {
   digitalWrite(ledG, LOW);
}

void setRedLightOn() {
  digitalWrite(ledR, HIGH);
}

void setRedLightOff() {
  digitalWrite(ledR, LOW);
}

void redGreenBlink() {
   digitalWrite(ledG, LOW);
   digitalWrite(ledR, LOW);
   digitalWrite(ledG, HIGH);
   digitalWrite(ledR, LOW);
   delay(400);
   digitalWrite(ledG, LOW);
   digitalWrite(ledR, HIGH);
   delay(400);
   digitalWrite(ledG, LOW);
   digitalWrite(ledR, LOW);
}

void errorBlink() {
  while (errorModeEnabled == true)
  {
   digitalWrite(ledG, HIGH);
   digitalWrite(ledR, LOW);
   delay(800);
   digitalWrite(ledG, LOW);
   digitalWrite(ledR, HIGH);
   delay(800);
  }
}

void failBlink() {
  for(int i = 0; i<4; i++){
     setRedLightOn();
     delay(700);
     setRedLightOff();
     delay(700);
  }
}

void successBlink() {
  for(int i = 0; i<3; i++){
     setGreenLightOn();
     delay(400);
     setGreenLightOff();
     delay(400);
  }
}

void sendAck() {
  successBlink();
  //send Timestamp as acknoledgement or only send acknoledgement
  //Noch nicht Implementiert, da bis jetzt nicht notwendig
}

void handleNotFound() {
  // We construct a message to be returned to the client
  String message = "File Not Found\n\n";
  // which includes what URI was requested
  message += "URI: ";
  message += server.uri();
  // what method was used
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  // and what parameters were passed
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  // the response, as expected, is a "Not Found" (404) error
  server.send(404, "text/plain", message);
}

String getPage() {
  String page = 
  "<html lang=en-EN>\
      <head>\
          <meta http-equiv='refresh' content='60'/>\
          <title>arduino-projekte.info</title>\
           <style>\ 
              body { background-color: #fffff; font-family: Arial, Helvetica, Sans-Serif; Color: #000000; }\
                    .button {display: inline-block; padding: 15px 15px; font-size: 25px; cursor: pointer; text-align: center; text-decoration: none; outline: none; color: #ffffff; background-color: #4db2ec; border: none; border-radius: 15px;}\
                    .button:hover {background-color: #4DCAEC; color:#ffffff;}\
                    .button:active {background-color: #4DCAEC; box-shadow: 0 3px #666; transform: translateY(3px); }\
           </style>\
      </head>\
   <body>\
      <h1>ChipAutomat Web Interface</h1>\
      <h2>Der ChipAutomat ist online</h2>\
  </body>\
  </html>";

  /*  <form action='/' method='POST'>\
      <INPUT class='button' type='submit' name='SERVO' value='0'>\    
      <INPUT class='button' type='submit' name='SERVO' value='90'>\    
      <INPUT class='button' type='submit' name='SERVO' value='180'>\
      <INPUT class='button' type='submit' name='KARTE' value='KarteDrauf'>\
      <INPUT class='button' type='submit' name='KARTE' value='KarteWeg'>\*/
  return page;
}
