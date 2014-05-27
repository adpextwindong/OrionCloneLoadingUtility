#include "stdafx.h"
#include "sfmlLoadUtil.h"
#include <vector>
#pragma once
class OCLULoader//TODO switch over to cleaner class instead of struct + function
{
public:
	OCLULoader(std::vector<struct OCLU::loadTarget> targetlist);
	OCLULoader(void);
	~OCLULoader(void);
	//void pushWholeList(std::vector<struct OCLU::loadTarget> targetlist); this should be the constructor. If you can manually add to a vector and push that list to it why not do it at construction 
	//void appendWholeList(std::vector<struct OCLU::loadTarget> targetlist);
	void feedTarget(struct OCLU::loadTarget target);
	void load();
	const std::vector<struct OCLU::loadTarget> * const seeList();
private:
	std::vector<struct OCLU::loadTarget> loadTarglist;
	//bool firstLoad;//handled by function
};
