#include <gtest/gtest.h>
#include "BitArray.h"

TEST(BitArrayTest, ConstructorExceptionHandling) {
    EXPECT_THROW(BitArray(-5, 0), std::invalid_argument);
    BitArray ba(4, 0b1111);
    EXPECT_THROW(ba[-1], std::out_of_range);
    EXPECT_THROW(ba[4], std::out_of_range);
    EXPECT_THROW(ba[100], std::out_of_range);
    BitArray empty(0, 0);
    EXPECT_THROW(empty[0], std::out_of_range);
}

TEST(BitArrayTest, ParameterizedConstructor) {
    {
        BitArray ba(4, 0b10101010);
        EXPECT_EQ(ba.toString(), "1010");
        EXPECT_THROW(ba[4], std::out_of_range);
    }

    {
        BitArray ba(0, 100);
        EXPECT_EQ(ba.size(), 0);
    }
}

TEST(BitArrayTest, ConstructorMultiBlockInitialization) {
    const size_t BLOCK_BITS = sizeof(unsigned long) * 8;
    const size_t TOTAL_BITS = BLOCK_BITS * 2;
    const BitArray ba(TOTAL_BITS, 0xFFFFFFFF);
    for (size_t i = 0; i < BLOCK_BITS; ++i) {
        EXPECT_TRUE(ba[i]);
    }
    for (size_t i = BLOCK_BITS; i < TOTAL_BITS; ++i) {
        EXPECT_FALSE(ba[i]);
    }
    EXPECT_THROW(ba[TOTAL_BITS], std::out_of_range);
}

TEST(BitArrayTest, CopyConstructor) {
    const BitArray original(8, 0b11001100);
    const BitArray copy(original);
    EXPECT_TRUE(original == copy);
}

TEST(BitArrayTest, Resize) {
    {
        const size_t INITIAL_BITS = 8;
        const size_t NEW_BITS = 10;
        BitArray ba(INITIAL_BITS, 204);
        ba.resize(NEW_BITS, true);
        EXPECT_EQ(ba.size(), NEW_BITS);
        EXPECT_EQ(ba.toString(), "1111001100");
        EXPECT_THROW(ba[10], std::out_of_range);
    }

    {
        const size_t INITIAL_BITS = 5;
        const size_t NEW_BITS = 3;
        BitArray a(INITIAL_BITS, 0b11100);
        a.resize(NEW_BITS, false);
        EXPECT_EQ(a.size(), NEW_BITS);
        EXPECT_EQ(a.toString(), "100");
        EXPECT_THROW(a[3], std::out_of_range);
        a.resize(5, false);
        EXPECT_FALSE(a[3]);
        EXPECT_FALSE(a[4]);
    }
}

TEST(BitArrayTest, ResizeWithReallocation) {
    const size_t BLOCK_BITS = sizeof(unsigned long) * 8;
    const size_t NEW_BITS = BLOCK_BITS * 2;
    BitArray b(BLOCK_BITS, 0xFFFFFFFF);
    b.resize(NEW_BITS, false);
    EXPECT_EQ(b.size(), NEW_BITS);
    for (size_t i = 0; i < BLOCK_BITS; ++i) {
        EXPECT_TRUE(b[i]);
    }
    for (size_t i = BLOCK_BITS; i < NEW_BITS; ++i) {
        EXPECT_FALSE(b[i]);
    }
    EXPECT_THROW(b[NEW_BITS], std::out_of_range);
}

TEST(BitArrayTest, ResizeEmptyArray) {
    BitArray empty;
    const size_t NEW_BITS = 10;
    empty.resize(NEW_BITS, false);
    EXPECT_EQ(empty.size(), NEW_BITS);
    EXPECT_EQ(empty.toString(), "0000000000");
    EXPECT_THROW(empty[NEW_BITS], std::out_of_range);
}

TEST(BitArrayTest, ResizeToSameSize) {
    const size_t BITS = 8;
    BitArray ba(BITS, 0b10101010);
    ba.resize(BITS, true);
    EXPECT_EQ(ba.size(), BITS);
    EXPECT_EQ(ba.toString(), "10101010");
}

