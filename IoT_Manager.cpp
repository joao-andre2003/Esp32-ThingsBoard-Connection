/* Projeto Láb. Iot, Conexão Esp32 ao servidor Thingsboard, UFPR - Autor: João André A. da Silva */
#include "IoT_Manager.h"

String Input(String message) {
	Serial.println(message);
	while (!Serial.available());
	return Serial.readStringUntil('\n');
}

IoT_Manager::IoT_Manager(const char* Server, const char* Token, const uint16_t MeasurementInterval, const std::vector<const char*>* Atributes, const std::array<RPC_Callback, MAX_RPC_CALLBACKS>* callbacks)
	: ThingsBoard_Server(Server),
     ThingsBoard_Token(Token),
	  Measurement_Interval(MeasurementInterval),
	  AtributesList(Atributes),
	  Atributes_Size(Atributes->size()),
	  Callbacks(callbacks),
	  mqttClient(WifiManager::wifiClient),
	  tb(mqttClient, MAX_MESSAGE_SEND_RECEIVE_SIZE, MAX_MESSAGE_SEND_RECEIVE_SIZE, Default_Max_Stack_Size, apis)
{}

// Conectar ao ThingsBoard
bool IoT_Manager::InitTB(bool SubscribeToRPC) {
  	Serial.println("Conectando ao ThingsBoard - " + String(ThingsBoard_Server));
  	if (!tb.connect(ThingsBoard_Server, ThingsBoard_Token, ThingsBoard_Port)) {
		Serial.printf("ERRO - Conexão ThingsBoard - %d\n", mqttClient.connected());
    	return false;
  	}
  	if (SubscribeToRPC && Callbacks->size() > 0) { // Lidar com inscrições RPC
    	if (!rpc.RPC_Subscribe(Callbacks->cbegin(), Callbacks->cend())) {
      	Serial.println("ERRO - Falha na inscrição do método RPC.");
      	return false;
   	 }
  	  Serial.println("Inscrição RPC realizada.");
	}
  	Serial.println("< Conectado ao ThingsBoard >");
  	return true;
}

void IoT_Manager::Initialize() 
{
	InitTB(true);

	Serial.print("Estabelecendo conexão com NTP.");
	configTime(GMT, DAYLIGHT_OFFSET, NTP_SERVER);
	while (measurement_timestamp < 1000) { // 1000 porque a função time funciona de um jeito estranho antes da inicialização completa, ele recebe valores muito pequenos até pegar de fato o timestamp
		time(&measurement_timestamp);
		delay(500);
		Serial.print(".");
	}
}

bool IoT_Manager::IsThingsBoardConnected() {
   if (!tb.connected()) { // Reconectar ThingsBoard
		Serial.println("ThingsBoard desconectado. Tentando reconectar.");
		if (!InitTB())
			return false;	
	}
   return true;
}

bool IoT_Manager::NextMeasurement() {
	currentMillis = millis();
	if (currentMillis < nextCycleMillis)
		return false;
	nextCycleMillis = Measurement_Interval * 1000 + currentMillis;
	return true;
}

void IoT_Manager::SendData(std::vector<float>& measurement) 
{
	data_list.push_back(measurement);

	if (WifiManager::IsWifiConnected() == false) 
		return;
	
	time_t timestamp;
	time(&timestamp);
	nextCycleMillis = (long signed)(measurement_timestamp + Measurement_Interval * data_list.size() - timestamp) * 1000 + currentMillis; // Timer mais preciso, corrige melhor margem de erro com o tempo

	if (IsThingsBoardConnected() == false) 
		return;

	for(int i=0; i<data_list.size(); ++i) {
		const uint8_t Json_Size = JSON_OBJECT_SIZE(1) + JSON_ARRAY_SIZE(Atributes_Size);
		DynamicJsonDocument Json_Data(Json_Size);
		measurement_timestamp += (i + 1) * Measurement_Interval;
		Json_Data["ts"] = measurement_timestamp * 1000;

		JsonObject values = Json_Data.createNestedObject("values");
		for(int j=0; j<Atributes_Size; ++j) 
			values[(*AtributesList)[j]] = data_list[i][j];

		String String_Data;
		serializeJson(Json_Data, String_Data);
		Serial.print("Enviando data: ");
		Serial.println(String_Data);
		if (!tb.sendTelemetryString(String_Data.c_str()))
			Serial.println("ERRO no envio da telemetria.");
	}
	data_list.clear();
	tb.loop();
}