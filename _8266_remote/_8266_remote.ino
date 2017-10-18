
  
#include <ArduinoOTA.h>

#include <dummy.h>
#include <EEPROM.h>

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Ticker.h>

#define BUTTON1          D0       
#define BUTTON2          D7      
#define BUTTON3          D6   
#define BUTTON4          D1   
#define BUTTON5          D8     
#define BUTTON6          D5      
#define BUTTON7          4     
#define RELAY1           D3                                
#define RELAY2           D3                                 
#define RELAY3           D3                               
#define RELAY4           D3    
#define RELAY5           D3
#define RELAY6           D3
#define RELAY7           D3                           
#define LED             D3                             

#define MQTT_CLIENT     "8switch"         
#define MQTT_SERVER     "192.168.123.110"                      // MQTT服务器IP
#define MQTT_PORT       1883                                
#define MQTT_TOPIC      "home/8ch/2"         
#define MQTT_USER       "ming"                               // MQTT服务器用户名
#define MQTT_PASS       "9228"                               //MQTT服务器的密码

#define WIFI_SSID       "malimalihome"                           // wifi ssid(你家的wifi名字，注意不能是5G频段)
#define WIFI_PASS       "922727lm"                       // wifi password(你家的wifi密码)

#define VERSION    "\n\n------------------ BY SE7EN  -------------------"

                          
bool sendStatus1 = false;
bool sendStatus2 = false;               
bool sendStatus3 = false;   
bool sendStatus4 = false;        
bool sendStatus5 = false;  
bool sendStatus6 = false;             
bool sendStatus7 = false;    

                                 
extern "C" { 
  #include "user_interface.h" 
}

bool sendStatus = false;
bool requestRestart = false;

int kUpdFreq = 1;
int kRetries = 10;

unsigned long TTasks;
unsigned long count = 0;
unsigned long count1 = 0;                                   
unsigned long count2 = 0;                                   
unsigned long count3 = 0;                                  
unsigned long count4 = 0;                                    
unsigned long count5 = 0;                                    
unsigned long count6 = 0;                                   
unsigned long count7 = 0;                                   

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient, MQTT_SERVER, MQTT_PORT);
Ticker btn_timer1, btn_timer2, btn_timer3, btn_timer4, btn_timer5, btn_timer6, btn_timer7;

void callback(const MQTT::Publish& pub) {
  if (pub.payload_string() == "stat") {
  }
  else if (pub.payload_string() == "1on") {
    digitalWrite(RELAY1, HIGH);
    sendStatus1 = true;
  }
  else if (pub.payload_string() == "1off") {
    digitalWrite(RELAY1, LOW);
    sendStatus1 = true;
  }
  else if (pub.payload_string() == "2on") {
    digitalWrite(RELAY2, HIGH);
    sendStatus2 = true;
  }
  else if (pub.payload_string() == "2off") {   
    digitalWrite(RELAY2, LOW);
    sendStatus2 = true;
  }
  else if (pub.payload_string() == "3on") {
    digitalWrite(RELAY3, HIGH);
    sendStatus3 = true;
  }
  else if (pub.payload_string() == "3off") {
    digitalWrite(RELAY3, LOW);
    sendStatus3 = true;
  }
  else if (pub.payload_string() == "4on") {
    digitalWrite(RELAY4, HIGH);
    sendStatus4 = true;
  }
  else if (pub.payload_string() == "4off") {
    digitalWrite(RELAY4, LOW);
    sendStatus4 = true;
  }
  else if (pub.payload_string() == "5on") {
    digitalWrite(RELAY5, HIGH);
    sendStatus5 = true;
  }
  else if (pub.payload_string() == "5off") {
    digitalWrite(RELAY5, LOW);
    sendStatus5 = true;
  }
  else if (pub.payload_string() == "6on") {
    digitalWrite(RELAY6, HIGH);
    sendStatus6 = true;
  }
  else if (pub.payload_string() == "6off") { 
    digitalWrite(RELAY6, LOW);
    sendStatus6 = true;
  }
  else if (pub.payload_string() == "7on") {
    digitalWrite(RELAY7, HIGH);
    sendStatus7 = true;
  }
  else if (pub.payload_string() == "7off") {
    digitalWrite(RELAY7, LOW);
    sendStatus7 = true;
  }  
  else if (pub.payload_string() == "reset") {
    requestRestart = true;
  }
}

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);  
  pinMode(RELAY3, OUTPUT);
  pinMode(RELAY4, OUTPUT);
  pinMode(RELAY5, OUTPUT);  
  pinMode(RELAY6, OUTPUT);  
  pinMode(RELAY7, OUTPUT);  
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(BUTTON3, INPUT_PULLUP); 
  pinMode(BUTTON4, INPUT_PULLUP);  
  pinMode(BUTTON5, INPUT_PULLUP);
  pinMode(BUTTON6, INPUT_PULLUP);
  pinMode(BUTTON7, INPUT_PULLUP);
  digitalWrite(LED, HIGH);
  digitalWrite(RELAY1, LOW);
  digitalWrite(RELAY2, LOW);
  digitalWrite(RELAY3, LOW);  
  digitalWrite(RELAY4, LOW);  
  digitalWrite(RELAY5, LOW);  
  digitalWrite(RELAY6, LOW);   
  digitalWrite(RELAY7, LOW);   
  btn_timer1.attach(0.05, button1);
  btn_timer2.attach(0.05, button2);
  btn_timer3.attach(0.05, button3); 
  btn_timer4.attach(0.05, button4);  
  btn_timer5.attach(0.05, button5); 
  btn_timer6.attach(0.05, button6); 
  btn_timer7.attach(0.05, button7);   
  mqttClient.set_callback(callback);
  
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.begin(115200);
  Serial.println(VERSION);
  Serial.print("\nESP ChipID: ");
  Serial.print(ESP.getChipId(), HEX);
  Serial.print("\nConnecting to "); Serial.print(WIFI_SSID); Serial.print(" Wifi"); 
  while ((WiFi.status() != WL_CONNECTED) && kRetries --) {
    delay(500);
    Serial.print(" .");
  }
  if (WiFi.status() == WL_CONNECTED) {  
    Serial.println(" DONE");
    Serial.print("IP Address is: "); Serial.println(WiFi.localIP());
    Serial.print("Connecting to ");Serial.print(MQTT_SERVER);Serial.print(" Broker . .");
    delay(500);
    while (!mqttClient.connect(MQTT::Connect(MQTT_CLIENT).set_keepalive(90).set_auth(MQTT_USER, MQTT_PASS)) && kRetries --) {
      Serial.print(" .");
      delay(1000);
    }
    if(mqttClient.connected()) {
      Serial.println(" DONE");
      Serial.println("\n----------------------------  Logs  ----------------------------");
      Serial.println();
      mqttClient.subscribe(MQTT_TOPIC);
      blinkLED(LED, 40, 8);
      digitalWrite(LED, LOW);
    }
    else {
      Serial.println(" FAILED!");
      Serial.println("\n----------------------------------------------------------------");
      Serial.println();
    }
  }
  else {
    Serial.println(" WiFi FAILED!");
    Serial.println("\n----------------------------------------------------------------");
    Serial.println();
  }
}

