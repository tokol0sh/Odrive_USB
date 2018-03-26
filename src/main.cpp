#include <iostream>
#include "libusb-1.0\libusb.h"
#include <string>
#include <vector>
#include <iterator>
#include <json.hpp>
#include "ODrive.h"
#include "endpoint.h"

using nlohmann::json;

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(*x))

int main() {
	ODrive ODrive;
	Endpoint root = ODrive.get_json_interface();
	Endpoint& motor0 = root("motor0");

	while (1) {
		float bus_voltage = (float)root("vbus_voltage");
		int loop_counter = (int)motor0("loop_counter");
		float phase_resistance = (float)motor0("phase_resistance");
		int motor0_status = (int)motor0("error");
		float motor_position = (float)motor0("pos_setpoint");


		Sleep(1000);
		printf("Bus voltage: %.2fV\n", bus_voltage);
		motor0("pos_setpoint") = (float)1600.0;
		motor_position = (float)motor0("pos_setpoint");
		printf("Motor0 position: %.2f\n", motor_position);
		Sleep(1000);
		motor0("pos_setpoint") = (float)0.0;
		motor_position = (float)motor0("pos_setpoint");
		printf("Motor0 position: %.2f\n", motor_position);
		
	}
	
	
	int loop_counter = (int)motor0("loop_counter");
	Endpoint& motor1 = root("motor1");


	return 0;
}


