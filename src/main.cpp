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
	Protocol ODrive;
	Endpoint root = ODrive.get_json_interface();
	Endpoint& motor0 = root("motor0");

	while (1) {
		float bus_voltage = (float)root("vbus_voltage");
		int loop_counter = motor0("loop_counter");
	}
	//printf("Bus voltage: %.2fV\n", voltage);

	
	int loop_counter = motor0("loop_counter");
	Endpoint& motor1 = root("motor1");


	//Endpoint& motor0_position = motor0["pos_setpoint"];
	//motor0_position = float(20.0);
	//float motor_position = motor0_position;
	

	/*
	libusb_release_interface(handle, 1);
	libusb_close(handle);
	libusb_exit(ctx);
	*/
	return 0;
}


