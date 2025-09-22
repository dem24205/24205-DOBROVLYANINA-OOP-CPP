#ifndef BITARRAY_H
#define BITARRAY_H

#include <climits>
#include <string>

class BitArray
{
private:
	unsigned long* array;
	const size_t BITS_PER_BLOCK = sizeof(unsigned long) * CHAR_BIT;
	size_t arraySize;
    size_t capacity;
	unsigned long getBitMask() const;
	void reallocate(size_t newCapacity, size_t newSize, bool value = false);
	void checkSizeMatch(const BitArray& other) const;
public:
	BitArray() : array(nullptr), arraySize(0), capacity(0) {};
	~BitArray();
	explicit BitArray(int size, unsigned long value = 0);
	BitArray(const BitArray& b);

	void swap(BitArray& other);
	BitArray& operator=(const BitArray& other);

	bool operator[](int bitPos) const;

	size_t size() const;
	bool empty() const;

	void resize(int newNumBits, bool value = false);
	void clear();
	void push_back(bool bit);

	BitArray& set(int bitPos, bool value = true);
	BitArray& set();
	BitArray& reset(int bitPos);
	BitArray& reset();

	bool any() const;
	bool none() const;

	BitArray operator~() const;

	int count() const;

	BitArray& operator>>=(int shift);
	BitArray& operator<<=(int shift);
	BitArray operator>>(int shift) const;
	BitArray operator<<(int shift) const;

	std::string toString() const;

	BitArray& operator&=(const BitArray& b);
	BitArray& operator|=(const BitArray& b);
	BitArray& operator^=(const BitArray& b);

};

BitArray operator&(const BitArray& b1, const BitArray& b2);
BitArray operator|(const BitArray& b1, const BitArray& b2);
BitArray operator^(const BitArray& b1, const BitArray& b2);

bool operator==(const BitArray & b1, const BitArray & b2);
bool operator!=(const BitArray & b1, const BitArray & b2);

#endif