#ifndef BITARRAY_H
#define BITARRAY_H

#include <climits>
#include <string>

class BitArray
{
private:
    // Reference to a single bit for read/write operations.
    // Used by BitArray::operator[] to enable assignment syntax.
    class BitReference {
    private:
        BitArray& bitArray;
        int bitPos;
    public:
        BitReference(BitArray& ba, const int pos)
        :bitArray(ba), bitPos(pos) {};
        operator bool() const;
        BitReference& operator=(bool value);
        BitReference& operator=(const BitReference& other);
    };

    unsigned long* array;
    size_t arraySize;
    size_t capacity;
    const size_t BITS_PER_BLOCK = sizeof(unsigned long) * CHAR_BIT;

    unsigned long getBitMask() const;
    void reallocate(size_t newCapacity, size_t newSize, bool value = false);
    void checkSizeMatch(const BitArray& other) const;

public:
    BitArray() : array(nullptr), arraySize(0), capacity(0) {};
    ~BitArray();
    //Constructs an array storing a number of bits.
    //The first sizeof(long) bits can be initialized using the value parameter.
    explicit BitArray(int size, unsigned long value = 0);
    BitArray(const BitArray& b);
    //Swaps the values of two bit arrays.
    void swap(BitArray& other);
    BitArray& operator=(const BitArray& other);

    bool operator[](int bitPos) const;
    // Returns a bit of reference proxy that enables both reading and writing.
    BitReference operator[](int bitPos);

    size_t size() const;
    bool empty() const;
    void resize(int newNumBits, bool value = false);
    void clear();
    // Adds a new bit to the end of the array.
    // Memory is reallocated if necessary.
    void push_back(bool bit);

    BitArray& set(int bitPos, bool value = true);
    BitArray& set();
    BitArray& reset(int bitPos);
    BitArray& reset();

    bool any() const;
    bool none() const;
    BitArray operator~() const;
    int count() const;
    // Bitwise shift with zero filling.
    BitArray& operator>>=(int shift);
    BitArray& operator<<=(int shift);
    BitArray operator>>(int shift) const;
    BitArray operator<<(int shift) const;
    // Returns a string representation of the array.
    std::string toString() const;
    // Bitwise operations on arrays.
    // Only work on arrays of the same size.
    BitArray& operator&=(const BitArray& b);
    BitArray& operator|=(const BitArray& b);
    BitArray& operator^=(const BitArray& b);
};

BitArray operator&(const BitArray& b1, const BitArray& b2);
BitArray operator|(const BitArray& b1, const BitArray& b2);
BitArray operator^(const BitArray& b1, const BitArray& b2);

bool operator==(const BitArray& b1, const BitArray& b2);
bool operator!=(const BitArray& b1, const BitArray& b2);

#endif