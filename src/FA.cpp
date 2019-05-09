#include "FA.h"

FA::FA(FAType type, bool isFree): type(type), isFree(isFree) {
	states.clear(); accept.clear();
	start = NULL;
}

void FA::copyFrom(const FA &other) {
    states.clear(); accept.clear();
    type = other.type;
    
    // tmp variable to store the edge info
    map <State*, size_t> mp; mp.clear();
    vector<pair<string, pair<size_t, size_t> > > edges; edges.clear();
    
    // map the address to array index
    for (size_t i = 0; i < other.states.size(); i++) mp[other.states[i]] = i;
    
    // copy the state from original FA
    for (auto s: other.states){
        State *tmp = new State(*s);
        states.push_back(tmp);
        if (tmp -> type == StateType::accept) accept.push_back(tmp);
    }
    start = states[mp[other.start]];
    
    // using the array index to collect edge info
    for (auto s: other.states){
        for (auto e: s -> trans) {
            edges.push_back(make_pair(e.label, make_pair(mp[e.from], mp[e.to])));
        }
    }
    
    // rebuild all the edges
    for (auto e: edges) {
        State *from = states[e.second.first], *to = states[e.second.second];
        string label = e.first;
        (from -> trans).push_back(Edge(label, from, to));
    }
}

FA::FA(const FA &other, bool isFree): isFree(isFree) {
    this -> copyFrom(other);
}

void FA::operator = (const FA &other) {
    this -> isFree = false;
    this -> copyFrom(other);
}

FA::~FA() {
	if (!isFree) return;
	for (auto s: states) {
		delete s;
	}
}

FA FA::operator + (const FA &other) {
	return Concat(*this, other);
}

FA FA::operator | (const FA &other) {
	return Union(*this, other);
}

FA FA::Concat(const FA &a, const FA &b) {
	//assert(a.type == FAType::NFA && b.type == FAType::NFA);
	FA at(a, false), bt(b, false), c(NFA);

	// build new FA based on at and bt
	for (auto s: at.accept) {
		(s -> trans).push_back(Edge("", s, bt.start));
		s -> type = StateType::normal;
	}

	// copy states from at and bt
	for (auto s: at.states) c.states.push_back(s);
	for (auto s: bt.states) c.states.push_back(s);
	for (auto s: bt.accept) c.accept.push_back(s);
	c.start = at.start;

	c.type = FAType::NFA;

	return c;
}

FA FA::Union(const FA &a, const FA &b) {
	//assert(a.type == FAType::NFA && b.type == FAType::NFA);
	FA at(a, false), bt(b, false), c(NFA);

	// build new FA based on at, bt and c
	State *st = new State(StateType::normal);
	c.states.push_back(st);
	(st -> trans).push_back(Edge("", st, at.start));
	(st -> trans).push_back(Edge("", st, bt.start));

	// copy states from at and bt
	for (auto s: at.states) c.states.push_back(s);
	for (auto s: bt.states) c.states.push_back(s);
	for (auto s: at.accept) c.accept.push_back(s);
	for (auto s: bt.accept) c.accept.push_back(s);

	c.type = FAType::NFA;

	return c;
}

FA FA::Closet(const FA &a) {
	//assert(a.type == FAType::NFA);
	FA c(a, false);

	// build new FA based on c
	State *st = new State(StateType::accept);
	c.states.push_back(st);
	(st -> trans).push_back(Edge("", st, c.start));
	for (auto s: c.accept) {
		(s -> trans).push_back(Edge("", s, c.start));
	}
	c.accept.push_back(st);
	c.start = st;

	c.type = FAType::NFA;

	return c;
}

void FA::genMapFromAddressToIndex() {
	mpa2i.clear();
	for(size_t i = 0; i < states.size(); i++)
		mpa2i[states[i]] = i;
}

void FA::extend(MyBitSet &st, State *s, string label) {
	MyBitSet tmp(states.size()); // tmp BitSet to store the states
	queue<State*> q;
	q.push(s); tmp.Set(mpa2i[s]);

	while(!q.empty()) {
		State *tt = q.front(); q.pop();
		for(auto &e: tt -> trans) {
			if (e.label == label) {
				if(!tmp[mpa2i[e.to]]) {
					q.push(e.to);
					tmp.Set(mpa2i[e.to]);
				}
			}
		}
	}
    
	st = st | tmp;
}

void FA::collectPossibleLabel(const MyBitSet &st, set<string> &pls) {
	pls.clear();
	for (size_t i = 0; i < states.size(); i++) if (st[i]) {
		for (auto e: states[i] -> trans) {
			if (!pls.count(e.label)) pls.insert(e.label);
		}
	}
}

