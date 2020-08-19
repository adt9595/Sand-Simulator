#include "Game.hpp"

SDL_Event Game::event;

unsigned int* Game::elementArray = new unsigned int[texWidth * texHeight];
unsigned int* Game::elementArrayFuture = new unsigned int[texWidth * texHeight]; // Double buffer
std::set<int> Game::filledPixels{};

float Game::scaleFactorW = WIDTH / texWidth;
float Game::scaleFactorH = HEIGHT / texHeight;

Element* Game::currentElement = new Element(1, "Sand", Colour::Colour(240, 220, 0, 255));
int Game::currentElementIndex = 1;
std::map<int, Element> Game::elementDict;

Game::Game() {
	// Define colours
	Colour clear = Colour::Colour(0, 0, 0, 0);
	Colour yellow = Colour::Colour(240,220,0,255);
	Colour blue = Colour::Colour(20,50,255,255);
	Colour grey = Colour::Colour(170,170,170,255);
	Colour darkGrey = Colour::Colour(90,90,90,255);
	//Colour green = Colour::Colour(70,230,70,255);

	// Define elements
	Element* air = new Element(0, "Air", clear);
	Element* sand = new Element(1, "Sand", yellow);
	Element* water = new Element(2, "Water", blue);
	Element* rock = new Element(3, "Rock", grey);
	Element* gas = new Element(4, "Gas", darkGrey);
	//Element* grass = new Element(5, "Grass", green);

	Element* currentElement = sand;
	int currentElementIndex = sand->id;

	elements = { *air,*sand,*water,*rock, *gas};
	numElements = elements.size();

	for (auto& element : elements) {
		elementDict[element.id] = element;
	}
	
	// Initialise element arrays
	std::fill_n(elementArray, texWidth * texHeight, 0);
	std::fill_n(elementArrayFuture, texWidth * texHeight, 0);

}

Game::~Game() {
	delete[] elementArray;
	delete[] elementArrayFuture;
}

void Game::init(const char* title, int xPos, int yPos, int width, int height, bool fullscreen) {
	int flags = 0;
	
	if (fullscreen) {
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		std::cout << "Subsystems initialised" << std::endl;

		window = SDL_CreateWindow(title,  xPos, xPos, width,height,flags);
		if (window) {
			std::cout << "Window created successfully" << std::endl;
		}

		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		if (renderer) {
			SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
			std::cout << "Renderer created successfully" << std::endl;
		}

		texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, texWidth, texHeight);
		if (texture) {
			std::cout << "Texture created successfully" << std::endl;
		}
		isRunning = true;

		// Text
		TTF_Init();
		initialiseText();

		
	}
	else {
		isRunning = false;
	}

}

void Game::handleEvents() {
	
	SDL_PollEvent(&event);

	switch (event.type) {
		case SDL_QUIT:
			isRunning = false;
			break;
		default:
			break;
	}
}

void Game::update() {
	// Update positions
	for (int i = 0;i < texWidth * texHeight;i++) {
		int currentPixelElement = elementArray[i];
		switch (currentPixelElement) {
			case 1:
				if (!moveDownAvailable(i)) {
					elementArrayFuture[i] = currentPixelElement;
				}
				break;
			case 2:
				if (moveDownAvailable(i)) continue;
				else if (moveSidewaysAvailable(i)) continue;
				else {
					elementArrayFuture[i] = currentPixelElement;
				}
				break;
			case 3:
				elementArrayFuture[i] = currentPixelElement;
				break;
			case 4:
				if (moveUpAvailable(i)) continue;
				else if (i < texWidth) {
					elementArrayFuture[i] = 0;
				}
				else if (moveSidewaysAvailable(i)) continue;
				else {
					elementArrayFuture[i] = currentPixelElement;
				}	
				break;
			/*case 5:
				Vector2Int currentPoint = convertTo2D(i,texWidth);
				for (int x = currentPoint.x - 1;x <= currentPoint.x + 1;x++) {
					for (int y = currentPoint.y - 1; y <= currentPoint.y + 1;y++) {
						
					}
				}*/


			default:
				break;
		}
	}
}

