#ifndef _PARSE_H_
#define _PARSE_H_

#include <string>
#include <list>
#include "../API/API.hpp"

enum Action {
    INSV,
    DELV,
    SELV,
    CTBL,
    DTBL,
    CIDX,
    DIDX,
    LEAVE,
    TOTAL_ACTION
};

class Plan {
public:
	Plan(WrapperList wlist, std::string tname, 
		Action acti): tname(tname), acti(acti), wlist(wlist) {}
	WrapperList wlist;
	std::string tname;
	Action acti;
};
typedef std::list<Plan> PlanList;

PlanList& parse(std::string str, int action = 0);


void flushzz();

#endif // parse.h
