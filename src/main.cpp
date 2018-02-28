#include <iostream>
#include "libusb-1.0\libusb.h"
#include <string>
#include <vector>

#define VID     0x1209
#define PID     0x0d31

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(*x))


struct odrive_packet_t {
	short sequence_number;
	short endpoint_id;
	short payload_length;
	std::string payload;
	short CRC16;
};

int send_to_odrive(libusb_device_handle* handle, unsigned char* packet, int* sent_bytes) {
	libusb_bulk_transfer(handle,
		(1 | LIBUSB_ENDPOINT_OUT),
		packet,
		ARRAY_SIZE(packet),
		sent_bytes,
		0);
}

int receive_from_odrive(libusb_device_handle* handle, unsigned char* packet, int max_bytes_to_receive, int* received_bytes, int timeout = 0) {
	libusb_bulk_transfer(handle,
		(1 | LIBUSB_ENDPOINT_IN),
		packet,
		max_bytes_to_receive,
		received_bytes,
		timeout);
}




odrive_packet_t remote_endpoint_operation(int endpoint_id, std::string payload, int ack, int length) {
	odrive_packet_t packet;

	// Assemble the packet to send
	if (ack) {
		packet.endpoint_id |= 0x8000;
	}
	packet.sequence_number = (packet.sequence_number + 1) & 0x7fff;
	packet.sequence_number |= 0x80;
	packet.payload = payload;
	packet.payload_length = length;
	if (endpoint_id & 0x7fff == 0) {
		packet.CRC16 = 1;
	}
	else {
		packet.CRC16 = 0;
	}

	// Send the packet
	send_to_odrive(handle, packet, sent_bytes);
	// Immediatly wait for response from Odrive and check if ack (if we asked for one)
	receive_from_odrive(handle, packet, max_bytes_to_receive, received_bytes, timeout)
	// return the response payload


}


int main() {
	printf("Hello world!\n");

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

	int actual_send = 0;
	int actual_receive = 0;
	unsigned char test[] = { 0, 1, 2, 3 };

	int chunk_length;


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
	printf("Received %i bytes!\n", actual_receive);
	printf("%s\n", chunk_buffer);


	libusb_release_interface(handle, 1);
	libusb_close(handle);
	libusb_exit(ctx);

	return 0;
}
