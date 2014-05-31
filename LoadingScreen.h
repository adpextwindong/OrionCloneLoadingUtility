#ifndef LOADING_SCREEN_H
#define LOADING_SCREEN_H
#include <SFML\Graphics.hpp>
#include "OCLULoader.h"
namespace OCLU{

enum LoadScreenType{
	OCLUBar,
};
class LoadingScreen
{
public:
	void draw(sf::RenderWindow * renderWindow);
	LoadingScreen(sf::Vector2u _windowSize,sf::Font fgFont);
	~LoadingScreen(void);
	void update(OCLU::LoadScreenStats,float elapsedTime);
private:
	sf::Vector2u windowSize;
	sf::RectangleShape bg;//White Background
	sf::RectangleShape fgLoad;//
	sf::Text fgText;//Stats Text
	sf::Font fgFont;
};
};
#endif LOADING_SCREEN_H