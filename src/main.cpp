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

	json j;
	std::map<std::string, int> endpoints;
	get_odrive_json_interface(handle, j);

	// json magic
	parse_endpoints(j, endpoints);
	
	libusb_release_interface(handle, 1);
	libusb_close(handle);
	libusb_exit(ctx);

	return 0;
}
