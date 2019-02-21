#include "RegExp.h"

#define HandleSpace \
else if (exp[pt] != ' ' && exp[pt] != '	' && exp[pt] != '\n') {}
	
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

int RegExp::matchAtPoint(string str, size_t point) {
	int ret;
	State *cur = dfa.start;
	for (ret = point; ret < str.length(); ++ret) {
		cur = dfa.Next(cur, string() + str[ret]);
		if (cur == NULL) return point-1;
		if (cur -> type == StateType::accept) return ret;
	}
	return point-1;
}

FA RegExp::regExp1() {
	FA left = regExp2();
	for (; pt+1 < exp.length(); ++pt) {
		if (exp[pt] == '\\' && exp[pt+1] == '|'){
			pt += 2;
			left = left | regExp2();
		} HandleSpace else {
			return left;
		}
	}
}

FA RegExp::regExp2() {
	FA left(FAType::NFA);
	pair<int, int> right;
	bool lFlag = false, rFlag = false;
	for (; pt + 1 < exp.length(); ++pt) {
		if (!lFlag && exp[pt] == '\\' && exp[pt+1] == '(') {
			lFlag = true;
			left = regExp3();
		} else if (!lFlag && exp[pt] == '\\' && exp[pt+1] == '[') {
			lFlag = true;
			left = charSetExp();
		} else if (!rFlag && exp[pt] == '\\' && exp[pt+1] == '{') {
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
		} HandleSpace else {
			fprintf(stderr, "wrong syntax\n");
			exit(233);
		}
	}
}

FA RegExp::regExp3() {
	FA left(FAType::NFA);
	bool vFlag = false;
	for (; pt + 1 < exp.length(); ++pt) {
		if (!vFlag && exp[pt] == '\\' && exp[pt+1] == '(') {
			vFlag = true;
			pt += 2;
			left = regExp1();
		} else if (vFlag && exp[pt] == '\\' && exp[pt+1] == ')') {
			pt += 2;
			return left;
		} HandleSpace else {
			fprintf(stderr, "wrong syntax\n");
			exit(233);
		}
	}
}

int RegExp::numExp() {
	int ret = -1;
	bool inner = false;
	for (; pt < exp.length(); ++pt) {
		if (!inner && exp[pt] >= '0' && exp[pt] <= '9') {
			ret = exp[pt] - '0';
			inner = true;
		} else if (inner && exp[pt] >= '0' && exp[pt] <= '9') {
			ret *= 10;
			ret += exp[pt] - '0';
		} else if (!inner && (exp[pt] == ' ' || exp[pt] == '	' || exp[pt] == '\n')) {
		} else {
			return ret;
		}
	}
}

pair<int, int> RegExp::rangeExp() {
	int left = numExp();
	int right = -1;
	for (; pt < exp.length(); ++pt) {
		if (exp[pt] == ',') {
			pt += 1;
			right = numExp();
		} HandleSpace else {
			return make_pair(max(0, left), right);
		}
	}
}

FA RegExp::charSetExp () {
	string str = "";
	bool inner = false;
	for (; pt+1 < exp.length(); ++pt) {
		if (!inner && (exp[pt] == ' ' || exp[pt] == '	' || exp[pt] == '\n')) {
		} else if (!inner && exp[pt] == '\\' & exp[pt+1] == '[') {
			inner = true;
			++pt;
		} else if (inner && (exp[pt] != '\\' || exp[pt+1] != ']')) {
			str += exp[pt];
		} else if (inner && exp[pt] == '\\' && exp[pt+1] == ']') {
			return FA::CharSetNFA(str);
		} else {
			fprintf(stderr, "wrong syntax\n");
			exit(233);
		}
	}
}
