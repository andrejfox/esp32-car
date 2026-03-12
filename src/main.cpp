#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <esp_sleep.h>

const char *ssid = "kersnikova-office";
const char *password = "2015kersnikovaOFFICE03";

const char *mqtt_broker = "192.168.200.144";
const int mqtt_port = 1883;
const char *mqtt_topic = "a/test";

const uint64_t sleep_seconds = 20;

const int led_pin_r = 37;

const int pwm_channel = 0;
const int pwm_freq = 5000;
const int pwm_res = 10;
const int pwm_pin = 7;
const int pwm_read_pin = 15;
const int moisture_min = 1500;
const int moisture_max = 500;

// Persists across deep sleep cycles
RTC_DATA_ATTR uint64_t uptime_seconds = 0;

WiFiClient esp_client;
PubSubClient mqtt_client(esp_client);

int readMoistureRaw() {
    ledcWrite(pwm_channel, 1);
    delay(10);
    return analogRead(pwm_read_pin);
}

void connectToWiFi() {
    if (WiFi.status() == WL_CONNECTED) return;

    Serial.println("Connecting to WiFi...");
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.print("WiFi connected, IP: ");
    Serial.println(WiFi.localIP());
}

void connectToMQTT() {
    while (!mqtt_client.connected()) {
        String client_id = "esp32-client-" + String(WiFi.macAddress());
        Serial.print("Connecting to MQTT broker...");

        if (mqtt_client.connect(client_id.c_str())) {
            Serial.println(" connected");
        } else {
            Serial.print(" failed, rc=");
            Serial.print(mqtt_client.state());
            Serial.println(" (retry in 3s)");
            delay(3000);
        }
    }
}

void setup() {
    Serial.begin(115200);
    delay(200);
    const uint32_t cycle_start_ms = millis();

    // Red LED on while active
    pinMode(led_pin_r, OUTPUT);
    digitalWrite(led_pin_r, HIGH);

    // Moisture sensor PWM setup
    ledcSetup(pwm_channel, pwm_freq, pwm_res);
    ledcAttachPin(pwm_pin, pwm_channel);

    Serial.print("Booting... uptime: ");
    Serial.print(uptime_seconds);
    Serial.println("s");

    connectToWiFi();
    mqtt_client.setServer(mqtt_broker, mqtt_port);
    connectToMQTT();

    const int moisture_raw = readMoistureRaw();
    const int moisture_percent = constrain(map(moisture_raw, moisture_min, moisture_max, 0, 100), 0, 100);
    const uint32_t cycle_ms = millis() - cycle_start_ms;

    String payload = "{\n\"cycle_ms\":" + String(cycle_ms)
                   + ",\n\"uptime_s\":" + String(uptime_seconds)
                   + ",\n\"moisture_raw\":" + String(moisture_raw)
                   + ",\n\"moisture_percent\":" + String(moisture_percent)
                   + "\n}";
    const bool sent = mqtt_client.publish(mqtt_topic, payload.c_str());

    Serial.print("Publish: ");
    Serial.print(payload);
    Serial.print(" -> ");
    Serial.println(sent ? "OK" : "FAILED");

    mqtt_client.loop();
    delay(200);

    uptime_seconds += sleep_seconds;

    // Red LED off before sleep
    digitalWrite(led_pin_r, LOW);

    Serial.print("Deep sleeping for ");
    Serial.print(sleep_seconds);
    Serial.println("s...");
    Serial.flush();

    esp_sleep_enable_timer_wakeup(sleep_seconds * 1000000ULL);
    esp_deep_sleep_start();
}

void loop() {
    // Not used: deep sleep restarts setup() on wake
}
