#include <SFML\Graphics.hpp>

#ifndef LOADING_SCREEN_H
#define LOADING_SCREEN_H

namespace OCLU{

enum LoadScreenType{
	OCLUBar,
};
class LoadingScreen
{
public:
	void draw(sf::RenderWindow * renderWindow);
	LoadingScreen(void);
	~LoadingScreen(void);
};
};
#endif LOADING_SCREEN_H