#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

// WiFi nastavitve
const char *ssid = "zavod404"; // Ime WiFi-ja
const char *password = "zavod404"; // Geslo WiFi-ja

// MQTT Broker nastavitve
const char *mqtt_broker = "213.172.234.74"; // naslov / IP
const char *mqtt_topic = "esp/test";
const char *mqtt_username = "pp"; 
const char *mqtt_password = "pp";
const int mqtt_port = 1883;

// pini
int motorL_1 = 15;
int motorL_2 = 16;
int motorD_1 = 17;
int motorD_2 = 18;
int trigPin = 4;
int echoPin = 5;
int lucka = 9;

// globalne spremenlivke
int hitrost = 200;
float duration, distance;
bool automatic_mode = false;

// WiFi and MQTT client inicializacija
WiFiClient esp_client;
PubSubClient mqtt_client(esp_client);

void naprej() {
    analogWrite(motorD_1, hitrost);
    analogWrite(motorD_2, LOW);
    analogWrite(motorL_1, LOW);
    analogWrite(motorL_2, hitrost);
}

void nazaj() {
    analogWrite(motorD_1, LOW);
    analogWrite(motorD_2, hitrost);
    analogWrite(motorL_1, hitrost);
    analogWrite(motorL_2, LOW);
}

void stop() {
    analogWrite(motorD_1, LOW);
    analogWrite(motorD_2, LOW);
    analogWrite(motorL_1, LOW);
    analogWrite(motorL_2, LOW);
}

void levi_nazaj() {
    analogWrite(motorD_1, LOW);
    analogWrite(motorD_2, hitrost);
    analogWrite(motorL_1, LOW);
    analogWrite(motorL_2, LOW);
}

void levi_naprej() {
    analogWrite(motorD_1, hitrost);
    analogWrite(motorD_2, LOW);
    analogWrite(motorL_1, LOW);
    analogWrite(motorL_2, LOW);
}

void desni_nazaj() {
    analogWrite(motorD_1, LOW);
    analogWrite(motorD_2, LOW);
    analogWrite(motorL_1, hitrost);
    analogWrite(motorL_2, LOW);
}

void desni_naprej() {
    analogWrite(motorD_1, LOW);
    analogWrite(motorD_2, LOW);
    analogWrite(motorL_1, LOW);
    analogWrite(motorL_2, hitrost);
}

void obrni_levo(int cas_obracanja) {
    analogWrite(motorD_1, LOW);
    analogWrite(motorD_2, hitrost);
    analogWrite(motorL_1, LOW);
    analogWrite(motorL_2, hitrost);
    delay(cas_obracanja);
}

void obrni_desno(int cas_obracanja) {
    analogWrite(motorD_1, hitrost);
    analogWrite(motorD_2, LOW);
    analogWrite(motorL_1, hitrost);
    analogWrite(motorL_2, LOW);
    delay(cas_obracanja);
}

void automatic() { // Automatsko premikanje
    do {
        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);

        duration = pulseIn(echoPin, HIGH);
        distance = (duration * 0.0343) / 2;
        Serial.print("Distance: ");
        Serial.println(distance);
        delay(100);

        if (distance < 10) {
            nazaj();
            delay(500);
            obrni_levo(1000);
        } else {
            naprej();
        }
        mqtt_client.loop();
    } while (automatic_mode == true);
}

void connectToWiFi() { // Povezovanje z internetom
    WiFi.begin(ssid, password);
    Serial.println();
    Serial.println("Connecting to WiFi");
    Serial.print("SSID: ");
    Serial.println(ssid);
    Serial.print("Password: ");
    Serial.println(password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(50);
        Serial.print(".");
    }
    Serial.println("\nConnected to WiFi\n");
}

void connectToMQTT() { // povezovanje z MQTT brockerjem
    while (!mqtt_client.connected()) {
        String client_id = "esp32-client-" + String(WiFi.macAddress());
        Serial.printf("Connecting to Broker as [%s]\n", client_id.c_str());
        if (mqtt_client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
            Serial.print("Connected to: ");
            Serial.println(mqtt_broker);
            Serial.print("Username: ");
            Serial.println(mqtt_username);
            Serial.print("Password: ");
            Serial.println(mqtt_password);
            mqtt_client.subscribe(mqtt_topic);
            Serial.print("Topic: ");
            Serial.println(mqtt_topic);
            Serial.println();
        } else {
            Serial.print("Failed to connect to MQTT broker, rc=");
            Serial.print(mqtt_client.state());
            Serial.println(" Retrying in 5 seconds.");
            delay(5000);
        }
    }
}

void mqttCallback(char *topic, byte *payload, unsigned int length) {
    String message = "";
    for (int i = 0; i < length; i++) { // Sestavimo spozočilo
        message += (char)payload[i];
    }

    Serial.print(topic);
    Serial.print(": ");
    Serial.println(message);

    if (message == "naprej") { // Preverimo kaj je sporočilo
        automatic_mode = false;
        Serial.println("naprej");
        naprej();
    } else if (message == "nazaj") {
        automatic_mode = false;
        nazaj();
    } else if (message == "stop") {
        automatic_mode = false;
        stop();
    } else if (message == "obrni") {
        automatic_mode = false;
        obrni_desno(500);
    } else if (message == "auto") {
        automatic_mode = true;
        automatic();
    }
}

void pinSetup() { // pine nastavimo na OUTPUT / INPUT
    pinMode(motorD_1, OUTPUT);
    pinMode(motorD_2, OUTPUT);
    pinMode(motorL_1, OUTPUT);
    pinMode(motorL_2, OUTPUT);
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(lucka, OUTPUT);
}

void setup() {
    Serial.begin(115200);
    for(int i = 0; i <= 20; i++) Serial.println("\n");

    connectToWiFi();
    mqtt_client.setServer(mqtt_broker, mqtt_port);
    mqtt_client.setCallback(mqttCallback);
    connectToMQTT();

    pinSetup();
}

void loop() {
    digitalWrite(lucka, HIGH);

    if (!mqtt_client.connected()) {
        connectToMQTT();
        Serial.println("connecting");
    }
    
    mqtt_client.loop();
}