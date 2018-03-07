#include <cstdio>
#include <vector>
#include <cstdint>
#include "libusb-1.0\libusb.h"

typedef std::vector<uint8_t> serial_buffer;


// Templates for basic serialization and deserialization
template <class T>
void serialize(serial_buffer&, const T&);

template <class T>
void deserialize(serial_buffer::iterator&, T&);



void serialize(serial_buffer& serial_buffer, const int& value) {
	serial_buffer.push_back((value >> 0) & 0xFF);
	serial_buffer.push_back((value >> 8) & 0xFF);
	serial_buffer.push_back((value >> 16) & 0xFF);
	serial_buffer.push_back((value >> 24) & 0xFF);
}

void serialize(serial_buffer& serial_buffer, const short& value) {
	serial_buffer.push_back((value >> 0) & 0xFF);
	serial_buffer.push_back((value >> 8) & 0xFF);

}

void serialize(serial_buffer& serial_buffer, const std::vector<uint8_t>& value) {
	serial_buffer = value;

}


void deserialize(serial_buffer::iterator& it, short& value) {
	value = *it++;
	value |= (*it++) << 8;
}

void deserialize(serial_buffer::iterator& it, int& value) {
	value = *it++;
	value |= (*it++) << 8;
	value |= (*it++) << 16;
	value |= (*it++) << 24;
}

void deserialize(serial_buffer::iterator& it, std::vector<uint8_t>& value) {
	value.push_back(1);
}


struct odrive_packet {
	short sequence_number;
	short endpoint_id;
	short payload_length;
	std::vector<uint8_t> payload;
	short CRC16;
};



// Variadic template to allow multiple arguments
template <class T, class U, class ...Us>
void serialize(serial_buffer& serial_buffer, const T& t, const U& u, const Us&... us) {
	serialize(serial_buffer, t);
	serialize(serial_buffer, u, us...);
}


template <class T, class U, class ...Us>
void deserialize(serial_buffer::iterator& it, T& t, U& u, Us&... us) {
	deserialize(it, t);
	deserialize(it, u, us...);
}



// Struct serialization
void serialize(serial_buffer& serial_buffer, const odrive_packet& odrive_packet) {
	serialize(serial_buffer, odrive_packet.sequence_number, odrive_packet.endpoint_id, odrive_packet.payload_length, odrive_packet.payload, odrive_packet.CRC16);
}


void deserialize(serial_buffer::iterator& it, odrive_packet& odrive_packet) {
	deserialize(it, odrive_packet.sequence_number, odrive_packet.endpoint_id, odrive_packet.payload_length, odrive_packet.payload, odrive_packet.CRC16);
}



template <class ...Ts>
serial_buffer create_odrive_packet(short seq_no, short endpoint, short response_size, const Ts&... ts) {
	serial_buffer payload;
	serialize(payload, ts...);


	serial_buffer data;

	serialize(data, (short)seq_no, (short)endpoint, (short)response_size);

	for (uint8_t b : payload) {
		data.push_back(b);
	}

	serialize(data, (short)(endpoint == 0 ? 0 : 1));

	return data;
}


serial_buffer decode_odrive_packet(serial_buffer::iterator& it, short& seq_no, serial_buffer& received_packet) {
	serial_buffer payload;
	deserialize(it, seq_no);

	// add this to a template?
	while (it != received_packet.end()) {
		payload.push_back(*it++);
	}
	return payload;
}



void send_to_odrive(libusb_device_handle* handle, std::vector<uint8_t>& packet, int* sent_bytes) {
	libusb_bulk_transfer(handle,
		(1 | LIBUSB_ENDPOINT_OUT),
		packet.data(),
		packet.size(),
		sent_bytes,
		0);
}

void receive_from_odrive(libusb_device_handle* handle, unsigned char* packet, int max_bytes_to_receive, int* received_bytes, int timeout = 0) {
	libusb_bulk_transfer(handle,
		(1 | LIBUSB_ENDPOINT_IN),
		packet,
		max_bytes_to_receive,
		received_bytes,
		timeout);
}



int odrive_endpoint_request(libusb_device_handle* handle, int endpoint_id, serial_buffer& received_payload, int count, int ack, int length) {
	serial_buffer send_buffer;
	serial_buffer receive_buffer;
	unsigned char receive_bytes[100] = { 0 };
	int sent_bytes = 0;
	int received_bytes = 0;
	int max_bytes_to_receive = 64;
	int timeout = 1000;
	int i = 0;
	static short seq_no = 0;
	short received_seq_no = 0;

	seq_no = (seq_no + 1) & 0x7fff;
	seq_no |= 0x80;
	// Send the packet
	send_to_odrive(handle, create_odrive_packet(seq_no, endpoint_id | 0x8000, (short)64, (int)count), &sent_bytes);


	// Immediatly wait for response from Odrive and check if ack (if we asked for one)
	receive_from_odrive(handle, receive_bytes, max_bytes_to_receive, &received_bytes, timeout);

	//printf("%s\n", receive_bytes);

	for (i = 0; i < received_bytes; i++) {
		receive_buffer.push_back(receive_bytes[i]);
	}

	serial_buffer::iterator it = receive_buffer.begin();
	received_payload = decode_odrive_packet(it, received_seq_no, receive_buffer);

	// return the response payload
	return received_payload.size();
}