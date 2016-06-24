#pragma once

#include <convert.hpp>

#include <limits>
#include <stdexcept>
#include <type_traits>

namespace safe_cast_details
{
struct InternalError
{
};

template <typename From, typename To>
using SameSignedness =
    typename std::enable_if<std::is_signed<From>::value == std::is_signed<To>::value>::type;

template <typename From, typename To>
using FromSignedToUnsigned =
    typename std::enable_if<std::is_signed<From>::value and std::is_unsigned<To>::value>::type;

template <typename From, typename To>
using FromUnsignedToSigned =
    typename std::enable_if<std::is_unsigned<From>::value and std::is_signed<To>::value>::type;

template <typename From, typename To>
To integer_safe_cast(From from, SameSignedness<From, To> * = 0)
{
    using limits = std::numeric_limits<To>;
    if (limits::min() <= from and from <= limits::max()) {
        return static_cast<To>(from);
    }
    throw InternalError();
}

template <typename From, typename To>
To integer_safe_cast(From from, FromSignedToUnsigned<From, To> * = 0)
{
    auto unsignedFrom = static_cast<typename std::make_unsigned<From>::type>(from);
    if (0 <= from and unsignedFrom <= std::numeric_limits<To>::max()) {
        return static_cast<To>(from);
    }
    throw InternalError();
}

template <typename From, typename To>
To integer_safe_cast(From from, FromUnsignedToSigned<From, To> * = 0)
{
    auto unsignedMax =
        static_cast<typename std::make_unsigned<To>::type>(std::numeric_limits<To>::max());
    if (from <= unsignedMax) {
        return static_cast<To>(from);
    }
    throw InternalError();
}
} // namespace safe_cast_details

/** Thrown when a safe_cast operation fails.
 */
struct CastError : std::range_error
{
    template <class To>
    CastError(const std::string &from, const To &)
        : std::range_error::range_error("Can't fit \"" + from + "\"into a type limited to [" +
                                        std::to_string(std::numeric_limits<To>::min()) + ", " +
                                        std::to_string(std::numeric_limits<To>::max()) + "].")
    {
    }
};

/** Converts any integer to any integer type.
 *
 * @throws CastError if not possible
 */
template <class To, class From>
To safe_cast(From from, typename std::enable_if<std::is_integral<To>::value and
                                                std::is_integral<From>::value>::type * = 0) try {
    return safe_cast_details::integer_safe_cast<From, To>(from);
} catch (safe_cast_details::InternalError &) {
    throw CastError(std::to_string(from), To());
}

/** Converts any floating point to any floating point type.
 *
 * @throws CastError if not possible
 */
template <class To, class From>
To safe_cast(From from, typename std::enable_if<std::is_floating_point<To>::value and
                                                std::is_floating_point<From>::value>::type * = 0)
{
    using limits = std::numeric_limits<To>;
    if (limits::min() <= from and from <= limits::max()) {
        return static_cast<To>(from);
    }
    throw CastError(std::to_string(from), To());
}

/** Converts a string to any numerical type.
 *
 * @throws CastError if not possible
 */
template <class To>
To safe_cast(const std::string &from)
{
    To to;
    if (not convertTo(from, to)) {
        throw CastError(from, to);
    }
    return to;
}
