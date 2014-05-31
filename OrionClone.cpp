   //Takumi Orion Clone Loading Screen Utility
   // Copyright (C) 2014  George Takumi Crary

   // This program is free software: you can redistribute it and/or modify
   // it under the terms of the GNU General Public License as published by
   // the Free Software Foundation, either version 3 of the License, or
   // any later version.

   // This program is distributed in the hope that it will be useful,
   // but WITHOUT ANY WARRANTY; without even the implied warranty of
   // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   // GNU General Public License for more details.

   // You should have received a copy of the GNU General Public License
   // along with this program.  If not, see <http://www.gnu.org/licenses/>.

// OrionClone.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include <string>
#include <vector>
#include <assert.h>
#include "LoadingScreen.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "sfmlLoadUtil.h"

template <typename F> void templateParamNameCheck(F pFuncArg){
	printf("%s",typeid(pFuncArg).name());
};
template <typename F> void templateClassNameCheck(){
	printf("%s",typeid(F).name());
};


int _tmain(int argc, _TCHAR* argv[])//LOAD SCREEN PROJECT HAS TAKEN OVER. ORION CLONE SHALL BE POSTPONED TILL LOADER PROJECT HAS BEEN RELEASED
{
	//http://stackoverflow.com/questions/17369972/c-templated-constructor-error
	sf::Font ArialFont;
	sf::Texture pText;

	std::vector<struct OCLU::loadTarget> loadTargets;
	OCLU::OCLULoadResultWrapper loadResult;
	loadTargets.push_back(struct OCLU::loadTarget(&ArialFont,"arial.ttf",NULL_LOAD_ARG));
	loadTargets.push_back(struct OCLU::loadTarget(&pText,"mjTextureTest.png",NULL_LOAD_ARG));


	

	sf::RenderWindow window(sf::VideoMode(640, 480), "Orion Clone Loading Screen Utility"); //OCLU
	window.setFramerateLimit(60);

	while((loadResult = loadAssets(&loadTargets))==OCLU::fileLoadSuccess){//TODO Make a error handling function that can easily force a program exit.
		printf("\n File Load Success \n");
	}

	sf::Text myText;
	//TODO make assertion functions that generically accept a pointer. Things like check if a font is loaded for a sf::Text to show properly
	//We need some sort of way to 
	sf::Text swagText;
	myText.setFont(ArialFont);
	bool test =  swagText.getFont()==nullptr;
	test = myText.getFont() == nullptr ;
	assert(myText.getFont()!=nullptr && "Null font");
	myText.setPosition(5,0);
	myText.setString("Loading Assets...");
	myText.setCharacterSize(20);
	myText.setColor(sf::Color(130,170,230,255));

	//if(!pText.loadFromFile("mjTextureTest.png")){
	//	printf("load failure");
	//}

	sf::Sprite pSprite;
	pSprite.setTexture(pText);
	pSprite.setPosition(0,0);

	OCLU::LoadingScreen gameLoadingScreen(window.getSize(),ArialFont);
	unsigned int updateCounter = 0;
	gameLoadingScreen.update(OCLU::LoadScreenStats(updateCounter,10,nullptr));

	bool drawLoadingText = true;
	sf::Clock timer;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed){
                window.close();
			}
			if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Return){
				updateCounter++;
				gameLoadingScreen.update(OCLU::LoadScreenStats(updateCounter,10,nullptr));
			}
        }
		//loadAssets();
        window.clear();
		gameLoadingScreen.draw(&window);
		//window.draw(pSprite);
		//window.draw(myText);
		//if(drawLoadingText){
		//	float timeElapsed=timer.getElapsedTime().asSeconds();
		//	if(timeElapsed<5.0f){
		//		myText.setColor(sf::Color(130,170,230,getAlphaOfInterval(timeElapsed,3.0f)));
		//		//printf("\n%d\n",getAlphaOfInterval(timeElapsed,5.0f));
		//	}else{
		//		drawLoadingText=false;
		//		printf("Time has elapsed");
		//	}
		//}
        window.display();
    }
	return 0;
}


//std::hash<std::string> hashFunc;
//printf("Test:%d\n",hashFunc(typeid(sf::Font).name()));
//printf("Hash Code:%d\n",typeid(sf::Text).hash_code());
//printf("Hash Code:%d\n",typeid(sf::Text).hash_code());
//void * pTest = &myText;
//printf("\nType Name is:%s\n",typeid(myText).name());//Type id just returns char * of that variable's type name. Not base variable's real type
////consider replacing sfType enum with just 
