#include "endpoint.h"
#include "Odrive.h"

Endpoint& Endpoint::add_child(const std::string& name, std::string type, int id) {
	children[name] = Endpoint(ODrive, id, type);
	return children[name];
}

Endpoint& Endpoint::operator() (const std::string& name) {
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
			//odrive.set(id, value);
			ODrive->set_float(id, value);
			printf("Set ID %i with float: %f\n", id, value);
		}
	}
}


void Endpoint::set(int value) const {
	if (!has_children() && is_valid()) {
		if (type == "uint32") {
			//odrive.set(id, value);
			ODrive->set_int(id, value);
			printf("Set ID %i with float: %d\n", id, value);
		}
	}
}



float Endpoint::get_float() const {
	if (!has_children() && is_valid()) {
		if (type == "float") {
			float value = 0;
			ODrive->get_float(id, value);
			printf("got %.2f from ID %i\n", value, id);
			return value;
		}
	}
}

int Endpoint::get_int() const {
	if (!has_children() && is_valid()) {
		if (type == "uint32") {
			int value = 0;
			ODrive->get_int(id, value);
			printf("got %.2f from ID %i\n", value, id);
			return value;
		}
	}
}



Endpoint::operator float() const {
	if (!has_children() && is_valid()) {
		if (type == "float") {
			//odrive_set_value_float(id, value);
			//odrive_endpoint_request(handle, 1, received_payload, send_payload, 1, 4);
			float value = 69.696969;
			ODrive->get_float(id, value);
			printf("got %.2f from ID %i\n", value, id);
			return value;

		}
	}
}

Endpoint::operator int() const {
	if (!has_children() && is_valid()) {
		if (type == "uint32") {
			//odrive_set_value_float(id, value);
			//odrive_endpoint_request(handle, 1, received_payload, send_payload, 1, 4);
			int value = 69;
			ODrive->get_int(id, value);
			printf("got %.2f from ID %i\n", value, id);
			return value;

		}
	}
}
