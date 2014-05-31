#include "StdAfx.h"
#include "LoadingScreen.h"


OCLU::LoadingScreen::LoadingScreen(sf::Vector2u _windowSize,sf::Font _fgFont):
	fgFont(_fgFont),
	windowSize(_windowSize)
{
	//fgText.setColor(sf::Color::Yellow);
	//fgText.setString("Loading Started");
	//fgText.setPosition(0,windowSize.y/2);
	fgText.setFont(fgFont);
	fgText.setPosition(5,0);
	fgText.setString("Loading Assets...");
	fgText.setCharacterSize(20);
	fgText.setColor(sf::Color(130,170,230,255));

	bg.setSize(sf::Vector2f(windowSize.x, 150));
	bg.setPosition(0, windowSize.y/2 - 150/2);
	bg.setFillColor(sf::Color::White);

	fgLoad.setSize(sf::Vector2f(50, 130));
	fgLoad.setPosition(0, windowSize.y/2 - 130/2);
	fgLoad.setFillColor(sf::Color::Blue);
}

void OCLU::LoadingScreen::update(OCLU::LoadScreenStats stats,float elapsedTime){
	fgText.setColor(sf::Color(130,170,230,getAlphaOfInterval(elapsedTime,3.0f)));
	sf::Vector2f tempSize(windowSize.x * ((float) stats.filesLoaded/ (float) stats.totalFiles), 130);
	fgLoad.setSize(tempSize);
}
void OCLU::LoadingScreen::draw(sf::RenderWindow * renderWindow){

	renderWindow->draw(bg);
	renderWindow->draw(fgLoad);
	renderWindow->draw(fgText);
	
}

OCLU::LoadingScreen::~LoadingScreen(void)
{
}