TEST(BitArrayTest, SetResetSingleBit) {
    {
        BitArray ba(8, 0);
        ba.set(0);
        EXPECT_TRUE(ba[0]);
        EXPECT_FALSE(ba[1]);
        ba.set(3, true);
        EXPECT_TRUE(ba[3]);
        ba.set(0, false);
        EXPECT_FALSE(ba[0]);
    }

    {
        BitArray ba(8, 0xFF);
        ba.reset(0);
        EXPECT_FALSE(ba[0]);
        EXPECT_TRUE(ba[1]);
        ba.reset(7);
        EXPECT_FALSE(ba[7]);
    }
}

TEST(BitArrayTest, SetResetChain) {
    BitArray ba(8, 0);
    ba.set(0).set(1).reset(1).set(3);
    EXPECT_TRUE(ba[0]);
    EXPECT_FALSE(ba[1]);
    EXPECT_TRUE(ba[3]);
}

TEST(BitArrayTest, SetResetOutOfRange) {
    BitArray ba(8, 0);
    EXPECT_THROW(ba.set(-1), std::out_of_range);
    EXPECT_THROW(ba.set(8), std::out_of_range);
    EXPECT_THROW(ba.reset(-1), std::out_of_range);
    EXPECT_THROW(ba.reset(8), std::out_of_range);
}

TEST(BitArrayTest, SetResetEmptyArray) {
    BitArray ba(0, 0);
    ba.set();
    ba.reset();
    EXPECT_THROW(ba.set(0), std::out_of_range);
    EXPECT_THROW(ba.reset(0), std::out_of_range);
}

TEST(BitArrayTest, SetResetMultipleBlocks) {
    const size_t BLOCK_BITS = sizeof(unsigned long) * 8;
    BitArray ba(BLOCK_BITS * 2);
    ba.set();
    for (size_t i = 0; i < BLOCK_BITS * 2; ++i) {
        EXPECT_TRUE(ba[i]);
    }
    ba.reset();
    for (size_t i = 0; i < BLOCK_BITS * 2; ++i) {
        EXPECT_FALSE(ba[i]);
    }
}

TEST(BitArrayTest, SwapDifferentSizes) {
    BitArray a(8, 0xAA);   // 10101010
    BitArray b(16, 0xFFFF);
    a.swap(b);
    EXPECT_EQ(a.size(), 16);
    EXPECT_EQ(b.size(), 8);
    for (int i = 0; i < 16; ++i) {
        EXPECT_TRUE(a[i]);
    }
    for (int i = 0; i < 8; ++i) {
        EXPECT_EQ(b[i], i % 2 == 1);
    }
    EXPECT_THROW(a[16], std::out_of_range);
    EXPECT_THROW(b[8], std::out_of_range);
}

TEST(BitArrayTest, AssignmentOperator) {
    const BitArray other(8, 0xAA);
    const BitArray ba = other;
    EXPECT_EQ(ba.toString(), "10101010");
}

TEST(BitArrayTest, PushBackToEmpty) {
    BitArray ba;
    ba.push_back(true);
    EXPECT_EQ(ba.size(), 1);
    EXPECT_TRUE(ba[0]);
    ba.push_back(false);
    EXPECT_EQ(ba.size(), 2);
    EXPECT_FALSE(ba[1]);
}

TEST(BitArrayTest, PushBackWithReallocation) {
    const size_t BLOCK_BITS = sizeof(unsigned long) * 8;
    BitArray ba(BLOCK_BITS,0);
    ba.push_back(true);
    EXPECT_EQ(ba.size(), BLOCK_BITS + 1);
    for (int i = 0; i < BLOCK_BITS; ++i) {
        EXPECT_FALSE(ba[i]);
    }
    EXPECT_TRUE(ba[BLOCK_BITS]);
    EXPECT_THROW(ba[BLOCK_BITS + 1], std::out_of_range);
}

TEST(BitArrayTest, AnyNone) {
    {
        BitArray empty;
        EXPECT_FALSE(empty.any());
        EXPECT_TRUE(empty.none());
    }

    {
        BitArray allZeros(8, 0);
        EXPECT_FALSE(allZeros.any());
        EXPECT_TRUE(allZeros.none());
    }

    {
        BitArray allOnes(8, 0xFF);
        EXPECT_TRUE(allOnes.any());
        EXPECT_FALSE(allOnes.none());
    }

    {
        BitArray mixed(8, 0x1); // Только первый бит = 1
        EXPECT_TRUE(mixed.any());
        EXPECT_FALSE(mixed.none());
    }
}

