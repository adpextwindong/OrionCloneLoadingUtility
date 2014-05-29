#include "stdafx.h"
#include "sfmlLoadUtil.h"
#include <vector>

void OCLU::swag(void){

};
enum OCLU::supportedClasses AssetClassStringToEnum(std::string classString){//implement conditional compile directives
	enum OCLU::supportedClasses returnVal = OCLU::nonSupported;

#ifdef SFML_GRAPHICS_HPP
	if(classString.compare(typeid(sf::Font *).name())==0){
		returnVal=sf_Font;
	}else if(classString.compare(typeid(sf::Shader *).name())==0){
		returnVal=sf_Shader;
	}else if(classString.compare(typeid(sf::Texture *).name())==0){
		returnVal=sf_Texture;
	}else if(classString.compare(typeid(sf::Image *).name())==0){
		returnVal=sf_Image;
	}
#endif

#ifdef SFML_AUDIO_HPP		//if audio compile only
	#ifdef SFML_GRAPHICS_HPP	//if graphics is included
		else 
	#endif
			if(classString.compare(typeid(sf::SoundBuffer *).name())==0){
				returnVal = sf_SoundBuffer;
		}
#endif
	return returnVal;
};

enum OCLU::loadEnum OCLU::loadAssets(std::vector<struct OCLU::loadTarget> * paramList){//Resource loader that behaves similar to cstring strtok
	static std::vector<struct OCLU::loadTarget> loadTargetList;
	static std::vector<struct OCLU::loadTarget> * prevCallList;
	static unsigned int assetCounter = 0;
	bool pushMore = false;
	if(prevCallList != paramList){
		prevCallList = paramList;
		pushMore = true;
		assetCounter = 0;
	}
	enum OCLU::loadEnum returnVal=OCLU::fileLoadFail;
	if(paramList!=nullptr && pushMore == true){
		for(unsigned int i=assetCounter; i < paramList->size(); i++){
			loadTargetList.push_back((*paramList)[i]);
		}
	}else{
		if(assetCounter >= loadTargetList.size()-1){
			returnVal = OCLU::loadingFinished;
		}
	}
	printf("%d %d",assetCounter,loadTargetList.size());
	if(assetCounter < loadTargetList.size()){
		struct OCLU::loadTarget * pLoadTarget = &loadTargetList[assetCounter];
		switch(AssetClassStringToEnum(pLoadTarget->targetTypeString)){

#ifdef SFML_GRAPHICS_HPP
		case sf_Font:
			{
				//sf::Font * pFont = static_cast<sf::Font *>(pLoadTarget->targetPointer);	//Static cast from void * to Type X so compiler/intellisense can see member functions //TODO learn how to really use c++ casts
				if(pLoadTarget->isUnaryFunctionTarget){
					bool (sf::Font::*pFunc)(const std::string &) = &sf::Font::loadFromFile;
					returnVal = templateUnaryLoad<sf::Font>(pLoadTarget,pFunc);
				}else{
					//Incorrect Secondary Arg Type
					returnVal = incorrectSecondaryArgType;
				}
				

				break;
			}

		case sf_Shader:
			{
				//null pointer exceptions and
				// parameter type checking should be left up to the switch case because you can't get the correct signature.
				//Just feed the appropriate function signatures and make sure you pass the correct class names to the template args 
				//http://www.parashift.com/c++-faq/pointers-to-members.html
				if(!pLoadTarget->isUnaryFunctionTarget){
					if(pLoadTarget->secondaryArgTypeString.compare(typeid(std::string).name())==0){// String signature function
						
						bool (sf::Shader::*pStringFunc)(const std::string &,const std::string &) = &sf::Shader::loadFromFile;
						returnVal = templateBinaryLoad<sf::Shader,const std::string &>(pLoadTarget,pStringFunc);

					}else if(pLoadTarget->secondaryArgTypeString.compare(typeid(sf::Shader::Type).name())==0){// Shader Signature function
						bool (sf::Shader::*pVertexFunc)(const std::string &,sf::Shader::Type) = &sf::Shader::loadFromFile;
						returnVal = templateBinaryDerefLoad<sf::Shader,sf::Shader::Type>(pLoadTarget,pVertexFunc);

					}else{
						//Incorrect Secondary Arg Type
						returnVal = incorrectSecondaryArgType;
					}
				}else{
					//lacking 2nd arg
					returnVal = incorrectSecondaryArgType;
				}

				break;
			}

		case sf_Texture:
			{
				
				if(!pLoadTarget->isUnaryFunctionTarget){
					if(!pLoadTarget->secondaryArgTypeString.compare(typeid(sf::IntRect).name())==0){
						bool (sf::Texture::*pFunc)(const std::string &,const sf::IntRect &) = &sf::Texture::loadFromFile;
						returnVal = templateBinaryLoad<sf::Texture,const sf::IntRect &>(pLoadTarget,pFunc);
					}else{
						//Incorrect Secondary Arg Type
						returnVal = incorrectSecondaryArgType;
					}
				}else{
					if(pLoadTarget->targetPointer!=nullptr){//the template function hangs up on calling the member function even with an empty int rect.
						sf::Texture * pTexture = static_cast<sf::Texture *>(pLoadTarget->targetPointer);
						if(!pTexture->loadFromFile(pLoadTarget->pathStringArg)){
							printf("\nError loading texture PATH: %s",pLoadTarget->pathStringArg);
							returnVal = fileLoadFail;
						}else{
							returnVal = fileLoadSuccess;
						}
					}else{
						returnVal = nullPointerException;
					}
				}
				break;
			}
		case sf_Image:
			{
				if(pLoadTarget->isUnaryFunctionTarget){
					bool (sf::Image::*pFunc)(const std::string &)= &sf::Image::loadFromFile;
					returnVal = templateUnaryLoad<sf::Image>(pLoadTarget,pFunc);
				}else{
					//Incorrect Secondary Arg Type
					returnVal = incorrectSecondaryArgType;
				}
				
				break;
			}
#endif
#ifdef SFML_AUDIO_HPP
		case sf_SoundBuffer:
			{
				if(pLoadTarget->isUnaryFunctionTarget){
					bool (sf::SoundBuffer::*pFunc)(const std::string &) = &sf::SoundBuffer::loadFromFile;
					returnVal = templateUnaryLoad<sf::SoundBuffer>(pLoadTarget,pFunc);
				}else{
					//Incorrect Secondary Arg Type
					returnVal = incorrectSecondaryArgType;
				}
				
				break;
			}
#endif
		default:
			{
				printf("\nERROR: Unimplemented Class or does not fit the loadFromFile interface\n");
				returnVal = OCLU::invalidClass;
				break;
			}
		}
		pLoadTarget->loadResult = returnVal;
	}
	if(returnVal == OCLU::fileLoadSuccess){
		assetCounter++;
	}
	
	return returnVal;
};