#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

const int motorAL = 15;
const int motorAR = 16;
const int motorBL = 17;
const int motorBR = 18;

const int speeeeed = 50; //0 - 255

// WiFi credentials
const char *ssid = "kabum2";             // Replace with your WiFi name
const char *password = "skratek2";     // Replace with your WiFi password

// MQTT Broker settings
const char *mqtt_broker = "213.172.234.74";
const char *mqtt_topic = "esp/marcel";
const char *mqtt_username = "pp";
const char *mqtt_password = "pp";
const int mqtt_port = 1883;

// WiFi and MQTT client initialization
WiFiClient esp_client;
PubSubClient mqtt_client(esp_client);

void connectToWiFi() {
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

void connectToMQTT() {
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

void mqttCallback(char* topic, byte* payload, unsigned int length) {
    String message = "";
    for (int i = 0; i < length; i++) {
        message += (char)payload[i];
    }
    
    Serial.print(topic);
    Serial.print(": ");
    Serial.println(message);

    if (message == "naprej") {
        analogWrite(motorAL, speeeeed);
        analogWrite(motorAR, 0);
        analogWrite(motorBL, speeeeed);
        analogWrite(motorBR, 0);
        delay(1000);
        analogWrite(motorAL, 0);
        analogWrite(motorAR, 0);
        analogWrite(motorBL, 0);
        analogWrite(motorBR, 0);
    } else if (message == "nazaj") {
        analogWrite(motorAL, 0);
        analogWrite(motorAR, speeeeed);
        analogWrite(motorBL, 0);
        analogWrite(motorBR, speeeeed);
        delay(1000);
        analogWrite(motorAL, 0);
        analogWrite(motorAR, 0);
        analogWrite(motorBL, 0);
        analogWrite(motorBR, 0);
    }
}

void setup() {
    pinMode(motorAL, OUTPUT);
    pinMode(motorAR, OUTPUT);
    pinMode(motorBL, OUTPUT);
    pinMode(motorBR, OUTPUT);

    Serial.begin(921600);
    Serial.println("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");

    connectToWiFi();
    mqtt_client.setServer(mqtt_broker, mqtt_port);
    mqtt_client.setCallback(mqttCallback);
    connectToMQTT();
}

void loop() { 
    if (!mqtt_client.connected()) {
        connectToMQTT();
    }
    mqtt_client.loop();
}
