#ifndef DEFINES
#define DEFINES

/**
 * @brief Nome da Rede de wifi a qual deseja conectar
 */
#define SSID "LabIoT"
/**
 * @brief Senha da Rede de wifi a qual deseja conectar
 */
#define PASS "labiot2020."

/**
 * @brief Senha da Rede de wifi a qual deseja conectar
 * rode ip r | grep default ou ip a | grep "inet "
 */
#define RASP_BROKER_IP "192.168.1.100"

/**
 * @brief Caso tenha usuários cadastrados ao broker, informe a senha equivalente
 * para o usário supracitado
 * 
 * equivalente ao -P na shell;
 * caso allow_anonymous seja false, define é necessário
 */
#define RASP_BROKER_PASS "senha"
/**
 * @brief Caso tenha usuários cadastrados ao broker, informe o usuário
 * 
 * equivalente ao -u na shell;
 * caso allow_anonymous seja false, define é necessário
 */
#define RASP_BROKER_USER "esp32"
/**
 * @brief Porta em que o mosquitto está escutando:
 * Default = 1883;
 * Com autenticação = 8883;
 */
#define RASP_BROKER_PORT 1883

/**
 * @brief Tópico em que o esp publica (escreve)
 */
#define PUBLISH_TOPIC "esp/temperatura"
/**
 * @brief Tópico em que o esp se inscreve (lê)
 */
#define SUBSCRIBE_TOPIC "esp/led/config"

#define ESP_ID "esp32-mqtt-demo"

#define WAIT_TIME 5000 //ms

#define LED_PIN LED_BUILTIN

#endif // DEFINES
