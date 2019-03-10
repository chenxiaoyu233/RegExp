#ifndef __FN_H__
#define __FN_H__

#include "common.h"
#include "State.h"
#include "Edge.h"
#include "MyBitSet.h"

enum FAType { NFA, DFA, GNFA };

class FA {
public:
	bool isFree;
	vector<State*> states;
	vector<State*> accept; // accept \in states
	State *start; // start \in states
	FAType type;

	// map from address to array index
	map<State*, size_t> mpa2i; 
	void genMapFromAddressToIndex();

	// extend st by the edges from s with label
	void extend(MyBitSet &st, State *s, string label);

	// collect all the possible label from a Set of States' transformation
	void collectPossibleLabel(const MyBitSet &st, set<string> &pls);

	// calc transformation from a set of states with a label
	MyBitSet transFromSet(const MyBitSet &st, string label);
	
	FA(FAType type, bool isFree = true);
    void copyFrom(const FA &other);
	FA(const FA &other, bool isFree = true);
    void operator = (const FA &other);
	~FA();
	static FA Concat(const FA &a,const FA &b);
	static FA Union(const FA &a, const FA &b);
	static FA Closet(const FA &a);
	static FA CharSetNFA(string chars);
    static FA PureStringNFA(string str);
	static FA EmptyStr();
    static FA EmptySet();
	FA operator + (const FA &other); // concat
	FA operator | (const FA &other); // union
	FA NtoD(); // convert NFA to DFA
	State* Next(State* cur, string label);
};

#endif