TEST(BitArrayTest, RightShiftOperator) {
    {
        BitArray ba(8, 0b10101010);
        ba >>= 0;
        EXPECT_EQ(ba.size(), 8);
        EXPECT_EQ(ba.toString(), "10101010");
    }

    {
        BitArray ba(8, 0b10101010);
        ba >>= 1;
        EXPECT_EQ(ba.toString(), "01010101");
        EXPECT_EQ(ba.size(), 8);
    }

    {
        BitArray ba(8, 0b11001100);
        ba >>= 2;
        EXPECT_EQ(ba.toString(), "00110011");
    }

    {
        BitArray ba(8);
        EXPECT_THROW(ba >>= -1, std::invalid_argument);
    }

    {
        BitArray ba(0);
        EXPECT_NO_THROW(ba >>= 5);
        EXPECT_EQ(ba.size(), 0);
    }

    {
        BitArray ba(16, 0xAAAA);
        ba.push_back(true);
        ba >>= 5;
        EXPECT_EQ(ba.toString(), "00000110101010101");
    }
}

TEST(BitArrayTest, LeftShiftOperator) {
    {
        BitArray ba(8, 0b10101010);
        ba <<= 0;
        EXPECT_EQ(ba.size(), 8);
        EXPECT_EQ(ba.toString(), "10101010");
    }

    {
        BitArray ba(8, 0b10101010);
        ba <<= 1;
        EXPECT_EQ(ba.size(), 8);
        EXPECT_EQ(ba.toString(), "01010100");
    }

    {
        BitArray ba(8, 0b11001100);
        ba <<= 2;
        EXPECT_EQ(ba.toString(), "00110000");
    }

    {
        BitArray ba(8);
        EXPECT_THROW(ba <<= -1, std::invalid_argument);
    }

    {
        BitArray ba(0);
        EXPECT_NO_THROW(ba <<= 5);
        EXPECT_EQ(ba.size(), 0);
    }

    {
        BitArray ba(8, 0b11111111);
        ba <<= 10;
        EXPECT_EQ(ba.toString(), "00000000");
    }

    {
        BitArray ba(16, 0xAAAA);
        ba.push_back(true);
        ba <<= 5;
        EXPECT_EQ(ba.toString(), "10101010101000000");
    }
}

TEST(BitArray, LeftShiftAcrossMultipleBlocks) {
    const int BITS_PER_BLOCK = sizeof(unsigned long) * 8;
    const int TOTAL_BITS = BITS_PER_BLOCK * 2 + 10;
    BitArray b(TOTAL_BITS);
    for (int i = 0; i < TOTAL_BITS; i++) {
        b.set(i,i % 2 == 0);
    }
    b <<= BITS_PER_BLOCK + 5;
    EXPECT_EQ(b.size(), TOTAL_BITS);
    for (int i = 0; i < BITS_PER_BLOCK + 5; i++) {
        EXPECT_FALSE(b[i]);
    }
    for (int i = BITS_PER_BLOCK + 5; i < TOTAL_BITS; i++) {
        bool expected = ((i - (BITS_PER_BLOCK + 5)) % 2 == 0);
        EXPECT_EQ(b[i], expected);
    }
}

TEST(BitArray, RightShiftAcrossMultipleBlocks) {
    const int BITS_PER_BLOCK = sizeof(unsigned long) * 8;
    const int TOTAL_BITS = BITS_PER_BLOCK * 3 + 15;
    BitArray b(TOTAL_BITS);
    for (int i = 0; i < TOTAL_BITS; i++) {
        b.set(i, i % 3 == 0);
    }
    const int SHIFT = BITS_PER_BLOCK * 2 + 7;
    b >>= SHIFT;
    EXPECT_EQ(b.size(), TOTAL_BITS);
    for (int i = TOTAL_BITS - SHIFT; i < TOTAL_BITS; i++) {
        EXPECT_FALSE(b[i]);
    }
    for (int i = 0; i < TOTAL_BITS - SHIFT; i++) {
        bool expected = ((i + SHIFT) % 3 == 0);
        EXPECT_EQ(b[i], expected);
    }
}

