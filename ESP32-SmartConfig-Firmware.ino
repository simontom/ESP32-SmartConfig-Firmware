
////    Use Android Application    ////
////      ESP8266 SmartConfig      ////
///////////////////////////////////////

#define CORE_DEBUG_LEVEL  ARDUHAL_LOG_LEVEL_VERBOSE

#include <initializer_list> // WORKAROUND - Intellisense does not work w/o this line

#include <esp_wifi.h>
#include <Streaming.h>
#include <WiFi.h>


#define BUTTON_PIN_0	0	/* Do not hold this during reset; starts DOWNLOAD MODE */
#define BUTTON_PIN_15	15


String getWifiSsid() {
	wifi_config_t conf;
	esp_wifi_get_config(WIFI_IF_STA, &conf);
	return String(reinterpret_cast<const char*>(conf.sta.ssid));
}

String getWifiPassword() {
	wifi_config_t conf;
	esp_wifi_get_config(WIFI_IF_STA, &conf);
	return String(reinterpret_cast<const char*>(conf.sta.password));
}


void setup() {
	//esp_log_level_set("*", ESP_LOG_VERBOSE);

	pinMode(BUTTON_PIN_0, INPUT_PULLUP);
	pinMode(BUTTON_PIN_15, INPUT_PULLUP);

	Serial.begin(115200);
	Serial << "Booting ..." << endl;

	WiFi.mode(WIFI_AP_STA);
	Serial << "\t Last SSID = " << getWifiSsid() << endl;
	Serial << "\t Last PASSS = " << getWifiPassword() << endl;

	if (digitalRead(BUTTON_PIN_15) != HIGH) {
		Serial << "Starting Smart Config" << endl;
		WiFi.beginSmartConfig();

		Serial << "Waiting for SmartConfig" << endl;
		while (!WiFi.smartConfigDone()) {
			delay(500);
			Serial << ".";
		}
		Serial.println();
		Serial << "SmartConfig done." << endl;

		Serial << "\n\tREADING NVR\n";
		Serial << "\t_new_SSID_ = " << getWifiSsid() << endl;
		Serial << "\t_new_PASSS_ = " << getWifiPassword() << endl;
		Serial << "\tPSK  = " << WiFi.psk() << endl;
	}
	else {
		WiFi.reconnect();
	}

	Serial << endl << "Waiting for WiFi to connect" << endl;
	if (WiFi.waitForConnectResult() != WL_CONNECTED) {
		ESP.restart();
	}
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial << ".";
	}
	Serial << "WiFi Connected" << endl;
	Serial << "\tIP Address: " << WiFi.localIP() << endl;
	Serial << "\tSSID: " << WiFi.SSID() << endl;
}

void loop() {
	// Left unused..so far
}
