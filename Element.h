#pragma once
#include <string>

// Stores RGBA colour
class Colour {
public:
	int r, g, b, a;
	Colour();
	Colour(int r, int g, int b, int a);
};

// Stores element parameters
class Element {
public:
	unsigned int id;
	std::string name;
	Colour colour;
	Element();
	Element(unsigned int _index, const std::string _name, Colour _colour);
};