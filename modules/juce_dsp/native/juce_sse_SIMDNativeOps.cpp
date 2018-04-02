/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2017 - ROLI Ltd.

   JUCE is an open source library subject to commercial or open-source
   licensing.

   By using JUCE, you agree to the terms of both the JUCE 5 End-User License
   Agreement and JUCE 5 Privacy Policy (both updated and effective as of the
   27th April 2017).

   End User License Agreement: www.juce.com/juce-5-license
   Privacy Policy: www.juce.com/juce-5-privacy-policy

   Or: You may also use this code under the terms of the GPL v3 (see
   www.gnu.org/licenses).

   JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
   EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
   DISCLAIMED.

  ==============================================================================
*/

namespace juce
{
    namespace dsp
    {
        DEFINE_SSE_SIMD_CONST (int32_t, float, kAllBitsSet)     = { -1, -1, -1, -1 };
        DEFINE_SSE_SIMD_CONST (int32_t, float, kEvenHighBit)    = { static_cast<int32_t>(0x80000000), 0, static_cast<int32_t>(0x80000000), 0 };
        DEFINE_SSE_SIMD_CONST (float, float, kOne)              = { 1.0f, 1.0f, 1.0f, 1.0f };

        DEFINE_SSE_SIMD_CONST (int64_t, double, kAllBitsSet)    = { -1LL, -1LL };
        DEFINE_SSE_SIMD_CONST (int64_t, double, kEvenHighBit)   = { static_cast<int64_t>(0x8000000000000000), 0 };
        DEFINE_SSE_SIMD_CONST (double, double, kOne)            = { 1.0, 1.0 };

        DEFINE_SSE_SIMD_CONST (int8_t, int8_t, kAllBitsSet)     = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };

        DEFINE_SSE_SIMD_CONST (uint8_t, uint8_t, kAllBitsSet)   = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
        DEFINE_SSE_SIMD_CONST (uint8_t, uint8_t, kHighBit)      = { 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80 };

        DEFINE_SSE_SIMD_CONST (int16_t, int16_t, kAllBitsSet)   = { -1, -1, -1, -1, -1, -1, -1, -1 };

        DEFINE_SSE_SIMD_CONST (uint16_t, uint16_t, kAllBitsSet) = { 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff };
        DEFINE_SSE_SIMD_CONST (uint16_t, uint16_t, kHighBit)    = { 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000 };

        DEFINE_SSE_SIMD_CONST (int32_t, int32_t, kAllBitsSet)   = { -1, -1, -1, -1 };

        DEFINE_SSE_SIMD_CONST (uint32_t, uint32_t, kAllBitsSet) = { 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff };
        DEFINE_SSE_SIMD_CONST (uint32_t, uint32_t, kHighBit)    = { 0x80000000, 0x80000000, 0x80000000, 0x80000000 };

        DEFINE_SSE_SIMD_CONST (int64_t, int64_t, kAllBitsSet)   = { -1, -1 };

        DEFINE_SSE_SIMD_CONST (uint64_t, uint64_t, kAllBitsSet) = { 0xffffffffffffffff, 0xffffffffffffffff };
        DEFINE_SSE_SIMD_CONST (uint64_t, uint64_t, kHighBit)    = { 0x8000000000000000, 0x8000000000000000 };
    }
}
