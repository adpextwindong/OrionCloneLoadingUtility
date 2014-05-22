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

#include <cmath>
#define PI 3.14159265
#define NULL_LOAD_ARG (void *)nullptr
/*
	http://www.sfml-dev.org/documentation/2.1/functions_func_0x6c.php#index_l
	loadFromFile() : sf::SoundBuffer , sf::Shader , sf::Texture , sf::Font , sf::Image
	loadFromImage() : sf::Texture
	loadFromMemory() : sf::Texture , sf::SoundBuffer , sf::Font , sf::Shader , sf::Image
	loadFromSamples() : sf::SoundBuffer
	loadFromStream() : sf::Shader , sf::Font , sf::SoundBuffer , sf::Texture , sf::Image
*/

namespace OCLU{
#define SUPPORTED_GRAPHICS_CLASSES_MACRO sf_Shader , sf_Texture , sf_Font , sf_Image, sf_VertexShader 
#define SUPPORTED_AUDIO_CLASSES_MACRO sf_SoundBuffer
#ifdef SFML_GRAPHICS_HPP  //wow check out how VS2010 highlights live preprocessor if's and greys out dead code. Only if it had switch case generators and other shit
	#ifdef SFML_AUDIO_HPP
		enum supportedClasses { SUPPORTED_GRAPHICS_CLASSES_MACRO , SUPPORTED_AUDIO_CLASSES_MACRO , nonSupported };
	#else
		enum supportedClasses { SUPPORTED_GRAPHICS_CLASSES_MACRO , nonSupported };
	#endif
#else
	#ifdef SFML_AUDIO_HPP
		enum supportedClasses { SUPPORTED_AUDIO_CLASSES_MACRO , nonSupported };
	#else
		enum supportedClasses { nonSupported } ;
		#error ("Neither SFML graphics or audio are included. This library workaround is useless without them. Defined behavior by the work around is not guarrenteed.")
		//TODO fucking figure out this pragma warning shit. I don't get why its not fucking showing
	#endif
#endif
//#undef SUPPORTED_GRAPHICS_CLASSES_MACRO cannot undef macro because macro in enum doesn't work.
//#undef SUPPORTED_AUDIO_CLASSES_MACRO

//enum supportedClasses{sf_Shader , sf_Texture , sf_Font , sf_Image , sf_Shader, nonSupported};//original preprocessorless enum.

enum supportedClasses AssetClassStringToEnum(std::string classString){//implement conditional compile directives
	enum supportedClasses returnVal = nonSupported;

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
}
enum loadEnum{
	neverTouched,//this resolves the same list problem. all loadTargets are initialized to neverTouched.
	invalidClass,//User tried to pass a class that doesn't have a sfml load function
	fileLoadSuccess,
	fileLoadFail,
	loadingFinished,
	incorrectSecondaryArgType,
	nullPointerException,
};
struct loadTarget{//Work around for the SFML library lacking an interface for objects that can "loadFromXXX"
	loadEnum loadResult;
	void * targetPointer;
	std::string targetTypeString;// 
	std::string pathStringArg;//fuck visual studio 2010, no quick refactor tool. eclispe++;
	void * secondaryArg;
	std::string secondaryArgTypeString;
	bool isUnaryFunctionTarget;

	//Using generics we can safely get the type of the class without relieing on the user to manually typeid'ing for the name
	//More secure that way, someone could forge a class name and throw and error. idk why they'd do that
	//Hopefully I can implement a c99 version of this for the c binding.

