/* Projeto Láb. Iot, Conexão Esp32 ao servidor Thingsboard, UFPR - Autor: João André A. da Silva */
#ifndef THINGSBOARD_MANAGER
#define THINGSBOARD_MANAGER

#include "WifiManager.h"
#include <string>
#include <time.h>
#include <Server_Side_RPC.h>
#include <Arduino_MQTT_Client.h>
#include <ThingsBoard.h>

#ifndef MAX_RPC_CALLBACKS
  #define MAX_RPC_CALLBACKS 2
#endif
String Input(String message);

class IoT_Manager {
   public:
      IoT_Manager(const char* Server, const char* Token, const uint16_t& MeasurementInterval, const std::vector<String>& Atributes, const std::array<RPC_Callback, MAX_RPC_CALLBACKS>& callbacks);
      bool IsThingsBoardConnected();
      bool NextMeasurement();
      void SendData(std::vector<float> measurement);
      void Initialize();
   private:
      bool InitTB(bool SubscribeToRPC=false);
      const char* ThingsBoard_Server;
      const char* ThingsBoard_Token;
      const uint16_t Measurement_Interval; // Segundos
      static constexpr uint8_t MAX_MESSAGE_SEND_RECEIVE_SIZE = 256U;
      #if ENCRYPTED
         static constexpr uint16_t ThingsBoard_Port = 8883U;
      #else	
         static constexpr uint16_t ThingsBoard_Port = 1883U;
      #endif
      Arduino_MQTT_Client mqttClient;
      ThingsBoard tb;
      const std::array<RPC_Callback, MAX_RPC_CALLBACKS> Callbacks;
      Server_Side_RPC<MAX_RPC_CALLBACKS, MAX_RPC_CALLBACKS> rpc;
      std::array<IAPI_Implementation*, 1> apis = { &rpc };

      std::vector<String> AtributesList;
      const uint8_t Atributes_Size;
      std::vector<std::vector<float>> data_list;

      time_t measurement_timestamp = 0; 
      unsigned long currentMillis = 0;
      unsigned long nextCycleMillis = 0;

      static constexpr const char* NTP_SERVER = "pool.ntp.org"; // Servidor para consultar tempo preciso
      static constexpr int32_t GMT = (-3 * 3600); // Fuso do GMT
      static constexpr int8_t DAYLIGHT_OFFSET = 0; // Fuso do horário de verão
};

#endif