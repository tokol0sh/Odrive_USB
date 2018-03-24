#include <iostream>
#include "libusb-1.0\libusb.h"
#include <string>
#include <vector>
#include "protocol.h"
#include <iterator>
#include <json.hpp>
#include "endpoint.h"
#include "ODrive.h"

using nlohmann::json;

#define VID     0x1209
#define PID     0x0D32


#define ARRAY_SIZE(x) (sizeof(x) / sizeof(*x))






int main() {
	libusb_context* ctx;
	libusb_init(&ctx);
	//libusb_set_debug(ctx, 3);
	libusb_device_handle* handle;
	handle = libusb_open_device_with_vid_pid(ctx, VID, PID);
	int r = libusb_claim_interface(handle, 1);
	json j;
	Endpoint root(0);
	Protocol ODrive(handle);


	ODrive.get_json_interface(root);

	Endpoint& motor0 = root["motor0"];
	Endpoint& motor0_position = motor0["pos_setpoint"];
	motor0_position = 20;
	float motor_position = motor0_position;




	serial_buffer received_payload;
	serial_buffer send_payload;
	serialize(send_payload, (int)0);
	while (1) {
		ODrive.endpoint_request(1, received_payload, send_payload, 1, 4);
		float value = 0;

		serial_buffer::iterator it = received_payload.begin();
		deserialize(it, value);
		printf("Bus Voltage: %.5f\n", value);

	}
	odrive_endpoint_request(handle, 10, received_payload, send_payload, 1, 4);
	int value1 = 0;

	serial_buffer::iterator it1 = received_payload.begin();
	deserialize(it1, value1);
	printf("Motor 0 error code: %d\n", value1);


	//Endpoint root(0);
	parse_endpoints2(j, root);
	



	
	libusb_release_interface(handle, 1);
	libusb_close(handle);
	libusb_exit(ctx);

	return 0;
}


