/* Projeto Láb. Iot, Conexão Esp32 ao servidor Thingsboard, UFPR - Autor: João André A. da Silva */
#define ENCRYPTED false
#include "WifiManager.h"
#include "IoT_Manager.h"

#define THINGSBOARD_SERVER "bellatrix.eletrica.ufpr.br"
#define THINGSBOARD_TOKEN "98pf2pi8h7xvi6mgjmgd"
#define MEASUREMENT_INTERVAL 10

#define WIFI_SSID "eduroam"
#define WIFI_PASSWORD ""
#define WIFI_LOGIN_USERNAME "joao.andre"
#define WIFI_LOGIN_PASSWORD "" // Deixe vazio para fazer login quando inciar o código

void DoSomething(const JsonVariantConst &data, JsonDocument &response) {
   Serial.println("Executando código solicitado via método RPC.");

	// ... Executar código desejável
   //const float example_data = data["SomeServerData"];
   //response["bool"] = true;
}

const std::array<RPC_Callback, MAX_RPC_CALLBACKS> CALLBACKS = { 
	//RPC_Callback { "DoSomething", DoSomething }, // Nome x em string enviada do ThingsBoard que vai chamar a função y no ESP32
	//RPC_Callback { "AnotherThing", AnotherThing } // Coloque mais métodos RPC se necessário.
};

// Seus atributos para enviar seus respectivos valores. Os atributos do dispositivo no ThingsBoard devem estar com o mesmo nome
std::vector<String> DEVICE_ATRIBUTES = {
	"temperature",
	"humidity"
};

IoT_Manager device(THINGSBOARD_SERVER, THINGSBOARD_TOKEN, MEASUREMENT_INTERVAL, DEVICE_ATRIBUTES, CALLBACKS);

void setup() {
	Serial.begin(115200);
	delay(1000); // Tempo para iniciar o Serial
	WifiManager::LoginWifi(WIFI_SSID, WIFI_PASSWORD, WIFI_LOGIN_USERNAME, WIFI_LOGIN_PASSWORD);
	device.Initialize();
}

void loop()
{
	if (device.NextMeasurement()) {
		Serial.println("Novo ciclo.");
		
		// Código de medição
		std::vector<float> medicao = { random(5.0f, 20.0f), random(40.0f, 70.0f) }; // temperatura e humidade respectivamente. Devem ser na mesma ordem que DEVICE_ATRIBUTES
		device.SendData(medicao);
	}
}