#include "stdafx.h"

#include "NativeJIT/BitOperations.h"
#include "Temporary/Allocator.h"
#include "TestSetup.h"


namespace NativeJIT
{
    namespace BitOperationsUnitTest
    {
        TEST_CASE(BitOperations, NonZeroBitCount)
        {
            TestEqual(0, BitOp::GetNonZeroBitCount(0u));
            TestEqual(0, BitOp::GetNonZeroBitCountFallback(0u));

            TestEqual(0, BitOp::GetNonZeroBitCount(static_cast<uint64_t>(0)));
            TestEqual(0, BitOp::GetNonZeroBitCountFallback(static_cast<uint64_t>(0)));

            TestEqual(1, BitOp::GetNonZeroBitCount(1u));
            TestEqual(1, BitOp::GetNonZeroBitCountFallback(1u));

            TestEqual(1, BitOp::GetNonZeroBitCount(0x80000000u));
            TestEqual(1, BitOp::GetNonZeroBitCountFallback(0x80000000u));

            TestEqual(1, BitOp::GetNonZeroBitCount(static_cast<uint64_t>(0x8000000000000000)));
            TestEqual(1, BitOp::GetNonZeroBitCountFallback(0x8000000000000000));

            TestEqual(32, BitOp::GetNonZeroBitCount(0xFFFFFFFFu));
            TestEqual(32, BitOp::GetNonZeroBitCountFallback(0xFFFFFFFFu));

            TestEqual(64, BitOp::GetNonZeroBitCount(static_cast<uint64_t>(0xFFFFFFFFFFFFFFFF)));
            TestEqual(64, BitOp::GetNonZeroBitCountFallback(0xFFFFFFFFFFFFFFFF));
        }


        void VerifyLowestBitSet(uint64_t testValue, unsigned expected)
        {
            TestNotEqual(0, testValue);

            unsigned actual;
            const bool foundBit = BitOp::GetLowestBitSet(testValue, &actual);

            TestAssert(foundBit, "Expected to find a bit in %I64u", testValue);
            TestEqual(expected, actual, "Mismatched expected and actual bit for %I64u", testValue);
        }


        TEST_CASE(BitOperations, LowestBitSet)
        {
            unsigned unused;
            TestAssert(!BitOp::GetLowestBitSet(0, &unused));

            VerifyLowestBitSet(1, 0);
            VerifyLowestBitSet(0xFFFFFFFFFFFFFFFF, 0);
            VerifyLowestBitSet(0x8000000000000000, 63);
        }


        void VerifyHighestBitSet(uint64_t testValue, unsigned expected)
        {
            TestNotEqual(0, testValue);

            unsigned actual;
            const bool foundBit = BitOp::GetHighestBitSet(testValue, &actual);

            TestAssert(foundBit, "Expected to find a bit in %I64u", testValue);
            TestEqual(expected, actual, "Mismatched expected and actual bit for %I64u", testValue);
        }


        TEST_CASE(BitOperations, HighestBitSet)
        {
            unsigned unused;
            TestAssert(!BitOp::GetHighestBitSet(0, &unused));

            VerifyHighestBitSet(1, 0);
            VerifyHighestBitSet(0x8000000000000000, 63);
            VerifyHighestBitSet(0xFFFFFFFFFFFFFFFF, 63);
        }


        TEST_CASE(BitOperations, TestBit)
        {
            const uint64_t bits62and63 = 0xC000000000000000;

            TestAssert(!BitOp::TestBit(bits62and63, 0));
            TestAssert(!BitOp::TestBit(bits62and63, 1));
            TestAssert(BitOp::TestBit(bits62and63, 62));
            TestAssert(BitOp::TestBit(bits62and63, 63));

            const uint64_t allZeros = 0;
            const uint64_t allOnes = 0xFFFFFFFFFFFFFFFF;

            for (unsigned int bit = 0; bit < 64; ++bit)
            {
                TestAssert(!BitOp::TestBit(allZeros, bit));
                TestAssert(BitOp::TestBit(allOnes, bit));
            }
        }


        TEST_CASE(BitOperations, SetBit)
        {
            uint64_t currentValue = 0;
            uint64_t allOnes = 0xFFFFFFFFFFFFFFFF;

            for (unsigned int bit = 0; bit < 64; ++bit)
            {
                TestAssert(!BitOp::TestBit(currentValue, bit));
                BitOp::SetBit(&currentValue, bit);
                TestAssert(BitOp::TestBit(currentValue, bit));

                TestAssert(BitOp::TestBit(allOnes, bit));
                BitOp::SetBit(&allOnes, bit);
                TestAssert(BitOp::TestBit(allOnes, bit));
            }
        }


        TEST_CASE(BitOperations, ClearBit)
        {
            uint64_t currentValue = 0xFFFFFFFFFFFFFFFF;
            uint64_t allZeros = 0;

            for (unsigned int bit = 0; bit < 64; ++bit)
            {
                TestAssert(BitOp::TestBit(currentValue, bit));
                BitOp::ClearBit(&currentValue, bit);
                TestAssert(!BitOp::TestBit(currentValue, bit));

                TestAssert(!BitOp::TestBit(allZeros, bit));
                BitOp::ClearBit(&allZeros, bit);
                TestAssert(!BitOp::TestBit(allZeros, bit));
            }
        }
    }
}