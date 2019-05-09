#include "MyBitSet.h"

MyBitSet::MyBitSet(size_t BitNum) {
	WordNum = (BitNum >> 5) + ((BitNum & mask) > 0) ;
	field = new uint32_t[WordNum];
	memset(field, 0, sizeof(uint32_t) * WordNum);
}

MyBitSet::MyBitSet(const MyBitSet &other) {
	WordNum = other.WordNum;
	field = new uint32_t[WordNum];
	memcpy(field, other.field, sizeof(uint32_t) * WordNum);
}

void MyBitSet::operator = (const MyBitSet &other) {
    if (WordNum != other.WordNum) {
        WordNum = other.WordNum;
        delete[] field;
        field = new uint32_t[WordNum];
    }
    memcpy(field, other.field, sizeof(uint32_t) * WordNum);
}

MyBitSet::~MyBitSet() {
	delete[] field;
 }

bool MyBitSet::operator [] (size_t idx) const {
	return (field[idx >> 5] >> (idx & mask)) & 1;
}

bool MyBitSet::operator < (const MyBitSet &other) const {
	for (size_t i = 0; i < WordNum; i++) {
        if (field[i] != other.field[i]) {
            if (field[i] < other.field[i]) return true;
            else return false;
        }
	}
	return false;
}

bool MyBitSet::operator == (const MyBitSet &other) {
	for (size_t i = 0; i < WordNum; i++) {
		if (field[i] != other.field[i]) return false;
	}
	return true;
}

MyBitSet MyBitSet::operator | (const MyBitSet &other) {
	MyBitSet ret(*this);
	for (size_t i = 0; i < WordNum; i++) {
		ret.field[i] |= other.field[i];
	}
	return ret;
}

MyBitSet MyBitSet::operator & (const MyBitSet &other) {
	MyBitSet ret(*this);
	for (size_t i = 0; i < WordNum; i++) {
		ret.field[i] &= other.field[i];
	}
	return ret;
}

void MyBitSet::Set(size_t idx) {
	field[idx >> 5] |= (1ul << (idx & mask));
}

void MyBitSet::Reset(size_t idx) {
	field[idx >> 5] &= ~(1ul << (idx & mask));
}

void MyBitSet::Log() {
    for (size_t i = 0; i < WordNum; i++) {
        for (int j = 0; j < 32; j++) fprintf(stderr, "%u", (field[i] >> j) & 1);
    }
}