void Game::render() {
	// Setup elements arrays
	if (!firstFrame) {
		for (int i = 0;i < texWidth * texHeight;i++) {
			elementArray[i] = elementArrayFuture[i];
		}
		//std::copy(std::begin(*elementArrayFuture), std::end(elementArrayFuture), std::begin(elementArray));
		//memcpy(&elementArray, &elementArrayFuture, sizeof(elementArray));
		std::fill_n(elementArrayFuture, texWidth * texHeight, 0);
		if (&filledPixels != NULL) {
			filledPixels.clear();
		}
		
	}
	else { firstFrame = false; }

	// Update pixel array
	int scaledIndex = 0;
	for (unsigned int i = 0; i < 4 * texWidth * texHeight; i += 4)
	{
		pixelArray[i + 0] = (elementArray[scaledIndex] == 0) ? 40 : elementDict[elementArray[scaledIndex]].colour.b;        // b
		pixelArray[i + 1] = (elementArray[scaledIndex] == 0) ? 40 : elementDict[elementArray[scaledIndex]].colour.g;        // g
		pixelArray[i + 2] = (elementArray[scaledIndex] == 0) ? 40 : elementDict[elementArray[scaledIndex]].colour.r;        // r
		pixelArray[i + 3] = 255;    // a

		scaledIndex += 1;
	}

	SDL_UpdateTexture(texture, NULL, pixelArray.data(), texWidth * 4);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	if (renderer == NULL) {
		std::cout << "No renderer to blit to" << std::endl;
	}
	else {
		SDL_RenderCopy(renderer, textTextureTopLeft, NULL, &textRectTopLeft);
	}
	SDL_RenderCopy(renderer, textTextureTopRight, NULL, &textRectTopRight);
	SDL_RenderPresent(renderer);


}

void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	TTF_CloseFont(mainFont);
	TTF_Quit();
	SDL_Quit();
	std::cout << "Cleanup Complete" << std::endl;
}

void Game::checkSurroundingsForElement(int x, int y, Element* element) {

}

// Initialise SDL_TTF and set text parameters
void Game::initialiseText()
{
	textColour = { 255,255,255 };
	mainFont = TTF_OpenFont("Roboto-Thin.ttf", 25);
	
	textSurfaceTopLeft = TTF_RenderText_Blended_Wrapped(mainFont, "0-4: Change element\no/p: Change brush size\nc: Clear screen", textColour,300);
	textSurfaceTopRight = TTF_RenderText_Blended_Wrapped(mainFont, (currentElement->name+"\n"+std::to_string(brushSize+1)).c_str(), textColour, 300);
	textTextureTopLeft = SDL_CreateTextureFromSurface(renderer, textSurfaceTopLeft);
	textTextureTopRight = SDL_CreateTextureFromSurface(renderer, textSurfaceTopRight);

	int textW = 0; int textH = 0;
	SDL_QueryTexture(textTextureTopLeft, NULL, NULL, &textW, &textH);
	textRectTopLeft = { 10,10,textW,textH };
	SDL_QueryTexture(textTextureTopRight, NULL, NULL, &textW, &textH);
	textRectTopRight = {WIDTH-70,10,textW,textH };
}


// Places element if no element is already in specified spot
void Game::placeElement(int mouseX, int mouseY)
{
	int scaledWidthIndex = std::min((int)ceil(mouseX / scaleFactorW),texWidth-1);
	int scaledHeightIndex = ceil(mouseY / scaleFactorH);
	int scaledIndex = convertTo1D(scaledWidthIndex,scaledHeightIndex,texWidth);
	//std::vector<Vector2Int> brushPoints;

	// Define min and max width/heights
	int minWidth = scaledWidthIndex - brushSize; int maxWidth = scaledWidthIndex + brushSize;
	int minHeight = scaledHeightIndex - brushSize; int maxHeight = scaledHeightIndex + brushSize;

	// Loop over surrounding points 
	int pointIndex = 0;
	for (int i = minWidth;i <= maxWidth;i++) {
		for (int j = minHeight;j <= maxHeight;j++) {
			if (manhattanDistance(i, j, scaledWidthIndex, scaledHeightIndex) > brushSize) continue;
			int currentIndex = convertTo1D(i, j, texWidth);
			if (currentIndex < 0 || currentIndex >= texWidth * texHeight) continue;
			// If air, replace no matter what
			if (currentElementIndex == 0) {
				elementArrayFuture[currentIndex] = 0;
			}
			else if (elementArray[currentIndex] == 0) {
				if (pointIndex >= lastBrushPointIndices.size()) {
					elementArrayFuture[currentIndex] = erasing ? 0 : currentElementIndex;
					Vector2Int brushPoint;
					brushPoint.x = i; brushPoint.y = j;
					lastBrushPointIndices.push_back(brushPoint);
				}
				else {
					drawLine(i, j, lastBrushPointIndices.at(pointIndex).x, lastBrushPointIndices.at(pointIndex).y);
					lastBrushPointIndices.at(pointIndex).x = i;
					lastBrushPointIndices.at(pointIndex).y = j;
				}
			}
			else {
				if (lastBrushPointIndices.size() > pointIndex) {
					lastBrushPointIndices.at(pointIndex).x = i;
					lastBrushPointIndices.at(pointIndex).y = j;
				}
			}
			pointIndex++;
		}
	}
}

