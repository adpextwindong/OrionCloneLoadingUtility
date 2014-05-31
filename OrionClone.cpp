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
#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "sfmlLoadUtil.h"


template <typename F> void templateParamNameCheck(F pFuncArg){
	printf("%s",typeid(pFuncArg).name());
};
template <typename F> void templateClassNameCheck(){
	printf("%s",typeid(F).name());
};
//https://www.youtube.com/watch?v=Gg_dnfOg7B8&feature=kp
//The first second is what we're trying to emulate with this.
int getAlphaOfInterval(float elapsedTime, float animInterval,double (*func)(double) = &sin){//consider function pointers for sin/cos curves
	//I seriously doubt this default param value works in VS2010.
	if(func == nullptr){
		printf("Why did you pass a null pointer.");
		func = &sin;
	}
	return (int)fabs((255*func((elapsedTime/animInterval)*2*PI)));
}
void animDemoText(sf::Text * pText, sf::Clock * pClock){
	float timeElapsed=pClock->getElapsedTime().asSeconds();
	if(timeElapsed<5.0f){
		pText->setColor(sf::Color(130,170,230,getAlphaOfInterval(timeElapsed,3.0f)));
		//printf("\n%d\n",getAlphaOfInterval(timeElapsed,5.0f));
	}
}

