/* Projeto Láb. Iot, Conexão Esp32 ao servidor Thingsboard, UFPR - Autor: João André A. da Silva */
#ifndef WIFI_MANAGER
#define WIFI_MANAGER

#include "IoT_Manager.h"
//#include <ESP8266WiFi.h>  // -> Se for um ESP8266
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "esp_eap_client.h"
//#include <HTTPClient.h> //-> UFPR_SEM_FIO
#ifndef ENCRYPTED
  #define ENCRYPTED false
#endif

class WifiManager {
   public:
      static void LoginWifi(const char* _Wifi_Ssid="", const char* _Wifi_Password="", const char* _Wifi_Login_Username="", const char* _Wifi_Login_Password="");
      static bool IsWifiConnected();
      #if ENCRYPTED
	      static WiFiClientSecure wifiClient;
      #else	
         static WiFiClient wifiClient;
      #endif
      static String Wifi_Ssid;
      static String Wifi_Password;
      static String Wifi_Login_Username;
      static String Wifi_Login_Password;
   private:
      WifiManager() {}
      static void HandleEAP_Wifi();
      static bool InitWiFi();
      static constexpr uint8_t WIFI_CONNECTION_TIMEOUT = 10;
      //#define WIFI_PORTAL_URL "http://www.msftconnecttest.com/redirect" //-> UFPR_SEM_FIO
};
#endif