// Places given element regardless of specified spot's current status
void Game::placeElement(int mouseX, int mouseY, int elementIndex)
{
	int scaledWidthIndex = ceil(mouseX / scaleFactorW);
	int scaledHeightIndex = ceil(mouseY / scaleFactorH);
	int scaledIndex = convertTo1D(scaledWidthIndex, scaledHeightIndex,texWidth);

	elementArrayFuture[scaledIndex] = elementIndex;
}

// Sets a particular pixel to an element if it is empty
void Game::setElement(int scaledIndex) {
	if (currentElementIndex == 0) {
		elementArrayFuture[scaledIndex] = 0;
	}
	else if (elementArray[scaledIndex] == 0) {
		elementArrayFuture[scaledIndex] = erasing ? 0 : currentElementIndex;
	}
}

// Changes the brush's selected element
void Game::changeCurrentElement(int elementID) {
	*currentElement = elementDict[elementID];
	currentElementIndex = elementID;

	// Change element text
	updateCurrentElementName();
}

void Game::updateCurrentElementName()
{
	textSurfaceTopRight = TTF_RenderText_Blended_Wrapped(mainFont, (currentElement->name + "\n" + std::to_string(brushSize+1)).c_str(), textColour, 300);
	textTextureTopRight = SDL_CreateTextureFromSurface(renderer, textSurfaceTopRight);
}

// Increases/decreases the size of the brush
void Game::changeBrushSize(int deltaSize)
{
	brushSize += deltaSize;
	if (brushSize < 0) brushSize = 0;
	if (brushSize > 5) brushSize = 5;
	lastBrushPointIndices.clear();
	updateCurrentElementName();
}

void Game::clearScreen()
{
	std::fill_n(elementArray, texWidth * texHeight, 0);
	std::fill_n(elementArrayFuture, texWidth * texHeight, 0);
	if (&filledPixels != NULL) {
		filledPixels.clear();
	}
}

void Game::resetBrushPoints() {
	lastBrushPointIndices.clear();
}

// Returns whether a downwards move is available for an element, 
// subsequently updates element arrays 
bool Game::moveDownAvailable(int pixelIndex) {
	int pixelElement = elementArray[pixelIndex];
	if (pixelIndex >= texWidth*(texHeight-1)) {
		return false;
	}
	if (elementArray[pixelIndex + texWidth] == 0) {
		movePixel(pixelIndex, pixelIndex + texWidth, pixelElement);
		return true;
	}
	else if (pixelIndex % texWidth != 0) {
		if (elementArray[pixelIndex + texWidth - 1] == 0) {
			movePixel(pixelIndex, pixelIndex + texWidth - 1, pixelElement);
			return true;
		}
		else if (pixelIndex % texWidth - 1 != 0) {
			if (elementArray[pixelIndex + texWidth + 1] == 0) {
				movePixel(pixelIndex, pixelIndex + texWidth + 1, pixelElement);
				return true;
			}
		}
	}
	
	return false;
}

