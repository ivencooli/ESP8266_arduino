/*
 * This is a sample configuration file for the "mqtt_esp8266_rgb" light.
 *
 * Change the settings below and save the file as "config.h"
 * You can then upload the code using the Arduino IDE.
 */

// Pins
#define CONFIG_PIN_RED D1
#define CONFIG_PIN_GREEN D2
#define CONFIG_PIN_BLUE D3

// WiFi
#define CONFIG_WIFI_SSID "BigBird"
#define CONFIG_WIFI_PASS "xueba8311"

// MQTT
#define CONFIG_MQTT_HOST "192.168.199.110"
#define CONFIG_MQTT_USER "ming"
#define CONFIG_MQTT_PASS "9227"

#define CONFIG_MQTT_CLIENT_ID "ESPRGBLED" // Must be unique on the MQTT network

// MQTT Topics
#define CONFIG_MQTT_TOPIC_STATE "home/rgb1"
#define CONFIG_MQTT_TOPIC_SET "home/rgb1/set"

#define CONFIG_MQTT_PAYLOAD_ON "ON"
#define CONFIG_MQTT_PAYLOAD_OFF "OFF"
