#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <iterator>

class Endpoint {
public:

	Endpoint(int id = -1, std::string type = std::string()) : id(id), type(type){}

	Endpoint& add_child(const std::string& name, std::string type, int id) {
		children[name] = Endpoint(id, type);
		return children[name];
	}

	Endpoint& operator[] (const std::string& name) {
		return children[name];
	}

	bool is_valid() const {
		return id != -1;
	}

	bool has_children() const {
		return !children.empty();
	}


	void set(float value) const {
		if (!has_children() && is_valid()) {
			if (type == "float") {
				//odrive_set_value_float(id, value);
				//odrive_endpoint_request(handle, 1, received_payload, send_payload, 1, 4);
				printf("Set ID %i with float: %f", id, value);
			}
		}
	}

	operator float() const {
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

	template <class T>
	void operator=(const T& value) {
		set(value);
	}




private:

	int id;
	std::string type;
	std::map<std::string, Endpoint> children;

};
