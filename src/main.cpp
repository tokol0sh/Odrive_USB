#include <iostream>
#include "libusb-1.0\libusb.h"
#include <string>
#include <vector>
#include "protocol.h"

#define VID     0x1209
#define PID     0x0d31

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(*x))


void send_to_odrive(libusb_device_handle* handle, std::vector<uint8_t>& packet, int* sent_bytes) {
	libusb_bulk_transfer(handle,
		(1 | LIBUSB_ENDPOINT_OUT),
		packet.data(),
		packet.size(),
		sent_bytes,
		0);
}

void receive_from_odrive(libusb_device_handle* handle, std::vector<uint8_t>& packet, int max_bytes_to_receive, int* received_bytes, int timeout = 0) {
	libusb_bulk_transfer(handle,
		(1 | LIBUSB_ENDPOINT_IN),
		packet.data(),
		max_bytes_to_receive,
		received_bytes,
		timeout);
}



void odrive_endpoint_request(libusb_device_handle* handle, int endpoint_id, serial_buffer payload, int ack, int length) {
	serial_buffer buffer;
	int sent_bytes = 0;
	int received_bytes = 0;
	int max_bytes_to_receive = 64;
	int timeout = 1000;
	int test = 0;

	buffer = create_odrive_packet(129, endpoint_id | 0x8000, (short)64, (int)0);

	// Send the packet
	send_to_odrive(handle, buffer, &sent_bytes);
	// Immediatly wait for response from Odrive and check if ack (if we asked for one)
	receive_from_odrive(handle, buffer, max_bytes_to_receive, &received_bytes, timeout);
	// return the response payload
	printf("%s\n", buffer.data());
	test = 1;
}


int main() {
	libusb_context* ctx;
	libusb_init(&ctx);
	libusb_set_debug(ctx, 3);


	libusb_device_handle* handle;
	handle = libusb_open_device_with_vid_pid(ctx, VID, PID);

	int r = libusb_claim_interface(handle, 1);

	unsigned char chunk_buffer[64] = { 0 };
	unsigned char buffer[100] = { 0 };
	unsigned char message[] = { 129, 0,    // Seqno
								0, 128,    // Endpoint
								64, 0,   // Response size
								0, 0, 0, 0,    // Payload
								1, 0 };  // Protocol version / CRC16

	serial_buffer payload;

	int actual_send = 0;
	int actual_receive = 0;
	unsigned char test[] = { 0, 1, 2, 3 };

	int chunk_length;

	odrive_endpoint_request(handle, 0, payload, 1, 64);
	/*
	chunk_length = 64;
	libusb_bulk_transfer(handle,
		(1 | LIBUSB_ENDPOINT_OUT),
		message,
		ARRAY_SIZE(message),
		&actual_send,
		0);
		
	libusb_bulk_transfer(handle,
		(1 | LIBUSB_ENDPOINT_IN),
		chunk_buffer,
		64,
		&actual_receive,
		1000);

	*/


	printf("Received %i bytes!\n", actual_receive);
	printf("%s\n", chunk_buffer);


	libusb_release_interface(handle, 1);
	libusb_close(handle);
	libusb_exit(ctx);

	return 0;
}