TEST(BitArrayTest, BitwiseANDOperator) {
    {
        BitArray ba1(8, 0b11001100);
        BitArray ba2(8, 0b10101010);
        ba1 &= ba2;
        EXPECT_EQ(ba1.toString(), "10001000");
    }

    {
        BitArray ba1(8, 0b11111111);
        BitArray ba2(8, 0b00000000);
        ba1 &= ba2;
        EXPECT_EQ(ba1.toString(), "00000000");
    }

    {
        BitArray ba1(8, 0b10101010);
        BitArray ba2(8, 0b10101010);
        ba1 &= ba2;
        EXPECT_EQ(ba1.toString(), "10101010");
    }

    {
        BitArray ba1(8);
        BitArray ba2(16);
        EXPECT_THROW(ba1 &= ba2, std::invalid_argument);
    }

    {
        const int TOTAL_BITS = sizeof(unsigned long) * 8 + 2;
        BitArray ba1;
        ba1.resize(TOTAL_BITS,true);
        BitArray ba2(TOTAL_BITS);
        for (size_t i = 0; i < TOTAL_BITS; i += 2) {
            ba2.set(i,true);
        }
        ba1 &= ba2;
        std::string expected;
        for (int i = TOTAL_BITS - 1; i >= 0; --i) {
            expected += (i % 2 == 0) ? '1' : '0';
        }
        EXPECT_EQ(ba1.toString(), expected);
    }
}

TEST(BitArrayTest, BitwiseOROperator) {
    {
        BitArray ba1(8, 0b11001100);
        const BitArray ba2(8, 0b10101010);
        ba1 |= ba2;
        EXPECT_EQ(ba1.toString(), "11101110");
    }

    {
        BitArray ba1(8, 0b00000000);
        const BitArray ba2(8, 0b11111111);
        ba1 |= ba2;
        EXPECT_EQ(ba1.toString(), "11111111");
    }

    {
        BitArray ba1(8, 0b10101010);
        const BitArray ba2(8, 0b00000000);
        ba1 |= ba2;
        EXPECT_EQ(ba1.toString(), "10101010");
    }

    {
        BitArray ba1;
        ba1.resize(63);
        BitArray ba2;
        ba2.resize(63, true);
        ba1 |= ba2;
        EXPECT_EQ(ba1.toString(), std::string(63, '1'));
    }
}

TEST(BitArrayTest, BitwiseXOROperator) {
    {
        BitArray ba1(8, 0b11001100);
        const BitArray ba2(8, 0b10101010);
        ba1 ^= ba2;
        EXPECT_EQ(ba1.toString(), "01100110");
    }

    {
        BitArray ba1(8, 0b10101010);
        const BitArray ba2(8, 0b10101010);
        ba1 ^= ba2;
        EXPECT_EQ(ba1.toString(), "00000000");
    }

    {
        BitArray ba1(8, 0b10101010);
        const BitArray ba2(8, 0b01010101);
        ba1 ^= ba2;
        EXPECT_EQ(ba1.toString(), "11111111");
    }

    {
        BitArray ba1(12, 0b111111111111);
        const BitArray ba2(12, 0b000000000000);
        ba1 ^= ba2;
        EXPECT_EQ(ba1.toString(), "111111111111");
    }
}

TEST(BitArrayTest, BitwiseANDOperatorNew) {
    {
        const BitArray b1(8, 0b11001100);
        const BitArray b2(8, 0b10101010);
        const BitArray result = b1 & b2;
        EXPECT_EQ(result.toString(), "10001000");
    }

    {
        BitArray b1(8, 0b11111111);
        BitArray b2(8, 0b00000000);
        const BitArray result = b1 & b2;
        EXPECT_EQ(result.toString(), "00000000");
    }

    {
        const BitArray b1(8);
        const BitArray b2(16);
        EXPECT_THROW(b1 & b2, std::invalid_argument);
    }
}

