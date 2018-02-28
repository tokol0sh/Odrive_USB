#include <cstdio>
#include <cassert>
#include <libusb.h>

#define VID     0x1209
#define PID     0x0d31

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(*x))

int main() {
	printf("Hello world!\n");

	libusb_context* ctx;
	libusb_init(&ctx);
	libusb_set_debug(ctx, 4);

	assert(ctx && "Error creating context");

	libusb_device_handle* handle;
	handle = libusb_open_device_with_vid_pid(ctx, VID, PID);

	assert(handle && "Error grabbing device handle");

	if (libusb_kernel_driver_active(handle, 0)) {
		assert(libusb_detach_kernel_driver(handle, 0) == 0 && "Error detaching\
        kernel driver");
	}

	int r = libusb_claim_interface(handle, 1);
	assert(r == 0 && "Error claiming interface");

	unsigned char buffer[100] = { 0 };
	unsigned char message[] = { 0, 0,    // Seqno
		0, 0,    // Endpoint
		32, 0,   // Response size
		0, 0,    // Payload
		1, 0 };  // Protocol version

	int actual = 0;

	libusb_bulk_transfer(handle,
		(1 | LIBUSB_ENDPOINT_OUT),
		message,
		ARRAY_SIZE(message),
		&actual,
		0);

	assert(actual == ARRAY_SIZE(message) && "Error transmitting data");

	libusb_bulk_transfer(handle,
		(1 | LIBUSB_ENDPOINT_IN),
		buffer,
		32,
		&actual,
		0);

	printf("Received %i bytes!\n", actual);
	printf("%s\n", buffer);



	libusb_release_interface(handle, 1);
	libusb_close(handle);
	libusb_exit(ctx);

	return 0;
}