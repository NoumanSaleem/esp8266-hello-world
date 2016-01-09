#include <osapi.h>
#include <os_type.h>
#include "driver/uart.h"

LOCAL void initDone();

LOCAL void initDone() {
	os_printf("Initialized! \n");
}

void user_rf_pre_init(void) {
}

void user_init(void) {
	uart_init(115200, 115200);
	system_init_done_cb(initDone);
}