int Game::availableMove(int pixelIndex) {
	int pixelElement = elementArray[pixelIndex];
	if (pixelIndex >= texWidth*(texHeight-1)) {
		return -1;
	}
	if (elementArray[pixelIndex + texWidth] == 0) {
		return pixelIndex+texWidth;
	}
	else if (pixelIndex % texWidth != 0) {
		if (elementArray[pixelIndex + texWidth - 1] == 0) {
			return pixelIndex+texWidth-1;
		}
		else if (pixelIndex % texWidth - 1 != 0) {
			if (elementArray[pixelIndex + texWidth + 1] == 0) {
				return pixelIndex + texWidth + 1;
			}
		}
	}
	return -1;
}

// Returns whether a sideways move is available for an element, 
// subsequently updates element arrays 
bool Game::moveSidewaysAvailable(int pixelIndex)
{
	int pixelElement = elementArray[pixelIndex];
	int furthestMoveIndex = pixelIndex;

	// Max displacement of a particle in one frame
	int pixelVelocity = 5;
	
	// Far left pixel column
	if (pixelIndex % texWidth == 0) {
		for (int i = 1;i <= pixelVelocity;i++) {
			if (elementArray[pixelIndex + i] == 0 && (filledPixels.find(pixelIndex + i) == filledPixels.end())) {
				furthestMoveIndex = pixelIndex + i;
				int potentialDownMoveIndex = availableMove(furthestMoveIndex);
				if (potentialDownMoveIndex > 0) {
					movePixel(pixelIndex, potentialDownMoveIndex, pixelElement);
					return true;
				}
				if (i == pixelVelocity) {
					movePixel(pixelIndex, furthestMoveIndex, pixelElement);
					return true;
				}
			}
			else {
				if (furthestMoveIndex != pixelIndex) {
					movePixel(pixelIndex, furthestMoveIndex, pixelElement);
					return true;
				}
				else {
					return false;
				}
			}
		}
		return false;
	}
	// Far right pixel column
	else if (pixelIndex % (texWidth - 1) == 0) {
		for (int i = 1;i <= pixelVelocity;i++) {
			if (elementArray[pixelIndex - i] == 0 && (filledPixels.find(pixelIndex - i) == filledPixels.end())) {
				furthestMoveIndex = pixelIndex - i;
				int potentialDownMoveIndex = availableMove(furthestMoveIndex);
				if (potentialDownMoveIndex > 0) {
					movePixel(pixelIndex, potentialDownMoveIndex, pixelElement);
					return true;
				}
				if (i == pixelVelocity) {
					movePixel(pixelIndex, furthestMoveIndex, pixelElement);
					return true;
				}
			}
			else {
				if (furthestMoveIndex != pixelIndex) {
					movePixel(pixelIndex, furthestMoveIndex, pixelElement);
					return true;
				}
				else {
					return false;
				}
			}
		}
	}
	else {
		// Choose left/right movement randomly
		int i = pixelIndex;
		int randomRoll = rand() % 2;
		int firstIndex = (randomRoll == 0) ? pixelVelocity : -pixelVelocity;
		int secondIndex = (randomRoll == 0) ? -pixelVelocity : pixelVelocity;
		bool firstCondition = (firstIndex > 0) ? i < pixelIndex + firstIndex : i > pixelIndex + firstIndex;
		bool secondCondition = (secondIndex < 0) ? i > pixelIndex + secondIndex : i < pixelIndex + secondIndex;

		// Move left/right if available
		for (i = pixelIndex; firstCondition; i += sgn(firstIndex)*1) {
			if (elementArray[i + sgn(firstIndex) * 1] == 0 && (filledPixels.find(i + sgn(firstIndex) * 1) == filledPixels.end())) {
				furthestMoveIndex = i + sgn(firstIndex) * 1;
				int potentialDownMoveIndex = availableMove(furthestMoveIndex);
				if (potentialDownMoveIndex > 0) {
					movePixel(pixelIndex, potentialDownMoveIndex, pixelElement);
					return true;
				}
				if (i == pixelIndex + firstIndex) {
					movePixel(pixelIndex, furthestMoveIndex, pixelElement);
					return true;
				}
			}
			else {
				if (furthestMoveIndex != pixelIndex) {
					movePixel(pixelIndex, furthestMoveIndex, pixelElement);
					return true;
				}
				else {
					return false;
				}
			}
			firstCondition = (firstIndex > 0) ? i < pixelIndex + firstIndex : i > pixelIndex + firstIndex;
		}
		for (i = pixelIndex; secondCondition; i += sgn(secondIndex) * 1) {
			
			if (elementArray[i + sgn(secondIndex) * 1] == 0 && (filledPixels.find(i + sgn(secondIndex) * 1) == filledPixels.end())) {
				furthestMoveIndex = i + sgn(secondIndex) * 1;
				int potentialDownMoveIndex = availableMove(furthestMoveIndex);
				if (potentialDownMoveIndex > 0) {
					movePixel(pixelIndex, potentialDownMoveIndex, pixelElement);
					return true;
				}
				if (i == pixelIndex + secondIndex) {
					movePixel(pixelIndex, furthestMoveIndex, pixelElement);
					return true;
				}
			}
			else {
				if (furthestMoveIndex != pixelIndex) {
					movePixel(pixelIndex, furthestMoveIndex, pixelElement);
					return true;
				}
				else {
					return false;
				}
			}
			secondCondition = (secondIndex < 0) ? i > pixelIndex + secondIndex : i < pixelIndex + secondIndex;
		}
	}
	return false;
}