void loop() { 
  mqttClient.loop();
  timedTasks();
  checkStatus();
}

void blinkLED(int pin, int duration, int n) {             
  for(int i=0; i<n; i++)  {  
    digitalWrite(pin, HIGH);        
    delay(duration);
    digitalWrite(pin, LOW);
    delay(duration);
  }
}

void button1() {
  if (!digitalRead(BUTTON1)) {
    count1++;
  } 
  else {
    if (count1 > 1 && count1 <= 40) {   
      digitalWrite(RELAY1, !digitalRead(RELAY1));
      sendStatus1 = true;
    } 
    else if (count1 >40){
      Serial.println("\n\nSonoff Rebooting . . . . . . . . Please Wait"); 
      requestRestart = true;
    } 
    count1=0;
  }
}

void button2() {
  if (!digitalRead(BUTTON2)) {
    count2++;
  } 
  else {
    if (count2 > 1 && count2 <= 40) {   
      digitalWrite(RELAY2, !digitalRead(RELAY2));
      sendStatus2 = true;
    } 
    count2=0;
  }
}
void button3() {
  if (!digitalRead(BUTTON3)) {
    count3++;
  } 
  else {
    if (count3 > 1 && count3 <= 40) {   
      digitalWrite(RELAY3, !digitalRead(RELAY3));
      sendStatus3 = true;
    } 
    count3=0;
  }
}
void button4() {
  if (!digitalRead(BUTTON4)) {
    count4++;
  } 
  else {
    if (count4 > 1 && count4 <= 40) {   
      digitalWrite(RELAY4, !digitalRead(RELAY4));
      sendStatus4 = true;
    } 
    count4=0;
  }
}
void button5() {
  if (!digitalRead(BUTTON5)) {
    count5++;
  } 
  else {
    if (count5 > 1 && count5 <= 40) {   
      digitalWrite(RELAY5, !digitalRead(RELAY5));
      sendStatus5 = true;
    } 
    count5=0;
  }
}
void button6() {
  if (!digitalRead(BUTTON6)) {
    count6++;
  } 
  else {
    if (count6 > 1 && count6 <= 40) {   
      digitalWrite(RELAY6, !digitalRead(RELAY6));
      sendStatus6 = true;
    } 
    count6=0;
  }
}
void button7() {
  if (!digitalRead(BUTTON7)) {
    count7++;
  } 
  else {
    if (count7 > 1 && count7 <= 40) {   
      digitalWrite(RELAY7, !digitalRead(RELAY7));
      sendStatus7 = true;
    } 
    count7=0;
  }
}
void checkConnection() {
  if (WiFi.status() == WL_CONNECTED)  {
    if (mqttClient.connected()) {
      Serial.println("mqtt broker connection . . . . . . . . . . OK");
    } 
    else {
      Serial.println("mqtt broker connection . . . . . . . . . . LOST");
      requestRestart = true;
    }
  }
  else { 
    Serial.println("WiFi connection . . . . . . . . . . LOST");
    requestRestart = true;
  }
}

