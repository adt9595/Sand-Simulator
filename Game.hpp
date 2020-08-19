#ifndef Game_hpp
#define Game_hpp
#define SDL_MAIN_HANDLED

#include "SDL.h"
#include "SDL_ttf.h"
#include <iostream>
#include <vector>
#include <iterator>
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include "Element.h"

constexpr auto WIDTH = 750;
constexpr auto HEIGHT = 540;

typedef std::vector<int> int_vector;

template <typename T> int sgn(T val) {
	return (T(0) < val) - (val < T(0));
}

struct Vector2Int {
	int x;
	int y;
};

class Game {

public:
	Game();
	~Game();

	void init(const char* title, int xPos, int yPos, int width, int height, bool fullscreen);
	
	void handleEvents();
	void update();
	void render();
	void clean();
	void initialiseText();
	void placeElement(int mouseX, int mouseY);
	void placeElement(int mouseX, int mouseY, int elementIndex);
	void setElement(int scaledIndex);
	void changeCurrentElement(int elementIndex);
	void changeBrushSize(int deltaSize);
	void clearScreen();
	void resetBrushPoints();
	static std::map<int, Element> elementDict;
	static std::string getCurrentElementName();

	int convertTo1D(int x, int y, int width);
	Vector2Int convertTo2D(int index, int width);
	int manhattanDistance(int x1, int y1, int x2, int y2);
	void drawLine(int x1, int y1, int x2, int y2);

	bool running() { return isRunning; }
	bool moveDownAvailable(int pixelIndex);
	bool erasing = false;
	int availableMove(int pixelIndex);
	bool moveSidewaysAvailable(int pixelIndex);
	bool moveUpAvailable(int pixelIndex);
	void movePixel(int sourceIndex, int destinationIndex, int elementID);

	//Vector2Int getLastElementPositionIndex();
	//void setLastElementPositionIndex(int x, int y);

	static SDL_Event event;

private:
	int count = 0;
	int brushSize = 0;
	int numElements;
	static float scaleFactorW;
	static float scaleFactorH;
	bool isRunning = false;
	bool firstFrame = true;
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Texture* texture;
	// Text elements
	SDL_Color textColour;
	SDL_Surface* textSurfaceTopLeft;
	SDL_Surface* textSurfaceTopRight;
	SDL_Texture* textTextureTopLeft;
	SDL_Texture* textTextureTopRight;
	SDL_Rect textRectTopLeft;
	SDL_Rect textRectTopRight;
	TTF_Font* mainFont;

	Colour yellow;
	Colour blue;
	Colour clear;
	Colour grey;
	std::vector<Element> elements;
	std::vector<Vector2Int> lastBrushPointIndices;
	static Element* currentElement;
	static int currentElementIndex;
	Vector2Int lastElementPositionIndex;
	void updateCurrentElementName();
	void checkSurroundingsForElement(int x, int y, Element* element);

	const static int texWidth = 250;
	const static int texHeight = 180;
	std::vector<unsigned char> pixelArray = std::vector<unsigned char>(texWidth * texHeight * 4, 0);
	static std::set<int> filledPixels;
	static unsigned int* elementArray;
	static unsigned int* elementArrayFuture;
};


#endif Game_hpp
