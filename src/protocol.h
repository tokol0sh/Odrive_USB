#include <cstdio>
#include <vector>
#include <cstdint>

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

	serialize(data, seq_no, endpoint, (short)response_size);

	for (uint8_t b : payload) {
		data.push_back(b);
	}

	serialize(data, (short)(endpoint == 0 ? 0 : 1));

	return data;
}