int _tmain(int argc, _TCHAR* argv[])//LOAD SCREEN PROJECT HAS TAKEN OVER. ORION CLONE SHALL BE POSTPONED TILL LOADER PROJECT HAS BEEN RELEASED
{
	//http://stackoverflow.com/questions/17369972/c-templated-constructor-error
	sf::Font defaultArialFont;
	if(!defaultArialFont.loadFromFile("resources/arial.ttf")){//For presentation purposes this shall be the only thing we hardcode.
		printf("Arial Font Load Failed");
	}
	sf::Font cour;
	sf::Font pala;
	sf::Font framd;
	sf::Text courSampleText;
	sf::Text palaSampleText;
	sf::Text framdSampleText;
	std::string sampleText = "sample text";

	courSampleText.setString(sampleText);
	courSampleText.setPosition(600,0);
	courSampleText.setColor(sf::Color::Blue);

	palaSampleText.setString(sampleText);
	palaSampleText.setPosition(600,100);
	palaSampleText.setColor(sf::Color::Blue);

	framdSampleText.setString(sampleText);
	framdSampleText.setPosition(600,200);
	framdSampleText.setColor(sf::Color::Blue);

	sf::Texture pText;
	std::vector<sf::Texture * > pTextures;

	float presentationDelay = 0.1f;
	std::cout << "Input presentation load delay in seconds (FLOAT VALUE)" << std::endl; 
	std::cin >> presentationDelay;
	std::cout << std::endl << "Presentation Delay: " << presentationDelay << std::endl;

	std::vector<struct OCLU::loadTarget> loadTargets;
	OCLU::loadEnum loadResult = OCLU::neverTouched;

	loadTargets.push_back(struct OCLU::loadTarget(&cour,"resources/cour.ttf",NULL_LOAD_ARG));
	loadTargets.push_back(struct OCLU::loadTarget(&pala,"resources/pala.ttf",NULL_LOAD_ARG));
	loadTargets.push_back(struct OCLU::loadTarget(&framd,"resources/framd.ttf",NULL_LOAD_ARG));

	loadTargets.push_back(struct OCLU::loadTarget(&pText,"resources/mjTextureTestAlpha.png",NULL_LOAD_ARG));
	pTextures.push_back(new sf::Texture);
	loadTargets.push_back(struct OCLU::loadTarget( pTextures[pTextures.size()-1],"resources/belAir.jpg",NULL_LOAD_ARG));
	pTextures.push_back(new sf::Texture);
	loadTargets.push_back(struct OCLU::loadTarget( pTextures[pTextures.size()-1],"resources/farSideVirtual.jpg",NULL_LOAD_ARG));
	pTextures.push_back(new sf::Texture);
	loadTargets.push_back(struct OCLU::loadTarget( pTextures[pTextures.size()-1],"resources/playingTheAngel.jpg",NULL_LOAD_ARG));
	
	pTextures.push_back(new sf::Texture);
	loadTargets.push_back(struct OCLU::loadTarget( pTextures[pTextures.size()-1],"resources/belAir.jpg",NULL_LOAD_ARG));
	pTextures.push_back(new sf::Texture);
	loadTargets.push_back(struct OCLU::loadTarget( pTextures[pTextures.size()-1],"resources/farSideVirtual.jpg",NULL_LOAD_ARG));
	pTextures.push_back(new sf::Texture);
	loadTargets.push_back(struct OCLU::loadTarget( pTextures[pTextures.size()-1],"resources/playingTheAngel.jpg",NULL_LOAD_ARG));
	
	pTextures.push_back(new sf::Texture);
	loadTargets.push_back(struct OCLU::loadTarget( pTextures[pTextures.size()-1],"resources/belAir.jpg",NULL_LOAD_ARG));
	pTextures.push_back(new sf::Texture);
	loadTargets.push_back(struct OCLU::loadTarget( pTextures[pTextures.size()-1],"resources/farSideVirtual.jpg",NULL_LOAD_ARG));
	pTextures.push_back(new sf::Texture);
	loadTargets.push_back(struct OCLU::loadTarget( pTextures[pTextures.size()-1],"resources/playingTheAngel.jpg",NULL_LOAD_ARG));
	

	sf::RenderWindow window(sf::VideoMode(800, 600), "Orion Clone Loading Screen Utility"); //OCLU
	window.setFramerateLimit(60);

	//while((loadResult = loadAssets(&loadTargets))==OCLU::fileLoadSuccess){//TODO Make a error handling function that can easily force a program exit.
	//	printf("\n File Load Success \n");
	//}


	//TODO Make a error handling function that can easily force a program exit.
	//TODO make assertion functions that generically accept a pointer. Things like check if a font is loaded for a sf::Text to show properly
	sf::Text myText;
	myText.setFont(defaultArialFont);
	myText.setPosition(5,0);
	myText.setString("Loading Assets...");
	myText.setCharacterSize(20);
	myText.setColor(sf::Color(130,170,230,255));

	sf::Text animDemoTest;
	animDemoTest.setFont(defaultArialFont);
	animDemoTest.setPosition(5,30);
	animDemoTest.setString("Loading Assets...");
	animDemoTest.setCharacterSize(20);
	animDemoTest.setColor(sf::Color(130,170,230,255));

	sf::Sprite mjSprite;
	mjSprite.setPosition(0,0);
	
	bool drawLoadingText = true;
	sf::Clock demoTimer;
	sf::Clock myClock;
	
	bool loadScreen = true;
	std::vector<sf::Sprite *> dynSprites;

	//unsigned int counterSprite = 0;


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed){
                window.close();
			}
        }
		//loadAssets();
        window.clear();
		
		switch(loadScreen){
		case true:
			myText.setColor(sf::Color(130,170,230,getAlphaOfInterval(myClock.getElapsedTime().asSeconds(),1.0f)));
			if(demoTimer.getElapsedTime().asSeconds() > presentationDelay){
				if(loadResult!=OCLU::loadingFinished){
					if((loadResult = loadAssets(&loadTargets))==OCLU::fileLoadSuccess){//THIS IS THE LOADING CODE
						printf("\n File Load Success \n");
					}
				}else{
					//INITIALIZATION AREA FOR THINGS DEPENDANT ON THE PREVIOUSLY LOADED OBJECTS
					loadScreen = false;
					framdSampleText.setFont(framd);
					courSampleText.setFont(cour);
					palaSampleText.setFont(pala);

					mjSprite.setTexture(pText);
					printf("\nLoading has finished. Took %f seconds.\n",myClock.getElapsedTime().asSeconds());
					for(unsigned int i = 0; i < pTextures.size(); i++){
						dynSprites.push_back(new sf::Sprite);
						dynSprites[i]->setTexture(*pTextures[i]);
						dynSprites[i]->setPosition(100 * i, 100 * i);
					}
				}
				demoTimer.restart();
			}
			window.draw(myText);
			break;
		case false:
			//FILES DEPENDANT ON OCLULoader LoadTargets get set here and whatnot. TODO possibly implement I guess a callback or something for sf::Textures to sprites that need to be set
			for(unsigned int i = 0; i < dynSprites.size();i++){
				window.draw(*dynSprites[i]);
			}
			window.draw(mjSprite);
			window.draw(framdSampleText);
			window.draw(courSampleText);
			window.draw(palaSampleText);
			break;
		}
		
		animDemoText(&animDemoTest,&demoTimer);
		
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

/*
sf::Text swagText;
	myText.setFont(defaultArialFont);
	bool test =  swagText.getFont()==nullptr;
	test = myText.getFont() == nullptr ;
	assert(myText.getFont()!=nullptr && "Null font");
*/