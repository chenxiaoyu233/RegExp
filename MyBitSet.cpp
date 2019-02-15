#include "MyBitSet.h"

MyBitSet::MyBitSet(size_t BitNum) {
	WordNum = (BitNum >> 6) + ((BitNum & mask) > 0) ;
	field = new uint64_t[WordNum];
	memset(field, 0, sizeof(uint64_t) * WordNum);
}

MyBitSet::MyBitSet(const MyBitSet &other) {
	WordNum = other.WordNum;
	field = new uint64_t[WordNum];
	memcpy(field, other.field, sizeof(uint64_t) * WordNum);
}

MyBitSet::~MyBitSet() {
	delete[] field;
 }

bool MyBitSet::operator [] (size_t idx) const {
	return (field[idx >> 6] >> (idx & mask)) & 1;
}

bool MyBitSet::operator < (const MyBitSet &other) const {
	for (size_t i = 0; i < WordNum; i++) {
		if (field[i] < other.field[i]) {
			return true;
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
	field[idx >> 6] |= (1ul << (idx & mask));
}

void MyBitSet::Reset(size_t idx) {
	field[idx >> 6] &= ~(1ul << (idx & mask));
}
