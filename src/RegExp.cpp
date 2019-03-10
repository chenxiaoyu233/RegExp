#include "RegExp.h"

#define HandleSpace \
	else if (exp[pt] == ' ' || exp[pt] == '\t' || exp[pt] == '\n') { pt++; }
	
RegExp::RegExp(string exp): exp(exp), dfa(FAType::DFA) {
	pt = 0;
	FA nfa(regExp1());
	dfa = nfa.NtoD();
}

vector<pair<int, int> > RegExp::Match(string str){
	vector<pair<int, int> > ret;
	for (size_t i = 0; i < str.length(); i++) {
		int cur = matchAtPoint(str, i);
		if (cur >= i) ret.push_back(make_pair(i, cur));
	}
	return ret;
}

FA RegExp::DFA() {
    return dfa;
}

int RegExp::matchAtPoint(string str, size_t point) {
	int ret = point - 1;
	State *cur = dfa.start;
	for (size_t pt = point; pt < str.length(); ++pt) {
		cur = dfa.Next(cur, string() + str[pt]);
        if (cur == NULL) break;
        if (cur -> type == StateType::accept) {
            ret = pt;
        }
	}
    return ret;
}

FA RegExp::regExp1() {
	FA left = regExpCat();
	while( pt + 1 < exp.length() ) {
		if (exp[pt] == '\\' && exp[pt+1] == '|'){
			pt += 2;
			left = left | regExpCat();
		} HandleSpace else {
            break;
		}
    }
    return left;
}

FA RegExp::regExpCat() {
    FA left = regExp2();
    while ( pt + 1 < exp.length() ) {
        if (exp[pt] == '\\' && exp[pt+1] == '+') {
            pt += 2;
            left = left + regExp2();
        } HandleSpace else {
            break;
        }
    }
    return left;
}

FA RegExp::regExp2() {
	FA left(FAType::NFA);
	pair<int, int> right;
	bool lFlag = false, rFlag = false;
	while( pt + 1 < exp.length() ) {
		if (!lFlag && exp[pt] == '\\' && exp[pt+1] == '(') {
			lFlag = true;
			left = regExp3();
		} else if (!lFlag && exp[pt] == '\\' && exp[pt+1] == '[') {
			lFlag = true;
			left = charSetExp();
		} else if (!lFlag && exp[pt] == '\\' && exp[pt+1] == '$') {
			lFlag = true;
			left = stringExp();
		} else if (!rFlag && exp[pt] == '\\' && exp[pt+1] == '{') {
			pt += 2;
			rFlag = true;
			right = rangeExp();
		} else if (lFlag && rFlag && exp[pt] == '\\' && exp[pt+1] == '}') {
			pt += 2;
			FA cur(FA::EmptyStr());
			for (int i = 1; i <= right.first; ++i) {
				cur = cur + left;
			}
			if (right.second < 0) {
				return cur + FA::Closet(left);
			} else {
				FA ret(cur);
				for (int i = right.first + 1; i <= right.second; i++) {
					cur = cur + left;
					ret = ret | cur;
				}
				return ret;
			}
		} HandleSpace
		else if (lFlag) {
			break;
		} else {
			fprintf(stderr, "wrong syntax\n");
			exit(233);
		}
	}
    return left;
}

FA RegExp::regExp3() {
	FA left(FAType::NFA);
	bool vFlag = false;
	while( pt + 1 < exp.length() ) {
		if (!vFlag && exp[pt] == '\\' && exp[pt+1] == '(') {
			vFlag = true;
			pt += 2;
			left = regExp1();
		} else if (vFlag && exp[pt] == '\\' && exp[pt+1] == ')') {
			pt += 2;
            break;
		} HandleSpace else {
			fprintf(stderr, "wrong syntax\n");
			exit(233);
		}
	}
    return left;
}

int RegExp::numExp() {
	int ret = -1;
	bool inner = false;
	while( pt < exp.length() ) {
		if (!inner && exp[pt] >= '0' && exp[pt] <= '9') {
			ret = exp[pt] - '0';
			inner = true;
            pt++;
		} else if (inner && exp[pt] >= '0' && exp[pt] <= '9') {
			ret *= 10;
			ret += exp[pt] - '0';
            pt++;
		} else if (!inner && (exp[pt] == ' ' || exp[pt] == '	' || exp[pt] == '\n')) {
            pt++;
		} else {
            break;
		}
	}
    return ret;
}

pair<int, int> RegExp::rangeExp() {
	int left = numExp();
	int right = -1;
	while( pt < exp.length() ) {
		if (exp[pt] == ',') {
			pt += 1;
			right = numExp();
		} HandleSpace else {
            break;
		}
	}
    return make_pair(max(0, left), right);
}

FA RegExp::charSetExp () {
	string str = "";
	bool inner = false;
	while( pt + 1 < exp.length() ) {
		if (!inner && (exp[pt] == ' ' || exp[pt] == '\t' || exp[pt] == '\n')) {
			pt++;
		} else if (!inner && exp[pt] == '\\' & exp[pt+1] == '[') {
			inner = true;
			pt += 2;
		} else if (inner && (exp[pt] != '\\' || exp[pt+1] != ']')) {
			str += exp[pt];
            pt++;
		} else if (inner && exp[pt] == '\\' && exp[pt+1] == ']') {
            pt += 2;
            break;
		} else {
			fprintf(stderr, "wrong syntax\n");
			exit(233);
		}
	}
    return FA::CharSetNFA(str);
}

FA RegExp::stringExp() {
	FA left(FAType::NFA);
	bool inner = false;
	while (pt + 1 < exp.length()) {
		if (!inner && (exp[pt] == ' ' || exp[pt] == '\t' || exp[pt] == '\n')) {
		} else if (!inner && exp[pt] == '\\' && exp[pt+1] == '$') {
			pt += 2;
			inner = true;
		} else if (inner && (exp[pt] != '\\' && exp[pt+1] != '$')) {
			left = left + FA::CharSetNFA(string() + exp[pt]);
		} else if (inner && (exp[pt] == '\\' && exp[pt+1] == '$')) {
			pt += 2;
			break;
		} else {
			fprintf(stderr, "wrong syntax\n");
			exit(233);
		}
	}
	return left;
}
