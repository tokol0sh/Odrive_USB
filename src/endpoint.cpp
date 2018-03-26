#include "endpoint.h"
#include "Odrive.h"

Endpoint& Endpoint::add_child(const std::string& name, std::string type, int id, std::string access) {
	children[name] = Endpoint(ODrive, id, type, access);
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
			ODrive->set_float(id, value);
			//printf("Set ID %i with float: %f\n", id, value);
		}
	}
}


void Endpoint::set(int value) const {
	if (!has_children() && is_valid()) {
		if (type == "uint32") {
			ODrive->set_int(id, value);
			//printf("Set ID %i with float: %d\n", id, value);
		}
	}
}


Endpoint::operator float() const {
	if (!has_children() && is_valid()) {
		if (type == "float") {
			float value = 0.0;
			ODrive->get_float(id, value);
			//printf("got %.2f from ID %i\n", value, id);
			return value;

		}
	}
}

Endpoint::operator int() const {
	if (!has_children() && is_valid()) {
		if (type == "uint32" | type == "int32") {
			int value = 0;
			ODrive->get_int(id, value);
			//printf("got %d from ID %i\n", value, id);
			return value;

		}
	}
}
