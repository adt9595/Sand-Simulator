#include <string>
#include "Element.h"

Colour::Colour(int r, int g, int b, int a) :r(r), g(g), b(b), a(a) {};
Colour::Colour() {
	Colour::r = 0; 
	Colour::g = 0; 
	Colour::b = 0; 
	Colour::a = 0;
};

Element::Element(unsigned int index, const std::string name, Colour colour) : id(index), name(name), colour(colour) {}
Element::Element() {
	id = 999;
	name = "null";
	Colour* colour = new Colour(255, 255, 255, 255);
}