MyBitSet FA::transFromSet(const MyBitSet &st, string label) {
	MyBitSet nxt(states.size());
	for (size_t i = 0; i < states.size(); i++) if(st[i]) {
		for (auto e: states[i] -> trans) if (e.label == label) {
			if (!nxt[mpa2i[e.to]]) {
				nxt.Set(mpa2i[e.to]);
				extend(nxt, e.to, "");
			}
		}
	}
	return nxt;
}

FA FA::NtoD() {
	// DS for bfs
	queue<MyBitSet> q;
	FA dfa(FAType::DFA);
	map<MyBitSet, State*> mp;

	// map address to array index
	genMapFromAddressToIndex();

	// initial start state
	MyBitSet s(states.size());
	extend(s, start, ""); // get the initial state
	q.push(s);
	dfa.start = new State(StateType::normal);
	dfa.states.push_back(dfa.start);
	mp[s] = dfa.start;

	// bfs
	while (!q.empty()) {
		MyBitSet tt = q.front(); q.pop();
        /* Debug
        if (!mp.count(tt)) {
            tt.Log(); fprintf(stderr, " tt == tt: %d, mp.count(tt): %d\n", tt == tt, mp.count(tt));
            cerr << "WTF???" << endl;
            for (auto p: mp) {
                MyBitSet tmp = p.first;
                tmp.Log(); fprintf(stderr, " tmp == tt: %d, mp.count(tmp): %d\n", tmp == tt, mp.count(tmp));
            }
            exit(234);
        }*/
		set<string> pls; // possible label set
		collectPossibleLabel(tt, pls);
		for (auto label: pls) if(label != "") {
			MyBitSet nxtBS = transFromSet(tt, label);
			if (!mp.count(nxtBS)) {
				State *tmp = new State(StateType::normal);
				mp[nxtBS] = tmp;
				dfa.states.push_back(tmp);
				q.push(nxtBS);
			}
			(mp[tt] -> trans).push_back(Edge(label, mp[tt], mp[nxtBS]));
		}
	}

	// handle the accept states
	for (auto &p: mp) {
		bool isAccept = false;
		for (auto sp: accept) if (p.first[mpa2i[sp]]) {
			isAccept = true;
			break;
		}
		if (isAccept) {
			p.second -> type = StateType::accept;
			dfa.accept.push_back(p.second);
		}
	}
    
    // collect the info vector
    for (auto &p: mp) {
        (p.second -> info).clear();
        for (size_t i = 0; i < states.size(); i++) if (p.first[i]) {
            for (auto it: states[i] -> info) {
                (p.second -> info).push_back(it);
            }
        }
    }
	
	dfa.type = FAType::DFA;
	return dfa;
}

FA FA::CharSetNFA(string chars) {
	FA fa(FAType::NFA);
	fa.start = new State(StateType::normal);
	fa.states.push_back(fa.start);
	State *acc = new State(StateType::accept);
	fa.accept.push_back(acc);
    fa.states.push_back(acc);

	for (size_t i = 0; i < chars.length(); i++) {
		(fa.start -> trans).push_back(Edge(string() + chars[i], fa.start, acc));
	}

	return fa;
}

FA FA::PureStringNFA(string str) {
    FA fa(FAType::NFA);
    fa.start = new State(StateType::normal);
    fa.states.push_back(fa.start);
    State *cur, *last = fa.start;
    for (size_t i = 0; i < str.length(); i++) {
        cur = new State(StateType::normal);
        fa.states.push_back(cur);
        (last -> trans).push_back(Edge(string() + str[i], last, cur));
        if (i + 1 == str.length()) cur -> type = StateType::accept;
        last = cur;
    }
    return fa;
}

FA FA::EmptyStr() {
	FA fa(FAType::NFA);
	fa.start = new State(StateType::accept);
	fa.accept.push_back(fa.start);
	fa.states.push_back(fa.start);
	return fa;
}

FA FA::EmptySet() {
    FA fa(FAType::NFA);
    fa.start = new State(StateType::normal);
    fa.states.push_back(fa.start);
    return fa;
}

State* FA::Next(State* cur, string label) {
	for (auto e: cur -> trans) if (e.label == label) {
		return e.to;
	}
	return NULL;
}

void FA::logState() {
    for (auto s: states) {
        fprintf(stderr, "%d;\n", mpa2i[s]);
    }
    for (auto s: accept) {
        fprintf(stderr, "%d [shape = doublecircle];\n", mpa2i[s]);
    }
}

void FA::logEdge(State* s) {
    for (auto e: s -> trans) {
        fprintf(stderr, "%d -> %d [ label = \"%s\" ];\n", mpa2i[e.from], mpa2i[e.to], e.label.c_str());
    }
}

void FA::Log() {
    genMapFromAddressToIndex();
    fprintf(stderr, "digraph G {\n");
    logState();
    for (auto s: states) logEdge(s);
    fprintf(stderr, "}\n");
}
