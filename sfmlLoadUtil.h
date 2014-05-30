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
#ifndef SFML_LOAD_UTIL_H
#define SFML_LOAD_UTIL_H
#include <cmath>
#include <vector>
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

enum supportedClasses { SUPPORTED_GRAPHICS_CLASSES_MACRO , SUPPORTED_AUDIO_CLASSES_MACRO , nonSupported };//this doesn't seem to work properly in the cpp

//#ifdef SFML_GRAPHICS_HPP  //wow check out how VS2010 highlights live preprocessor if's and greys out dead code. Only if it had switch case generators and other shit
//	#ifdef SFML_AUDIO_HPP
//		enum supportedClasses { SUPPORTED_GRAPHICS_CLASSES_MACRO , SUPPORTED_AUDIO_CLASSES_MACRO , nonSupported };
//	#else
//		enum supportedClasses { SUPPORTED_GRAPHICS_CLASSES_MACRO , nonSupported };
//	#endif
//#else
//	#ifdef SFML_AUDIO_HPP
//		enum supportedClasses { SUPPORTED_AUDIO_CLASSES_MACRO , nonSupported };
//	#else
//		enum supportedClasses { nonSupported } ;
//		//#error ("Neither SFML graphics or audio are included. This library workaround is useless without them. Defined behavior by the work around is not guarrenteed.")
//		//TODO fucking figure out this pragma warning shit. I don't get why its not fucking showing
//	#endif
//#endif


//#undef SUPPORTED_GRAPHICS_CLASSES_MACRO cannot undef macro because macro in enum doesn't work.
//#undef SUPPORTED_AUDIO_CLASSES_MACRO

//enum supportedClasses{sf_Shader , sf_Texture , sf_Font , sf_Image , sf_Shader, nonSupported};//original preprocessorless enum.


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

enum supportedClasses AssetClassStringToEnum(std::string classString);
enum loadEnum loadAssets(std::vector<struct loadTarget> * paramList);//CSTRTOK VERSION

//TEMPLADIC FUNCS GO BELOW THIS COMMENT
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



}

#endif SFML_LOAD_UTIL_H