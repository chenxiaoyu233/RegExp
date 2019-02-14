#ifndef __MY_BITSET_H__
#define __MY_BITSET_H__

#include "common.h"

class MyBitSet {
private:
	size_t WordNum;
	uint_64 *field;
	const static uint_64 mask = 63;
public:
	MyBitSet(size_t BitNum);
	MyBitSet(const MyBitSet& other);
	~MyBitSet();
	bool operator [] (size_t idx);
	bool operator < (const MyBitSet &other);
	bool operator == (const MyBitSet &other);
	MyBitSet operator | (const MyBitSet &other);
	MyBitSet operator & (const MyBitSet &other);
	void Set(size_t idx);
	void Reset(size_t idx);
};

#endif
