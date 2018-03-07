#include <iostream>
#include "libusb-1.0\libusb.h"
#include <string>
#include <vector>
#include "protocol.h"
#include <iterator>
#include <json.hpp>

using nlohmann::json;
#define VID     0x1209
#define PID     0x0d31


#define ARRAY_SIZE(x) (sizeof(x) / sizeof(*x))


int main() {

	

	libusb_context* ctx;
	libusb_init(&ctx);
	//libusb_set_debug(ctx, 3);

	libusb_device_handle* handle;
	handle = libusb_open_device_with_vid_pid(ctx, VID, PID);
	int r = libusb_claim_interface(handle, 1);

	serial_buffer receive_payload;
	serial_buffer received_message;
	int received_bytes = 128;
	int total_received = 0;
	json j;

	while (received_bytes > 0) {
		received_bytes = odrive_endpoint_request(handle, 0, receive_payload, total_received, 1, 64);
		total_received += received_bytes;
		for (uint8_t byte : receive_payload) {
			received_message.push_back(byte);
		}
	}

	// json magic
	j = json::parse(received_message);
	//j[]
	std::cout << std::setw(10) << j << "\n\n";
	printf("\n Received %i bytes!\n", total_received);


	libusb_release_interface(handle, 1);
	libusb_close(handle);
	libusb_exit(ctx);

	return 0;
}
