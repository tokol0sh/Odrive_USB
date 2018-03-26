// This class holds ODrive endpoints (or any endpoint really) in a std::map  
// which contains the endpoint name and endpoint id. Each endpoint can also 
// have a child (root has child, 'motor0'. 'motor0' has child, 'pos_setpoint').
// This class also implements the abstraction from the ODrive class, essentially
// providing a direct interface from an Endpoint to a endpoint on the ODrive hardware.

#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <iterator>

class ODrive;

class Endpoint {
public:

	Endpoint(ODrive* Odrive = NULL, int id = -1, std::string type = std::string(), std::string access = std::string()) : ODrive(Odrive), id(id), type(type), access(access){}

	Endpoint& add_child(const std::string& name, std::string type, int id, std::string access);
	Endpoint& operator() (const std::string& name);

	bool is_valid() const;
	bool has_children() const;

	void set(float value) const;
	void set(int value) const;
	explicit operator float() const;
	explicit operator int() const;

	template <class T>
	void operator=(const T& value) {
		set(value);
	}

private:

	ODrive* ODrive;
	int id;
	std::string type;
	std::string access;
	std::map<std::string, Endpoint> children;

};
