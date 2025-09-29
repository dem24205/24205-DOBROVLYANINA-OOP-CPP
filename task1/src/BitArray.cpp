#include "BitArray.h"
#include <algorithm>
#include <stdexcept>

static void checkArgument(const int arg) {
    if (arg < 0) {
        throw std::invalid_argument("Argument cannot be negative");
    }
}

static void checkBitPos(const int bitPos, const size_t arraySize) {
    if (bitPos < 0 || static_cast<size_t>(bitPos) >= arraySize) {
        throw std::out_of_range("BitArray index out of range");
    }
}

BitArray::~BitArray() {
    delete[] array;
}

unsigned long BitArray::getBitMask() const {
    const size_t extra = arraySize % BITS_PER_BLOCK;
    return (extra == 0) ? ~0UL : (1UL << extra) - 1;
}

void BitArray::checkSizeMatch(const BitArray &other) const{
    if (other.size() != arraySize) {
        throw std::invalid_argument("Size mismatch");
    }
}

BitArray::BitArray(const int size, const unsigned long value) {
    checkArgument(size);

    if (size == 0) {
        arraySize = 0;
        capacity = 0;
        array = nullptr;
        return;
    }
    arraySize = size;
    capacity = (size + BITS_PER_BLOCK - 1) / BITS_PER_BLOCK;
    array = new unsigned long[capacity]();

    if (capacity > 0) {
        array[0] = value & getBitMask();
    }
}

BitArray::BitArray(const BitArray &b)
: array(nullptr) , arraySize(b.arraySize), capacity(b.capacity){
    if (capacity > 0) {
        array = new unsigned long[capacity];
        for (size_t i = 0; i < capacity; ++i) {
            array[i] = b.array[i];
        }
    }
}

bool BitArray::operator[](const int bitPos) const {
    if (arraySize == 0) {
        throw std::out_of_range("BitArray is empty");
    }
    checkBitPos(bitPos, arraySize);
    const size_t blockIdx = bitPos / BITS_PER_BLOCK;
    const size_t bitOffset = bitPos % BITS_PER_BLOCK;
    return (array[blockIdx] >> bitOffset) & 1;
}

size_t BitArray::size() const {
    return arraySize;
}

bool BitArray::empty() const {
    return arraySize == 0;
}

void BitArray::clear() {
    delete[] array;
    array = nullptr;
    arraySize = 0;
    capacity = 0;
}

void BitArray::reallocate(const size_t newCapacity, const size_t newSize, const bool value) {
    auto* newArray = new unsigned long[newCapacity]();
    if (array != nullptr) {
        for (size_t i = 0; i < capacity; ++i) {
            newArray[i] = array[i];
        }
        delete[] array;
    }
    array = newArray;
    if (value) {
        for (size_t i = capacity; i < newCapacity; ++i) {
            newArray[i] = ~0UL;
        }
        newArray[newCapacity - 1] &= getBitMask();
    }
    capacity = newCapacity;
    arraySize = newSize;
}

BitArray &BitArray::set(const int bitPos, const bool value) {
    checkBitPos(bitPos, arraySize);
    const size_t blockIdx = bitPos / BITS_PER_BLOCK;
    const size_t bitOffset = bitPos % BITS_PER_BLOCK;
    if (value) {
        array[blockIdx] |= (1UL << bitOffset);
    }
    else {
        array[blockIdx] &= ~(1UL << bitOffset);
    }
    return *this;
}

BitArray &BitArray::set() {
    if (array == nullptr) return *this;
    for (size_t i = 0; i < capacity; ++i) {
        array[i] = ~0UL;
    }
    array[capacity - 1] &= getBitMask();
    return *this;
}

BitArray &BitArray::reset(const int bitPos) {
    return set(bitPos,false);
}

BitArray &BitArray::reset() {
    if (array == nullptr) return *this;
    for (size_t i = 0; i < capacity; ++i) {
        array[i] = 0UL;
    }
    return *this;
}

void BitArray::resize(const int newNumBits, const bool value) {
    checkArgument(newNumBits);

    const auto newSize = static_cast<size_t>(newNumBits);
    const auto oldSize = arraySize;
    if (newSize == oldSize) return;
    if (newSize == 0) {
        clear();
        return;
    }

    if (newSize < oldSize) {
        for (size_t i = newSize; i < oldSize; ++i) {
            reset(static_cast<int>(i));
        }
        arraySize = newSize;
        return;
    }

    const size_t newCapacity = (newSize + BITS_PER_BLOCK - 1) / BITS_PER_BLOCK;
    if (newCapacity > capacity) {
        reallocate(newCapacity, newSize, value);
        return;
    }

    arraySize = newSize;
    if (value) {
        for (size_t i = oldSize; i < arraySize; ++i) {
            set(static_cast<int>(i), value);
        }
    }
}

void BitArray::swap(BitArray& other) {
    std::swap(array, other.array);
    std::swap(arraySize, other.arraySize);
    std::swap(capacity, other.capacity);
}

BitArray& BitArray::operator=(const BitArray& other) {
    if (this == &other) return *this;
    BitArray temp(other);
    swap(temp);
    return *this;
}

void BitArray::push_back(const bool bit) {
    const size_t newSize = arraySize + 1;
    const size_t newCapacity = (newSize + BITS_PER_BLOCK - 1) / BITS_PER_BLOCK;
    if (newCapacity > capacity) {
        reallocate(newCapacity, newSize, false);
    } else {
        arraySize = newSize;
    }
    set(static_cast<int>(arraySize - 1), bit);
}

