#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <iterator>


class Endpoint {
public:

	Endpoint(int id = -1, std::string type = std::string()) : id(id), type(type){}

	Endpoint& add_child(const std::string& name, std::string type, int id);

	Endpoint& operator[] (const std::string& name);

	bool is_valid() const;

	bool has_children() const;

	void set(float value) const;

	operator float() const;

	template <class T>
	void operator=(const T& value) {
		set(value);
	}


private:

	int id;
	std::string type;
	std::map<std::string, Endpoint> children;

};