	template <class T,class S> loadTarget(T * targParam,std::string pathParam,S * secondArg):	
		targetPointer((void *) targParam),								
		targetTypeString(typeid(targParam).name()),							
		pathStringArg(pathParam),
		secondaryArg((void *) secondArg),
		secondaryArgTypeString(typeid(secondArg).name()),
		loadResult(neverTouched)
		{
			isUnaryFunctionTarget = secondArg == nullptr ? (strcmp(secondaryArgTypeString.c_str(),typeid(void *).name())== 0 ? true : false ) : false; // nullptr of type void * means user used NULL_LOAD_ARG macro
		};

};


template <class CAST_CLASS, class FUNC> loadEnum templateUnaryLoad(struct loadTarget * payload,FUNC memberFunction){	//cast and call correct signature
	loadEnum returnVal = nullPointerException;
	if(payload->targetPointer!=nullptr && memberFunction!=nullptr){										//Parameter type must be confirmed before calling. 
		CAST_CLASS * pCastTarg = static_cast<CAST_CLASS *>(payload->targetPointer);					//Should be done after getting address of correct signature	
		if(!((*pCastTarg).*memberFunction)(payload->pathStringArg)){									//These functions should be used privatly in the library anyways. Don't tear me a new one like OpenSSL
			printf("\nError loading %s PATHS: %s\n",typeid(CAST_CLASS).name(),payload->pathStringArg.c_str());
			returnVal = fileLoadFail;
		}else{
			returnVal = fileLoadSuccess;
		}
	}else{
		returnVal = nullPointerException;
	}
	return returnVal;
};
template <class CAST_CLASS, class CAST_PARAM, class FUNC> loadEnum templateBinaryDerefLoad(struct loadTarget * payload,FUNC memberFunction){//derefs the second pointer. Don't know how to have a conditional compile per template instantiation
	loadEnum returnVal = nullPointerException;
	if(payload->targetPointer!=nullptr && memberFunction!=nullptr){										//Parameter type must be confirmed before calling. 
		CAST_CLASS * pCastTarg = static_cast<CAST_CLASS *>(payload->targetPointer);
		if(!payload->isUnaryFunctionTarget){//if it has two arguments									//These functions should be used privatly in the library anyways. Don't tear me a new one like OpenSSL
			const std::string & pFirstArg = (payload->pathStringArg);
			CAST_PARAM * pSecondArg = (CAST_PARAM *)(payload->secondaryArg);
			CAST_PARAM secondArg = *pSecondArg;
			if(!((*pCastTarg).*memberFunction)(pFirstArg,secondArg)){
				printf("\nError loading %s PATHS: %s and %s\n",typeid(CAST_CLASS).name(),payload->pathStringArg.c_str(),payload->secondaryArgTypeString.c_str());
				returnVal = fileLoadFail;
			}else{
				returnVal = fileLoadSuccess;
			}
		}
	}else{
		returnVal = nullPointerException;
	}
	return returnVal;
};
template <class CAST_CLASS, class CAST_PARAM, class FUNC> loadEnum templateBinaryLoad(struct loadTarget * payload,FUNC memberFunction){	//cast and call correct signature //second function is needed because of type casting
	loadEnum returnVal = nullPointerException;
	if(payload->targetPointer!=nullptr && memberFunction!=nullptr){										//Parameter type must be confirmed before calling. 
		CAST_CLASS * pCastTarg = static_cast<CAST_CLASS *>(payload->targetPointer);
		if(!payload->isUnaryFunctionTarget){//if it has two arguments									//These functions should be used privatly in the library anyways. Don't tear me a new one like OpenSSL
			const std::string & pFirstArg = (payload->pathStringArg);
			CAST_PARAM pSecondArg = (CAST_PARAM)(payload->secondaryArg);
			if(!((*pCastTarg).*memberFunction)(pFirstArg,pSecondArg)){
				printf("\nError loading %s PATHS: %s and %s\n",typeid(CAST_CLASS).name(),payload->pathStringArg.c_str(),payload->secondaryArgTypeString.c_str());
				returnVal = fileLoadFail;
			}else{
				returnVal = fileLoadSuccess;
			}
		}
	}else{
		returnVal = nullPointerException;
	}
	return returnVal;
};

enum loadEnum loadAssets(std::vector<struct loadTarget> * paramList){//Resource loader that behaves similar to cstring strtok
	static std::vector<struct loadTarget> loadTargetList;
	static std::vector<struct loadTarget> * prevCallList;
	static unsigned int assetCounter = 0;
	bool pushMore = false;
	if(prevCallList != paramList){
		prevCallList = paramList;
		pushMore = true;
		assetCounter = 0;
	}
	enum loadEnum returnVal=fileLoadFail;
	if(paramList!=nullptr && pushMore == true){
		for(unsigned int i=assetCounter; i < paramList->size(); i++){
			loadTargetList.push_back((*paramList)[i]);
		}
	}else{
		if(assetCounter >= loadTargetList.size()-1){
			returnVal = loadingFinished;
		}
	}
	printf("%d %d",assetCounter,loadTargetList.size());
	if(assetCounter < loadTargetList.size()){
		struct loadTarget * pLoadTarget = &loadTargetList[assetCounter];
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
				returnVal = invalidClass;
				break;
			}
		}
		pLoadTarget->loadResult = returnVal;
	}
	if(returnVal == fileLoadSuccess){
		assetCounter++;
	}
	
	return returnVal;
}
//draw loading text. "Brightness based on time passed"

template <typename F> void templateParamNameCheck(F pFuncArg){
	printf("%s",typeid(pFuncArg).name());
};
template <typename F> void templateClassNameCheck(){
	printf("%s",typeid(F).name());
};

class OCLULoader{//TODO switch over to cleaner class instead of struct + function
public:
	void pushList(std::vector<struct OCLU::loadTarget> list);
	void load();
	const std::vector<struct OCLU::loadTarget> seeList();
private:
	bool firstLoad;
};
}