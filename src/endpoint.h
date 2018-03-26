#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <iterator>

class Protocol;

class Endpoint {
public:

	Endpoint(Protocol* Odrive = NULL, int id = -1, std::string type = std::string()) : ODrive(Odrive), id(id), type(type){}

	Endpoint& add_child(const std::string& name, std::string type, int id);

	Endpoint& operator() (const std::string& name);

	bool is_valid() const;

	bool has_children() const;

	void set(float value) const;
	void set(int value) const;

	float get_float() const;
	int get_int() const;

	template <class T>
	void operator=(const T& value) {
		set(value);
	}


	explicit operator float() const;

	explicit operator int() const;




private:
	Protocol* ODrive;
	int id;
	std::string type;
	std::map<std::string, Endpoint> children;

};
