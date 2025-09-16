/* Projeto Láb. Iot, Conexão Esp32 ao servidor Thingsboard, UFPR - Autor: João André A. da Silva */
#ifndef WIFI_MANAGER
#define WIFI_MANAGER

#include "IoT_Manager.h"
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "esp_eap_client.h"
//#include <ESP8266WiFi.h>  // -> Se for um ESP8266
//#include <HTTPClient.h> //-> UFPR_SEM_FIO
#define ENCRYPTED false

class WifiManager {
   public:
      static void LoginWifi(String Wifi_Ssid="", String Wifi_Password="", String Wifi_Login_Username="", String Wifi_Login_Password="");
      static bool IsWifiConnected();
      #if ENCRYPTED
	      static WiFiClientSecure wifiClient;
      #else	
         static WiFiClient wifiClient;
      #endif
      static String WIFI_SSID;
      static String WIFI_PASSWORD;
      static String WIFI_LOGIN_USERNAME;
      static String WIFI_LOGIN_PASSWORD;
   private:
      WifiManager() {}
      static void HandleEAP_Wifi();
      static bool InitWiFi();
      static constexpr uint8_t WIFI_CONNECTION_TIMEOUT = 10;
      //#define WIFI_PORTAL_URL "http://www.msftconnecttest.com/redirect" //-> UFPR_SEM_FIO
};
#endif