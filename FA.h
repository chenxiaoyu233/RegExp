#ifndef __FN_H__
#define __FN_H__

#include "common.h"
#include "State.h"
#include "Edge.h"

enum FAType { NFA, DFA, GNFA };

class FA {
private:
	bool isFree;
	vector<State*> states;
	vector<State*> accept; // accept \in states
	State *start; // start \in states
	FAType type;
public:
	FA(FAType type, bool isFree = true);
	FA(const FA &other, bool isFree = true);
	~FA();
	static FA Concat(const FA &a,const FA &b);
	static FA Union(const FA &a, const FA &b);
	static FA Closet(const FA &a);
	friend FA operator - (const FA &a, const FA &b); // concat
	friend FA operator + (const FA &a, const FA &b); // union
	FA NtoD(); // convert NFA to DFA
};

#endif
