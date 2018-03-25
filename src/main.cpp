#include <iostream>
#include "libusb-1.0\libusb.h"
#include <string>
#include <vector>
#include <iterator>
#include <json.hpp>
#include "endpoint.h"
#include "ODrive.h"

using nlohmann::json;

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(*x))


int main() {
	/*
	libusb_context* ctx;
	libusb_init(&ctx);
	//libusb_set_debug(ctx, 3);
	libusb_device_handle* handle;
	handle = libusb_open_device_with_vid_pid(ctx, VID, PID);
	int r = libusb_claim_interface(handle, 1);
	*/

	Protocol ODrive;
	Endpoint root(0);
	root = ODrive.get_json_interface();

	Endpoint& bus_voltage = root["vbus_voltage"];
	float voltage = bus_voltage;

	Endpoint& motor0 = root["motor0"];
	Endpoint& motor1 = root["motor1"];

	Endpoint& motor0_position = motor0["pos_setpoint"];
	motor0_position = float(20.0);
	float motor_position = motor0_position;
	


	/*
	libusb_release_interface(handle, 1);
	libusb_close(handle);
	libusb_exit(ctx);
	*/
	return 0;
}


