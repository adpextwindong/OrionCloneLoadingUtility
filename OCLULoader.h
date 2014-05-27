#ifndef OCLULOADER_H
#define OCLULOADER_H
#include <vector>
#include "sfmlLoadUtil.h"
class OCLULoader{//TODO switch over to cleaner class instead of struct + function
public://this class should just be a simple little wrapper for std::vector<struct OCLU::loadTarget>, nothing more
	void pushWholeList(std::vector<struct OCLU::loadTarget> targetlist);
	void feedTarget(struct OCLU::loadTarget target);
	void load();
	const std::vector<struct OCLU::loadTarget> * const seeList();
	OCLULoader(void);
	~OCLULoader(void);
private:
	std::vector<struct OCLU::loadTarget> loadTarglist;
	//bool firstLoad;//handled by function
};
#endif