void checkStatus() {
  if (sendStatus1) {
    if(digitalRead(RELAY1) == LOW)  {
      mqttClient.publish(MQTT::Publish(MQTT_TOPIC"/stat", "1off").set_retain().set_qos(1));
      Serial.println("Relay 1 . . . . . . . . . . . . . . . . . . OFF");
    } else {
    mqttClient.publish(MQTT::Publish(MQTT_TOPIC"/stat", "1on").set_retain().set_qos(1));
    Serial.println("Relay 1 . . . . . . . . . . . . . . . . . . ON");
    }
    sendStatus1 = false;
  }
  if (sendStatus2) {
    if(digitalRead(RELAY2) == LOW)  {
      mqttClient.publish(MQTT::Publish(MQTT_TOPIC"/stat", "2off").set_retain().set_qos(2));
      Serial.println("Relay 2 . . . . . . . . . . . . . . . . . . OFF");
    } else {
    mqttClient.publish(MQTT::Publish(MQTT_TOPIC"/stat", "2on").set_retain().set_qos(2));
    Serial.println("Relay 2 . . . . . . . . . . . . . . . . . . ON");
    }
    sendStatus2 = false;
  }
  if (sendStatus3) {
    if(digitalRead(RELAY3) == LOW)  {
      mqttClient.publish(MQTT::Publish(MQTT_TOPIC"/stat", "3off").set_retain().set_qos(3));
      Serial.println("Relay 3 . . . . . . . . . . . . . . . . . . OFF");
    } else {
    mqttClient.publish(MQTT::Publish(MQTT_TOPIC"/stat", "3on").set_retain().set_qos(3));
    Serial.println("Relay 3 . . . . . . . . . . . . . . . . . . ON");
    }
    sendStatus3 = false;
  }
  if (sendStatus4) {
    if(digitalRead(RELAY4) == LOW)  {
      mqttClient.publish(MQTT::Publish(MQTT_TOPIC"/stat", "4off").set_retain().set_qos(4));
      Serial.println("Relay 4 . . . . . . . . . . . . . . . . . . OFF");
    } else {
    mqttClient.publish(MQTT::Publish(MQTT_TOPIC"/stat", "4on").set_retain().set_qos(4));
    Serial.println("Relay 4 . . . . . . . . . . . . . . . . . . ON");
    }
    sendStatus4 = false;
  }
  if (sendStatus5) {
    if(digitalRead(RELAY5) == LOW)  {
      mqttClient.publish(MQTT::Publish(MQTT_TOPIC"/stat", "5off").set_retain().set_qos(5));
      Serial.println("Relay 5 . . . . . . . . . . . . . . . . . . OFF");
    } else {
    mqttClient.publish(MQTT::Publish(MQTT_TOPIC"/stat", "5on").set_retain().set_qos(5));
    Serial.println("Relay 5 . . . . . . . . . . . . . . . . . . ON");
    }
    sendStatus5 = false;
  }  
  if (sendStatus6) {
    if(digitalRead(RELAY6) == LOW)  {
      mqttClient.publish(MQTT::Publish(MQTT_TOPIC"/stat", "6off").set_retain().set_qos(6));
      Serial.println("Relay 6 . . . . . . . . . . . . . . . . . . OFF");
    } else {
    mqttClient.publish(MQTT::Publish(MQTT_TOPIC"/stat", "6on").set_retain().set_qos(6));
    Serial.println("Relay 6 . . . . . . . . . . . . . . . . . . ON");
    }
    sendStatus6 = false;
  }  
  if (sendStatus7) {
    if(digitalRead(RELAY7) == LOW)  {
      mqttClient.publish(MQTT::Publish(MQTT_TOPIC"/stat", "7off").set_retain().set_qos(7));
      Serial.println("Relay 7 . . . . . . . . . . . . . . . . . . OFF");
    } else {
    mqttClient.publish(MQTT::Publish(MQTT_TOPIC"/stat", "7on").set_retain().set_qos(7));
    Serial.println("Relay 7 . . . . . . . . . . . . . . . . . . ON");
    }
    sendStatus7 = false;
  }  
  if (requestRestart) {
    blinkLED(LED, 400, 4);
    ESP.restart();
  }
}

void timedTasks() {
  if ((millis() > TTasks + (kUpdFreq*60000)) || (millis() < TTasks)) { 
    TTasks = millis();
    checkConnection();
  }
}
