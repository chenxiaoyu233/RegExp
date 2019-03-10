#include "RegExp.h"
#include "common.h"

void printsub(string str, int begin, int end) {
	for (int i = begin; i <= end; i++)  {
		printf("%c", str[i]);
	}
	puts("");
}

void test() {
    string rexp = "\\[ \t\n\\]\\{, \\}";
	string text = "abccababcdefabababe\t\nfacb";
	RegExp matcher(rexp);
	auto ret = matcher.Match(text);
	for (auto pr: ret) {
		printsub(text, pr.first, pr.second);
	}
}

int main() {
	test();
	return 0;
}
