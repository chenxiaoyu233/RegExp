#include "RegExp.h"
#include "common.h"

void printsub(string str, int begin, int end) {
	for (int i = begin; i <= end; i++)  {
		printf("%c", str[i]);
	}
	puts("");
}

void test() {
    string rexp = "\\[ab\\]\\{, \\} \\+ \\[c\\]\\{1, 1\\} \\| \\[c\\]\\{2, 2\\}";
	string text = "abccababcdefabababefacb";
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
