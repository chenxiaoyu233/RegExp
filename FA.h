#ifndef __FN_H__
#define __FN_H__

#include "common.h"
#include "State.h"
#include "Edge.h"
#include "MyBitSet.h"

enum FAType { NFA, DFA, GNFA };

class FA {
private:
	bool isFree;
	vector<State*> states;
	vector<State*> accept; // accept \in states
	State *start; // start \in states
	FAType type;

	// map from address to array index
	map<State*, size_t> mpa2i; 
	void genMapFromAddressToIndex();

	// extend st by the edges from s with label
	void extend(MyBitset &st, const State *s, string label);

	// collect all the possible label from a Set of States' transformation
	void collectPossibleLabel(const MyBitSet &st, set<string> &pls);

	// calc transformation from a set of states with a label
	MyBitSet transFromSet(const MyBitSet &st, string label);
	
public:
	FA(FAType type, bool isFree = true);
	FA(const FA &other, bool isFree = true);
	~FA();
	static FA Concat(const FA &a,const FA &b);
	static FA Union(const FA &a, const FA &b);
	static FA Closet(const FA &a);
	static FA CharSetNFA(string chars);
	static FA EmptyStr();
	friend FA operator + (const FA &a, const FA &b); // concat
	friend FA operator | (const FA &a, const FA &b); // union
	FA NtoD(); // convert NFA to DFA
	State* Next(State* cur, string label);
};

#endif
