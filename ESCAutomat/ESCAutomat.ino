#include <Servo.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* ssid     = "devolo-000B3BCBD17C";
const char* password = "TQMC-BGYL-NIMJ-UZPX";
Servo servoblau;
Servo servorot;
ESP8266WebServer server ( 80 );
bool errorModeEnabled;
bool karteLiegtAuf;
int ledG = 12;
int ledR = 14

void setup()
{
  Serial.begin(9600);
  servoblau.attach(12); //D6
  servorot.attach(13); //D7
  connectWifi();
  beginServer();
  servoblau.write(0);
  servorot.write(165);
  errorModeEnabled = false;
  karteLiegtAuf = false;

  
  
  
}

void loop() {
  server.handleClient();
  delay(1000);

  bool buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) // check if the button is pressed
  {
   getChip();
  }
  
}

void connectWifi()
{
  WiFi.enableSTA(true);
  delay(2000);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void beginServer()
{
  server.on ( "/", handleRoot );
  server.begin();
  Serial.println ( "HTTP server started" );
}

void handleRoot() {
  if ( server.hasArg("SERVO") ) {
    handleSubmit();
  } else {
    server.send ( 200, "text/html", getPage() );
  }
}

void handleSubmit() {

  String SERVOValue;
  SERVOValue = server.arg("SERVO");

  String KARTEValue;
  KARTEValue = server.arg("KARTE");
  
  Serial.println("Set GPIO ");
  Serial.print(SERVOValue);


  if ( KARTEValue == "KarteDrauf" ) {
    setRedLightOff();
    setGreenLightOn()
   /* servoblau.write(0);
    servorot.write(165);
    server.send ( 200, "text/html", getPage() );*/
    
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
  }
}

void buttonPressed() {
   if (checkKarte() == true) {
     getChip();
   } else {
     Serial.println("Es wurde keine Karte aufgelegt oder anderer Fehler");
     setGreenLightOff();
     setRedLightOn();
   }
}

void getChip() {
    servoblau.write(0);
    servorot.write(165);
    //server.send ( 200, "text/html", getPage() );
    delay(2000);
    servoblau.write(165);
    servorot.write(0);
    server.send ( 200, "text/html", getPage() );
    sendAck();
}

bool checkKarte() {
  // Raspbery Pi fragen ob Karte drauf liegt
  return true;
} 

void setGreenLightOn() {
   digitalWrite(ledG, HIGH);
}

void setGreenLightOff() {
   digitalWrite(led, LOW);
}

void setRedLightOn() {
  digitalWrite(ledR, HIGH);
}

void setRedLightOff() {
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

void sendAck() {
  for(int i = 0; i<3; i++){
     setGreenLightOn();
     delay(800);
     setGreenLightOff();
     delay(800);
  }
  //send Timestamp as acknoledgement or only send acknoledgement
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
      <h3>Servo Test</h3>\
      <form action='/' method='POST'>\
      <INPUT class='button' type='submit' name='SERVO' value='0'>\    
      <INPUT class='button' type='submit' name='SERVO' value='90'>\    
      <INPUT class='button' type='submit' name='SERVO' value='180'>\
      <INPUT class='button' type='submit' name='KARTE' value='KarteDrauf'>\
      <INPUT class='button' type='submit' name='KARTE' value='KarteWeg'>\
  </body>\
  </html>";

/*page = "<html>\
  <head>\
    <meta http-equiv='refresh' content='5'/>\
    <title>ESP8266 Demo</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h1>Hello from ESP8266!</h1>\
    <p>Uptime: %02d:%02d:%02d</p>\
    <img src=\"/test.svg\" />\
  </body>\
</html>";*/

    
  return page;
}
