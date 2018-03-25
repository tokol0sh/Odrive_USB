#include "endpoint.h"

Endpoint& Endpoint::add_child(const std::string& name, std::string type, int id) {
	children[name] = Endpoint(id, type);
	return children[name];
}

Endpoint& Endpoint::operator[] (const std::string& name) {
	return children[name];
}

bool Endpoint::is_valid() const {
	return id != -1;
}

bool Endpoint::has_children() const {
	return !children.empty();
}

void Endpoint::set(float value) const {
	if (!has_children() && is_valid()) {
		if (type == "float") {
			//odrive_set_value_float(id, value);
			//odrive_endpoint_request(handle, 1, received_payload, send_payload, 1, 4);
			printf("Set ID %i with float: %f\n", id, value);
		}
	}
}

Endpoint::operator float() const {
	if (!has_children() && is_valid()) {
		if (type == "float") {
			//odrive_set_value_float(id, value);
			//odrive_endpoint_request(handle, 1, received_payload, send_payload, 1, 4);
			float value = 69.696969;
			printf("got %.2f from ID %i\n", value, id);
			return value;

		}
	}
}