#include <Arduino.h>
#include <Adafruit_BMP280.h>
#include <PubSubClient.h>
#include <WiFiClient.h>
#include <WiFi.h>
#include <string.h>

#include "log.h"
#include "defines.h"

static const char TAG[] = "main.cpp";

/**
 * @brief Função executada quando o tópico em que o esp está
 * inscrito recebe uma mensagem
 * 
 * @param topic 
 * @param message 
 * @param length 
 * @pre Nenhuma
 * @post Nenhuma
 */
void callbackMqtt(char* topic, byte* message, unsigned int length);

/**
 * @brief Conecta no wifi
 * 
 * @pre Nenhuma
 * @post Esp conectado a rede
 */
void wifiConnect();

/**
 * @brief Reconecta ao broker
 * 
 * @pre Nenhuma
 * @post Esp conectado ao broker ou loop infinito
 */
void MqttConnect();

/**
 * @brief Responsável por enviar os dados da temperatura
 * 
 * @pre Nenhuma
 * @post Nenhuma
 */
void taskSendTemperature(void*);

Adafruit_BMP280 sensor;
WiFiClient wifiClient;

/*
inicializa o mqtt com todos os parâmetros necessários, sendo:
    - IP do broker
    - porta utilizada para comunicação
    - função chamada no callbackMqtt
    - Cliente Wifi
		PubSubClient(IPAddress, uint16_t, MQTT_CALLBACK_SIGNATURE,Client& client);
*/
PubSubClient mqttClient(RASP_BROKER_IP, RASP_BROKER_PORT, callbackMqtt, wifiClient);

TaskHandle_t handle = NULL;

void setup() {
    wifiConnect();
    pinMode(LED_PIN, OUTPUT);
	MqttConnect();
	xTaskCreate(taskSendTemperature, "send temperature", 20000, NULL, 1, &handle);
}

void loop() {
    if (!mqttClient.connected()) {
        MqttConnect();
    }
    mqttClient.loop();
}

void callbackMqtt(char* topic, byte* message, unsigned int length) {
    LOGD("Chegou um dado Topico [%s]", topic);
    if (strcmp(topic, SUBSCRIBE_TOPIC)) {
		return ;
	}
    char buffer[length + 1];
    for(int i = 0; i < length; i++) {
        buffer[i] = message[i];
    }
    buffer[length] = '\0';

    if (!strcmp(buffer, "liga")) {
        LOGD("acendendo a luz");
        digitalWrite(LED_PIN, HIGH);
    } else {
        LOGD("apagando a luz");
        digitalWrite(LED_PIN, LOW);
    }
}

void wifiConnect() {
    WiFi.begin(SSID, PASS);
    int time_lim = millis() + 10000;

    while ((WiFi.status() != WL_CONNECTED) && (millis() < time_lim)) {
        ;
    }

    if (WiFi.status() != WL_CONNECTED) {
		LOGE("Falha ao Conectar no Wifi");
        delay(1000);
        ESP.restart();
    }
    WiFi.mode(WIFI_STA);
}

void MqttConnect() {
    LOGD("Conectando ao Broker MQTT...");
    while (!mqttClient.connected()) {
        if (mqttClient.connect(ESP_ID, RASP_BROKER_USER, RASP_BROKER_PASS)) {
            LOGD("Connectado ao Broker");
            mqttClient.subscribe(SUBSCRIBE_TOPIC);
			return;
        }
        LOGD("Reconectando to Broker MQTT...");
    }
}

void taskSendTemperature(void*) {
	char temperature_buffer[8];
	while (true) {
		const auto temperature = sensor.readTemperature();
		LOGD("Temperatura lida = %.2f", temperature);
		sprintf(temperature_buffer, "%.2f", temperature);
		LOGD("Temperatura convertida = %.2f", temperature_buffer);
		if (mqttClient.publish(PUBLISH_TOPIC, temperature_buffer)) {
			LOGD("Enviei dado");
		} else {
			LOGE("Não enviei dado");
		}
		vTaskDelay(5000 / portTICK_RATE_MS);
	}
}