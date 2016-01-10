#include <osapi.h>
#include <os_type.h>
#include <user_interface.h>
#include <espconn.h>
#include "driver/uart.h"

LOCAL struct espconn conn1;
LOCAL void initDone();
LOCAL void setupUDP();
LOCAL void wifiEventHandler(System_Event_t *event);
LOCAL void recvCB(void *arg, char *pData, unsigned short len);

LOCAL void initDone() {
  os_printf("Initialized! \n");

  struct station_config config;
  strncpy(config.ssid, SSID, 32);
  strncpy(config.password, SSID_PASSWORD, 64);

  wifi_set_opmode_current(STATION_MODE);
  wifi_station_set_config_current(&config);
  wifi_station_connect();
}

LOCAL void recvCB(void *arg, char *pData, unsigned short len) {
  os_printf("Received data!\n");
}

LOCAL void setupUDP() {
  esp_udp udp;
  udp.local_port = 25867;

  conn1.type = ESPCONN_UDP;
  conn1.state = ESPCONN_NONE;
  conn1.proto.udp = &udp;

  espconn_create(&conn1);
  espconn_regist_recvcb(&conn1, recvCB);
  os_printf("Listening for data\n");
}

LOCAL void wifiEventHandler(System_Event_t *event) {
  switch (event->event) {
    case EVENT_STAMODE_GOT_IP:
      os_printf("IP: %d.%d.%d.%d\n", IP2STR(&event->event_info.got_ip.ip));
      setupUDP();
    break;
    default:
      os_printf("WiFi Event: %d\n", event->event);
  }
}

void user_rf_pre_init(void) {
}

void user_init(void) {
 uart_div_modify(0, UART_CLK_FREQ / BIT_RATE_115200);

 system_init_done_cb(initDone);
 wifi_set_event_handler_cb(wifiEventHandler);
}
