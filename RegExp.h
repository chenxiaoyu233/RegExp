#ifndef __REG_EXP_H__
#define __REG_EXP_H__

#include "common.h"
#include "FA.h"

// <RegExp1> ::= <RegExp2> "\|" <RegExp1> |
//               <RegExp2>
// <RegExp2> ::= <CharSetExp> "\{" <RangeExp> "\}" |
//               <RegExp3> "\{" <RangeExp> "\}" |
//               <CharSetExp> |
//               <RegExp3>
// <RangeExp> ::= <Num> ", " | ", " <Num> | <Num> ", " <Num>
// <CharSetExp> ::= "\[" <Chars> "\]"
// <RegExp3> ::= "\(" <RegExp1> "\)"

class RegExp {
private:
	FA dfa;
	string exp;
	size_t pt;
	FA regExp1();
	FA regExp2();
	FA regExp3();
	pair<int, int> rangeExp();
	int numExp();
	FA charSetExp();
	int matchAtPoint(string str, size_t point);
public:
	RegExp(string exp);
	vector<pair<int, int> > Match(string str);
};

#endif