// Returns whether an upwards move is available for an element, 
// subsequently updates element arrays 
bool Game::moveUpAvailable(int pixelIndex) {
	int pixelElement = elementArray[pixelIndex];
	if (pixelIndex < texWidth) {
		return false;
	}
	if (elementArray[pixelIndex - texWidth] == 0) {
		movePixel(pixelIndex, pixelIndex - texWidth, pixelElement);
		return true;
	}
	else if (pixelIndex % texWidth != 0) {
		if (elementArray[pixelIndex - texWidth - 1] == 0) {
			movePixel(pixelIndex, pixelIndex - texWidth - 1, pixelElement);
			return true;
		}
		else if (pixelIndex % texWidth - 1 != 0) {
			if (elementArray[pixelIndex - texWidth + 1] == 0) {
				movePixel(pixelIndex, pixelIndex - texWidth + 1, pixelElement);
				return true;
			}
		}
	}
	return false;
}

// Moves a pixel from one place to another
void Game::movePixel(int sourceIndex, int destinationIndex, int elementID)
{
	elementArrayFuture[sourceIndex] = 0;
	elementArrayFuture[destinationIndex] = elementID;
	filledPixels.insert(destinationIndex);
}



/* 
	GETS + SETS 
*/

std::string Game::getCurrentElementName() {
	return currentElement->name;
}

//Vector2Int Game::getLastElementPositionIndex()
//{
//	return lastElementPositionIndex;
//}
//
//void Game::setLastElementPositionIndex(int x, int y)
//{
//	lastElementPositionIndex.x = x;
//	lastElementPositionIndex.y = y;
//}


/*
	HELPER FUNCTIONS
*/

// Bresenham's line algorithm implementation from https://rosettacode.org/wiki/Bitmap/Bresenham%27s_line_algorithm#C.2B.2B
void Game::drawLine(int x1, int y1, int x2, int y2)
{
	if (x2 < 0 || y2 < 0) return;
	const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
	if (steep)
	{
		std::swap(x1, y1);
		std::swap(x2, y2);
	}

	if (x1 > x2)
	{
		std::swap(x1, x2);
		std::swap(y1, y2);
	}

	const float dx = x2 - x1;
	const float dy = fabs(y2 - y1);

	float error = dx / 2.0f;
	const int ystep = (y1 < y2) ? 1 : -1;
	int y = (int)y1;

	const int maxX = (int)x2;

	for (int x = (int)x1; x <= maxX; x++)
	{
		if (steep)
		{
			setElement(convertTo1D(y, x,texWidth));
		}
		else
		{
			setElement(convertTo1D(x, y,texWidth));
		}

		error -= dy;
		if (error < 0)
		{
			y += ystep;
			error += dx;
		}
	}
}

// Converts 2D array index to 1D
int Game::convertTo1D(int x, int y, int width) {
	return (width * y) + x;
}

// Converts 1D array index to 2D point
Vector2Int Game::convertTo2D(int index, int width) {
	Vector2Int _point = { index % width, floor(index / width) };
	return _point;

}

// Returns the Manhattan (rectilinear) distance between two 2D points
int Game::manhattanDistance(int x1, int y1, int x2, int y2) {
	return abs(x1 - x2) + abs(y1 - y2);
}