TEST(BitArrayTest, BitwiseOROperatorNew) {
    {
        const BitArray b1(8, 0b11001100);
        const BitArray b2(8, 0b10101010);
        const BitArray result = b1 | b2;
        EXPECT_EQ(result.toString(), "11101110");
    }

    {
        const BitArray b1(8, 0b00000000);
        const BitArray b2(8, 0b11111111);
        const BitArray result = b1 | b2;
        EXPECT_EQ(result.toString(), "11111111");
    }
}

TEST(BitArrayTest, BitwiseXOROperatorNew) {
    {
        const BitArray b1(8, 0b11001100);
        const BitArray b2(8, 0b10101010);
        const BitArray result = b1 ^ b2;
        EXPECT_EQ(result.toString(), "01100110");
    }

    {
        const BitArray b1(8, 0b10101010);
        const BitArray result = b1 ^ b1;
        EXPECT_EQ(result.toString(), "00000000");
    }
}

TEST(BitArrayTest, EqualityOperator) {
    {
        const BitArray ba1(8, 0b10101010);
        const BitArray ba2(8, 0b10101010);
        EXPECT_TRUE(ba1 == ba2);
    }

    {
        const BitArray ba1(8, 0b10101010);
        const BitArray ba2(8, 0b01010101);
        EXPECT_FALSE(ba1 == ba2);
    }

    {
        const BitArray ba1(8);
        const BitArray ba2(16);
        EXPECT_FALSE(ba1 == ba2);
    }
}

TEST(BitArrayTest, BitwiseNOTOperator) {
    {
        const BitArray ba(8, 0b10101010);
        const BitArray inverted = ~ba;
        EXPECT_EQ(inverted.toString(), "01010101");
    }

    {
        const BitArray ba(8, 0b11111111);
        const BitArray inverted = ~ba;
        EXPECT_EQ(inverted.toString(), "00000000");
    }

    {
        BitArray ba(8);
        ba.set(0, true);
        ba.set(3, true);
        ba.set(7, true);
        const BitArray inverted = ~ba;
        EXPECT_EQ(inverted.toString(), "01110110");
    }

    {
        const BitArray ba(8, 0b11001100);
        const BitArray doubleInverted = ~~ba;
        EXPECT_EQ(doubleInverted.toString(), "11001100");
        EXPECT_TRUE(ba == doubleInverted);
    }

    {
        const BitArray ba(0);
        const BitArray inverted = ~ba;
        EXPECT_EQ(inverted.size(), 0);
    }
}

TEST(BitArrayTest, CountMethod) {
    {
        const BitArray ba(0);
        EXPECT_EQ(ba.count(), 0);
    }

    {
        const BitArray ba(8, 0b00000000);
        EXPECT_EQ(ba.count(), 0);
    }

    {
        const BitArray ba(8, 0b11111111);
        EXPECT_EQ(ba.count(), 8);
    }

    {
        const BitArray ba(8, 0b10101010);
        EXPECT_EQ(ba.count(), 4);
    }

    {
        const BitArray ba(10, 0b1111111111);
        EXPECT_EQ(ba.count(), 10);
    }

    {
        BitArray ba(1000);
        for (size_t i = 0; i < 1000; i += 3) {
            ba.set(i, true);
        }
        EXPECT_EQ(ba.count(), 334);
    }
}

TEST(BitArrayTest, ProxyReadWrite) {
    BitArray ba(8);
    ba[0] = true;
    ba[3] = true;
    ba[7] = true;
    EXPECT_EQ(ba.toString(), "10001001");
}

TEST(BitArrayTest, ProxyChainAssignment) {
    BitArray ba(8);
    ba[0] = true;
    ba[1] = ba[0];
    ba[2] = false;
    ba[3] = ba[2];
    EXPECT_EQ(ba.toString(), "00000011");
}

TEST(BitArrayTest, ProxyOutOfRange) {
    {
        BitArray ba(5);
        EXPECT_THROW(ba[-1] = true, std::out_of_range);
        EXPECT_THROW(ba[5] = false, std::out_of_range);
        EXPECT_THROW(bool b = ba[-1], std::out_of_range);
        EXPECT_THROW(bool b = ba[5], std::out_of_range);
    }

    {
        BitArray ba(0);
        EXPECT_THROW(ba[0] = true, std::out_of_range);
        EXPECT_THROW(bool b = ba[0], std::out_of_range);
    }
}