/*
 * Copyright (c) 2016, Intel Corporation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#pragma once

#include <safe_cast.hpp>
#include <limits>
#include <string>

namespace structure
{
namespace attributes
{
/** Represents a range of any numerical values
 *
 * A range has a lower and upper bound - which are both included in the range.
 *
 * Ranges can be converted between any numerical type, provided the original range can be fully
 * represented within the destination type (conversion throws a range_error if not).
 *
 * a Range can also be converted from a range of string (i.e. Range<std::string>) provided that both
 * bounds can be parsed into the destination numerical type (conversion throws a range_error if
 * not).
 *
 * @tparam T : The numerical type over which the range is defined.
 */
template <class T>
class Range;

template <>
class Range<std::string>
{
public:
    Range(const std::string &min, const std::string &max) : min(min), max(max) {}

    const std::string min, max;
};

template <class Storage>
class Range // TODO: derive from a "ValueRestriction" or "AllowedValues" class, with a pure virtual
            // "isAllowed" method.
{
    static_assert(std::is_arithmetic<Storage>::value,
                  "Ranges can only be defined over arithmetic (aka. numerical) types.");

    template <class OtherStorage>
    friend class Range;

public:
    Range() = default;
    Range(Storage min, Storage max) : mMin(min), mMax(max)
    {
        if (mMin > mMax) {
            using std::to_string;
            throw std::range_error("The [" + to_string(mMin) + ", " + to_string(mMax) +
                                   "] range does not contain any value (i.e. min > max).");
        }
    }

    Range(const Range<std::string> &from) { *this = from; }
    Range &operator=(const Range<std::string> &from) try {
        mMin = safe_cast<Storage>(from.min);
        mMax = safe_cast<Storage>(from.max);
        return *this;
    } catch (CastError &e) {
        throw std::range_error("When trying to convert range [" + from.min + ", " + from.max +
                               "] from string: " + e.what());
    }

    template <class Other>
    Range(const Range<Other> &from)
    {
        *this = from;
    }
    template <class Other>
    Range &operator=(const Range<Other> &from) try {
        mMin = safe_cast<Storage>(from.mMin);
        mMax = safe_cast<Storage>(from.mMax);
        return *this;
    } catch (CastError &e) {
        using std::to_string;
        throw std::range_error("When trying to convert range [" + to_string(from.mMin) + ", " +
                               to_string(from.mMax) + "]: " + e.what());
    }

    bool operator==(const Range &other) const { return other.mMin == mMin and other.mMax == mMax; }

    /** A Range with only a lower bound.
     *
     * The upper bound is implicitly the numerical type's upper bound.
     */
    static Range Min(Storage min) { return Range(min, maxima); }
    /** A Range with only an upper bound.
     *
     * The lower bound is implicitly the numerical type's lower bound.
     */
    static Range Max(Storage max) { return Range(minima, max); }

    Storage min() const { return mMin; }
    Storage max() const { return mMax; }

private:
    static constexpr Storage minima = std::numeric_limits<Storage>::lowest();
    static constexpr Storage maxima = std::numeric_limits<Storage>::max();
    Storage mMin = minima;
    Storage mMax = maxima;
};

/** Helper for constructing an untyped Range. */
inline Range<std::string> mkRange(const char *min, const char *max)
{
    return Range<std::string>(min, max);
}

/** Helper for constructing a Range over a numerical type. */
template <class T, typename = std::enable_if<std::is_arithmetic<T>::value>>
Range<T> mkRange(const T &min, const T &max)
{
    return Range<T>(min, max);
}
} // namespace attributes
} // namespace structure
