#ifndef __REG_EXP_H__
#define __REG_EXP_H__

#include "common.h"
#include "FA.h"

// <RegExp1> ::= <RegExp+> "\|" <RegExp1> |
//               <RegExp+>
// <RegExp+> ::= <RegExp2> "\+" <RegExp+> |
//               <RegExp2>
// <RegExp2> ::= <CharSetExp> "\{" <RangeExp> "\}" |
//               <RegExp3> "\{" <RangeExp> "\}" |
//               <StringExp> "\{" <RangeExp> "\}" |
//               <StringExp> |
//               <CharSetExp> |
//               <RegExp3>
// <StringExp> ::= "\$" a string "\$"
// <RangeExp> ::= <Num> ", " | ", " <Num> | <Num> ", " <Num>
// <CharSetExp> ::= "\[" <Chars> "\]"
// <RegExp3> ::= "\(" <RegExp1> "\)"

class RegExp {
private:
	FA dfa;
	string exp;
	size_t pt;
	FA regExp1();
    FA regExpCat();
	FA regExp2();
	FA regExp3();
	pair<int, int> rangeExp();
	int numExp();
	FA charSetExp();
	FA stringExp();
	int matchAtPoint(string str, size_t point);
public:
	RegExp(string exp);
	vector<pair<int, int> > Match(string str);
    FA DFA();
};

#endif