BitArray::BitReference::operator bool() const {
    return static_cast<const BitArray&>(bitArray).operator[](bitPos);
}

BitArray::BitReference& BitArray::BitReference::operator=(const bool value) {
    bitArray.set(bitPos, value);
    return *this; //assignment chain support
}

BitArray::BitReference& BitArray::BitReference::operator=(const BitReference& other) {
    const bool value = static_cast<bool>(other); //explicit call of the bool operator
    bitArray.set(bitPos, value);
    return *this;
}

BitArray::BitReference BitArray::operator[](const int bitPos) {
    if (arraySize == 0) {
        throw std::out_of_range("BitArray is empty");
    }
    checkBitPos(bitPos, arraySize);
    return {*this, bitPos};
}


bool BitArray::any() const {
    if (array == nullptr) return false;
    for (size_t i = 0; i < capacity; ++i) {
        if (array[i] != 0) return true;
    }
    return false;
}

bool BitArray::none() const {
    return !any();
}

int BitArray::count() const {
    int count = 0;
    for (size_t i = 0; i < capacity; ++i) {
        unsigned long value = array[i];
        while (value) {
            count++;
            value &= value - 1;
        }
    }
    return count;
}

BitArray BitArray::operator~() const {
    BitArray result(*this);
    for (size_t i = 0; i < capacity; ++i) {
        result.array[i] = ~array[i];
    }
    if (result.capacity != 0) {
        result.array[capacity - 1] &= getBitMask();
    }
    return result;
}

BitArray &BitArray::operator>>=(const int shift) {
    checkArgument(shift);

    if (shift == 0) return *this;

    if (shift >= static_cast<int>(arraySize)) {
        for (size_t i = 0; i < capacity; ++i) {
            array[i] = 0;
        }
        return *this;
    }

    const size_t byteShift = shift / BITS_PER_BLOCK;
    const size_t bitShift = shift % BITS_PER_BLOCK;
    if (byteShift > 0) {
        for (size_t i = 0; i < capacity - byteShift; ++i) {
            array[i] = array[i + byteShift];
        }
        for (size_t i = capacity - byteShift; i < capacity; ++i) {
            array[i] = 0;
        }
    }
    if (bitShift > 0) {
        unsigned long carry = 0;
        for (size_t i = capacity; i > 0; --i) {
            const unsigned long current = array[i - 1];
            array[i - 1] = (current >> bitShift) | carry;
            carry = current << (BITS_PER_BLOCK - bitShift);
        }
    }
    return *this;
}

BitArray &BitArray::operator<<=(const int shift) {
    checkArgument(shift);

    if (shift == 0) return *this;

    if (shift >= static_cast<int>(arraySize)) {
        for (size_t i = 0; i < capacity; ++i) {
            array[i] = 0;
        }
        return *this;
    }

    const size_t byteShift = shift / BITS_PER_BLOCK;
    const size_t bitShift = shift % BITS_PER_BLOCK;
    if (byteShift > 0) {
        for (size_t i = capacity - 1; i >= byteShift; --i) {
            array[i] = array[i - byteShift];
        }
        for (size_t i = 0; i < byteShift; ++i) {
            array[i] = 0;
        }
    }
    if (bitShift > 0) {
        unsigned long carry = 0;
        for (size_t i = 0; i < capacity; ++i) {
            const unsigned long current = array[i];
            array[i] = (current << bitShift) | carry;
            carry = current >> (BITS_PER_BLOCK - bitShift);
        }
    }
    return *this;
}

BitArray BitArray::operator>>(const int shift) const {
    checkArgument(shift);
    BitArray result(*this);
    result >>= shift;
    return result;
}

BitArray BitArray::operator<<(const int shift) const {
    checkArgument(shift);
    BitArray result(*this);
    result <<= shift;
    return result;
}

std::string BitArray::toString() const {
    std::string result;
    result.reserve(arraySize);
    for (size_t i = 0; i < arraySize; ++i) {
        result += (operator[](static_cast<int>(i)) ? '1' : '0');
    }
    std::reverse(result.begin(), result.end());
    return result;
}

BitArray &BitArray::operator^=(const BitArray &b) {
    checkSizeMatch(b);
    for (size_t i = 0; i < capacity; ++i) {
        array[i] ^= b.array[i];
    }
    return *this;
}

BitArray &BitArray::operator&=(const BitArray &b) {
    checkSizeMatch(b);
    for (size_t i = 0; i < capacity; ++i) {
        array[i] &= b.array[i];
    }
    return *this;
}

BitArray &BitArray::operator|=(const BitArray &b) {
    checkSizeMatch(b);
    for (size_t i = 0; i < capacity; ++i) {
        array[i] |= b.array[i];
    }
    return *this;
}

BitArray operator&(const BitArray &b1, const BitArray &b2) {
    BitArray result = b1;
    result &= b2;
    return result;
}

BitArray operator^(const BitArray &b1, const BitArray &b2) {
    BitArray result = b1;
    result ^= b2;
    return result;
}

BitArray operator|(const BitArray &b1, const BitArray &b2) {
    BitArray result = b1;
    result |= b2;
    return result;
}

bool operator==(const BitArray &b1, const BitArray &b2) {
    if (b1.size() != b2.size()) return false;
    for (size_t i = 0; i < b1.size(); ++i) {
        if (b1[static_cast<int>(i)] != b2[static_cast<int>(i)]) {
            return false;
        }
    }
    return true;
}

bool operator!=(const BitArray &b1, const BitArray &b2) {
    return !(b1==b2);
}