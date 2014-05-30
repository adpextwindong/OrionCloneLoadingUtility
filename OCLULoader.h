#include <SFML\System.hpp>
#include "sfmlLoadUtil.h"

#ifndef OCLU_LOADER_H
#define OCLU_LOADER_H
namespace OCLU{

struct LoadScreenStats{
	unsigned int filesLoaded;
	unsigned int totalFiles;
	float secondsElapsed;

	LoadScreenStats(unsigned int _filesLoaded,unsigned int _totalFiles,sf::Clock * pClock):
		filesLoaded(_filesLoaded),
		totalFiles(_totalFiles),
		secondsElapsed(pClock->getElapsedTime().asSeconds())
		{
		};
};
class OCLULoader{//TODO switch over to cleaner class instead of struct + function
public://this class should just be a simple little wrapper for std::vector<struct OCLU::loadTarget>, nothing more
	void pushWholeList(std::vector<struct OCLU::loadTarget> targetlist);
	void feedTarget(struct OCLU::loadTarget target);
	const struct OCLU::LoadScreenStats load();
	const std::vector<struct OCLU::loadTarget> * const seeList();
	OCLULoader();
	~OCLULoader();
private:
	std::vector<struct OCLU::loadTarget> loadTarglist;
	//bool firstLoad;//handled by function
};

}
#endif OCLU_LOADER_H