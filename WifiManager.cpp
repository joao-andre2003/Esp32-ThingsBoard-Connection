/* Projeto Láb. Iot, Conexão Esp32 ao servidor Thingsboard, UFPR - Autor: João André A. da Silva */
#include "WifiManager.h"

#if ENCRYPTED
	WiFiClientSecure WifiManager::wifiClient;
#else	
	WiFiClient WifiManager::wifiClient;
#endif

String WifiManager::Wifi_Ssid;
String WifiManager::Wifi_Password;
String WifiManager::Wifi_Login_Username;
String WifiManager::Wifi_Login_Password;

void WifiManager::HandleEAP_Wifi() {
  	WiFi.mode(WIFI_STA); 
	esp_eap_client_set_identity((uint8_t *)Wifi_Login_Username.c_str(), strlen(Wifi_Login_Username.c_str()));
  	esp_eap_client_set_username((uint8_t *)Wifi_Login_Username.c_str(), strlen(Wifi_Login_Username.c_str()));
  	esp_eap_client_set_password((uint8_t *)Wifi_Login_Password.c_str(), strlen(Wifi_Login_Password.c_str()));
	
	esp_eap_client_set_ttls_phase2_method(ESP_EAP_TTLS_PHASE2_PAP);
	esp_wifi_sta_enterprise_enable();
	Wifi_Password = "";
}

// Conectar ao wifi
bool WifiManager::InitWiFi() // Conectar ao Wifi
{
	Serial.print("Conectando ao Wifi.");
	WiFi.disconnect(true); 
  	if (Wifi_Ssid == "eduroam") {
		HandleEAP_Wifi();
  	} 
	WiFi.begin(Wifi_Ssid, Wifi_Password);

	uint32_t _connection_time = 0U;
	while (WiFi.status() != WL_CONNECTED) {
		_connection_time += 500U;
		delay(500);
		Serial.print(".");

		if (_connection_time >= WIFI_CONNECTION_TIMEOUT * 1000) {
			Serial.println("\nERRO - Conexão Wifi Timeout.");
			return false;
		}
	}
	Serial.println("\n< Conectado ao Wifi >");
	Serial.print("Endereço IP Obtido: ");
	Serial.println(WiFi.localIP());

	//if (WIFI_SSID == "UFPR_SEM_FIO")
	//	HandleCaptivePortal();
  	return true;
}

// Login Manual ao wifi, se a conexão automática falhar
void WifiManager::LoginWifi(const char* _Wifi_Ssid, const char* _Wifi_Password, const char* _Wifi_Login_Username, const char* _Wifi_Login_Password) {
   if (_Wifi_Ssid != "") {
      Wifi_Ssid = _Wifi_Ssid;
      Wifi_Password = _Wifi_Password;
      Wifi_Login_Username = _Wifi_Login_Username; 
      Wifi_Login_Password = _Wifi_Login_Password;
   }
	while (!InitWiFi()) 
	{
		Serial.println("ERRO - Insira as credenciais do Wifi: ");
		Wifi_Ssid = Input("Nome do Wifi: ");

		if (Wifi_Ssid == "eduroam" || Wifi_Ssid == "UFPR_SEM_FIO") {
			Wifi_Login_Username = Input("Usuário de " + Wifi_Ssid + ": ");
			Wifi_Login_Password = Input("Senha do usuário " + Wifi_Login_Username + ": ");
			continue;
		}
		Wifi_Password = Input("Senha do Wifi: ");
	}
}

bool WifiManager::IsWifiConnected() {
   if (WiFi.status() != WL_CONNECTED) {
		Serial.println("Wifi desconectado. Tentando reconectar.");
		if (!InitWiFi())
			return false;
	}
   return true;
}

/*
// Conectar ao UFPR_SEM_FIO se precisar (NÃO RECOMENDADO)
void HandleCaptivePortal() { 
	HTTPClient http;
	http.begin(WIFI_PORTAL_URL);
	const uint16_t HttpCode = http.GET();

	if (HttpCode != 200) {
      Serial.println("ERRO: Sem conexão à URL de redirecionamento para o portal de login do wifi");
		Serial.println("HTTP Code: ");
	   Serial.print(HttpCode);
      http.end();
		return;
	}
   const std::string Html(http.getString().c_str());
   Serial.println(Html.c_str());
   const std::string Url_Location = "window.location=\"";
   const uint16_t Url_Location_Index = Html.find(Url_Location) + Url_Location.length();

   std::string portal_url = Html.substr(Url_Location_Index, Html.length() - Url_Location_Index);
   portal_url = portal_url.substr(0, portal_url.find("\""));
   Serial.println(portal_url.c_str());
	// region - - - - Portal - - - -
   http.end();
   HTTPClient portal_http;
	portal_http.begin(portal_url.c_str());
	const uint16_t HttpPortalCode = portal_http.GET(); 

	if (HttpPortalCode != 200) {
      Serial.println("ERRO: Sem conexão ao portal de login do wifi");
		Serial.println("HTTP Portal Code: ");
   	Serial.print(HttpPortalCode);
      portal_http.end();
		return;
	}
	portal_http.addHeader("Content-Type", "application/x-www-form-urlencoded");
	const String Magic = portal_url.substr(portal_url.find("?") + 1, portal_url.length()).c_str();
	String postData = "4Tredir=" + String("http%3A%2F%2Fwww.msftconnecttest.com%2Fredirect") + "&magic=" + Magic + "&username=" + String(UFPR_SEM_FIO_UN) + "&password=" + String(UFPR_SEM_FIO_PD);
	Serial.println("Aguardando conexão para evitar bloqueio do IP...");
	delay((10 + random(-2.0f, 2.0f)) * 1000);
	int PortalResponseCode = portal_http.POST(postData);

	Serial.printf((PortalResponseCode > 0)?"Código de resposta HTTP: %d\n" : "Erro na requisição POST. Código: %d\n", PortalResponseCode);
	portal_http.end(); 
} 
*/