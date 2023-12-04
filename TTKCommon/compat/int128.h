#pragma once
/*
int128.h
An signed/unsigned 128 bit integer type for C++

https://github.com/zhanhb/int128

MIT License

Copyright (c) 2018 zhanhb

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <cmath>
#include <iostream>
#include <cinttypes>
#include <type_traits>

#if defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN || \
    defined(__BIG_ENDIAN__) ||                               \
    defined(__ARMEB__) ||                                    \
    defined(__THUMBEB__) ||                                  \
    defined(__AARCH64EB__) ||                                \
    defined(_MIBSEB) || defined(__MIBSEB) || defined(__MIBSEB__)
#  ifndef __BIG_ENDIAN__
#    define __BIG_ENDIAN__
#  endif
#elif defined(__BYTE_ORDER) && __BYTE_ORDER == __LITTLE_ENDIAN || \
    defined(__LITTLE_ENDIAN__) ||                                 \
    defined(__ARMEL__) ||                                         \
    defined(__THUMBEL__) ||                                       \
    defined(__AARCH64EL__) ||                                     \
    defined(_MIPSEL) || defined(__MIPSEL) || defined(__MIPSEL__) || \
    defined(_WIN32) || defined(__i386__) || defined(__x86_64__) || \
    defined(_X86_) || defined(_IA64_)
#  ifndef __LITTLE_ENDIAN__
#    define __LITTLE_ENDIAN__
#  endif
#else
#  error "I don't know what architecture this is!"
#endif

#if (defined(__SIZEOF_INT128__) && __SIZEOF_INT128__ == 16) || defined(_GLIBCXX_USE_INT128)
#  define USE_BUILTIN_INT128
#endif

#ifdef _MSC_VER
#include <intrin.h>
#pragma warning(disable:4804)

static __inline int __builtin_clz(int x)
{
    unsigned long r = 0;
    _BitScanReverse(&r, x);
    return 31 ^ static_cast<int>(r);
}

static __inline int __builtin_clzll(const uint64_t x)
{
    unsigned long r = 0;
#ifdef _WIN64
    _BitScanReverse64(&r, x);
#  else
    // Scan the high 32 bits.
    if (_BitScanReverse(&r, static_cast<uint32_t>(x >> 32)))
        return 63 ^ static_cast<int>(r + 32);
    // Scan the low 32 bits.
    _BitScanReverse(&r, static_cast<uint32_t>(x));
#endif
    return 63 ^ static_cast<int>(r);
}

static __inline int __builtin_clzl(uint32_t x)
{
#ifdef _WIN64
    return __builtin_clzll(x);
#else
    return __builtin_clz(x);
#endif
}
#endif


namespace large_int {
    template<class, class>
    class int128_base;

    typedef int128_base<int64_t, uint64_t> int128_t;
    typedef int128_base<uint64_t, uint64_t> uint128_t;

    template<class _Tp>
    struct half_mask : std::integral_constant<_Tp, (_Tp(1) << (4 * sizeof(_Tp))) - _Tp(1)> {
    };

    template<bool= true>
    struct detail_delegate;

    constexpr bool operator<(int128_t, int128_t);

    constexpr bool operator<(uint128_t, uint128_t);

    constexpr uint128_t operator>>(uint128_t, int);

    constexpr int128_t operator>>(int128_t, int);

    constexpr int128_t operator*(int128_t, int128_t);

    constexpr uint128_t operator*(uint128_t, uint128_t);

    constexpr uint128_t operator<<(uint128_t, int);

    constexpr int128_t operator<<(int128_t, int);

    inline uint128_t operator/(uint128_t, uint128_t);

    inline int128_t operator/(int128_t, int128_t);

    inline uint128_t operator%(uint128_t, uint128_t);

    inline int128_t operator%(int128_t, int128_t);

    template<class _Hi, class _Low>
    class alignas(sizeof(_Hi) * 2) int128_base final {
        static_assert(sizeof(_Hi) == sizeof(_Low), "low type, high type should have same size");

    public:
#ifdef __LITTLE_ENDIAN__
        _Low low_{};
        _Hi high_{};

        constexpr int128_base(_Hi high, _Low low) : low_(low), high_(high) {}

#elif __BIG_ENDIAN__
        _Hi high_{};
        _Low low_{};

        constexpr int128_base(_Hi high, _Low low) : high_(high), low_(low) {}

#else
#error endian not support
#endif

    private:
        struct integral_tag {
        };
        struct signed_integral_tag : integral_tag {
        };
        struct unsigned_integral_tag : integral_tag {
        };
        struct float_tag {
        };
        template<size_t>
        struct size_constant {
        };

        template<class _Tp>
        constexpr int128_base(_Tp value_, signed_integral_tag, size_constant<8>) :
                int128_base(-(value_ < 0), value_) {}

        template<class _Tp>
        constexpr int128_base(_Tp value_, unsigned_integral_tag, size_constant<8>) : int128_base(0, _Low(value_)) {}

        template<class _Tp>
        constexpr int128_base(_Tp value_, integral_tag, size_constant<16>) : // NOLINT explicit
                int128_base(_Hi(value_ >> 64U), _Low(value_)) {} // NOLINT signed shift

    public:

        constexpr int128_base() noexcept = default;

        constexpr int128_base(const int128_base &) noexcept = default;

        constexpr int128_base(int128_base &&) noexcept = default;

        int128_base &operator=(const int128_base &) noexcept = default;

        int128_base &operator=(int128_base &&) noexcept = default;

        template<class _Tp>
        constexpr explicit int128_base(int128_base<_Tp, _Low> val_) : int128_base(val_.high_, val_.low_) {}

        template<class _Tp>
        constexpr int128_base(_Tp val_, float_tag) :
                int128_base(_Hi(std::ldexp(val_, -64)) - (val_ < 0), _Low(val_)) {}

        constexpr explicit int128_base(float val_) : int128_base(val_, float_tag()) {}

        constexpr explicit int128_base(double val_) : int128_base(val_, float_tag()) {}

        constexpr explicit int128_base(long double val_) : int128_base(val_, float_tag()) {}

        constexpr int128_base(long long val_) : // NOLINT explicit
                int128_base(val_, signed_integral_tag(), size_constant<sizeof(val_)>()) {}

        constexpr int128_base(long val_) : int128_base(static_cast<long long>(val_)) {} // NOLINT explicit

        constexpr int128_base(int val_) : int128_base(long(val_)) {} // NOLINT explicit

        constexpr int128_base(unsigned long long val_) : // NOLINT explicit
                int128_base(val_, unsigned_integral_tag(), size_constant<sizeof(val_)>()) {}

        constexpr int128_base(unsigned long val_) : // NOLINT explicit
                int128_base(static_cast<unsigned long long>(val_)) {}

        constexpr int128_base(unsigned val_) : int128_base(static_cast<unsigned long>(val_)) {} // NOLINT explicit

        constexpr explicit operator bool() const { return high_ || low_; }

        constexpr explicit operator char() const { return char(low_); }

        constexpr explicit operator signed char() const { return static_cast<signed char>(low_); }

        constexpr explicit operator unsigned char() const { return static_cast<unsigned char>(low_); }

        constexpr explicit operator short() const { return short(low_); }

        constexpr explicit operator unsigned short() const { return static_cast<unsigned short>(low_); }

        constexpr explicit operator int() const { return int(low_); }

        constexpr explicit operator unsigned() const { return unsigned(low_); }

        constexpr explicit operator long() const { return long(low_); }

        constexpr explicit operator unsigned long() const { return static_cast<unsigned long>(low_); }

        constexpr explicit operator long long() const { return static_cast<long long>(low_); }

        constexpr explicit operator unsigned long long() const { return static_cast<unsigned long long>(low_); }

        constexpr explicit operator wchar_t() const { return wchar_t(low_); }

        constexpr explicit operator char16_t() const { return char16_t(low_); }

        constexpr explicit operator char32_t() const { return char32_t(low_); }

#if USE_BUILTIN_INT128

        constexpr explicit int128_base(__int128 val_) :
                int128_base(val_, signed_integral_tag(), size_constant<sizeof(val_)>()) {}

        constexpr explicit int128_base(unsigned __int128 val_) :
                int128_base(val_, unsigned_integral_tag(), size_constant<sizeof(val_)>()) {}

        constexpr explicit operator unsigned __int128() const {
            return static_cast<unsigned __int128>(high_) << 64U | static_cast<unsigned __int128>(low_);
        }

        constexpr explicit operator __int128() const {
            return static_cast<__int128>(static_cast<unsigned __int128>(*this));
        }

#endif

    private:
        template<class _Tp>
        constexpr _Tp cast_to_float() const;

    public:
        constexpr explicit operator float() const { return cast_to_float<float>(); }

        constexpr explicit operator double() const { return cast_to_float<double>(); }

        constexpr explicit operator long double() const { return cast_to_float<long double>(); }

        constexpr int128_base operator+() const { return *this; }

        constexpr int128_base operator-() const { return int128_base(-high_ - (low_ != 0), -low_); }

        constexpr int128_base operator~() const { return int128_base(~high_, ~low_); }

        constexpr bool operator!() const { return !high_ && !low_; }

        // avoid self plus on rvalue
        int128_base &operator++() &{ return *this = *this + int128_base(1); }

        int128_base &operator--() &{ return *this = *this - int128_base(1); }

        int128_base operator++(int) &{ // NOLINT returns non constant
            int128_base tmp = *this;
            ++*this;
            return tmp;
        }

        int128_base operator--(int) &{ // NOLINT returns non constant
            int128_base tmp = *this;
            --*this;
            return tmp;
        }

        friend constexpr int128_base operator+(int128_base lhs_, int128_base rhs_) {
            // no worry for unsigned type, won't be optimized if overflow
            return {_Hi(lhs_.high_ + rhs_.high_ + (lhs_.low_ + rhs_.low_ < lhs_.low_)), lhs_.low_ + rhs_.low_};
        }

        friend constexpr int128_base operator-(int128_base lhs_, int128_base rhs_) {
            return {_Hi(lhs_.high_ - rhs_.high_ - (lhs_.low_ < rhs_.low_)), lhs_.low_ - rhs_.low_};
        }

        friend constexpr int128_base operator&(int128_base lhs_, int128_base rhs_) {
            return {lhs_.high_ & rhs_.high_, lhs_.low_ & rhs_.low_};
        }

        friend constexpr int128_base operator|(int128_base lhs_, int128_base rhs_) {
            return {lhs_.high_ | rhs_.high_, lhs_.low_ | rhs_.low_};
        }

        friend constexpr int128_base operator^(int128_base lhs_, int128_base rhs_) {
            return {lhs_.high_ ^ rhs_.high_, lhs_.low_ ^ rhs_.low_};
        }

        friend constexpr bool operator==(int128_base lhs_, int128_base rhs_) {
            return lhs_.high_ == rhs_.high_ && lhs_.low_ == rhs_.low_;
        }

        friend constexpr bool operator>(int128_base lhs_, int128_base rhs_) { return rhs_ < lhs_; }

        friend constexpr bool operator>=(int128_base lhs_, int128_base rhs_) { return !(lhs_ < rhs_); }

        friend constexpr bool operator<=(int128_base lhs_, int128_base rhs_) { return !(rhs_ < lhs_); }

        friend constexpr bool operator!=(int128_base lhs_, int128_base rhs_) { return !(lhs_ == rhs_); }

        friend constexpr int128_base operator<<(int128_base lhs_, int128_base rhs_) { return lhs_ << (int) rhs_.low_; }

        friend constexpr int128_base operator>>(int128_base lhs_, int128_base rhs_) { return lhs_ >> (int) rhs_.low_; }

        int128_base &operator+=(int128_base rhs_) &{ return *this = *this + rhs_; }

        int128_base &operator-=(int128_base rhs_) &{ return *this = *this - rhs_; }

        int128_base &operator*=(int128_base rhs_) &{ return *this = *this * rhs_; }

        int128_base &operator/=(int128_base rhs_) &{ return *this = *this / rhs_; }

        int128_base &operator%=(int128_base rhs_) &{ return *this = *this % rhs_; }

        int128_base &operator<<=(int128_base rhs_) &{ return *this = *this << rhs_; }

        int128_base &operator>>=(int128_base rhs_) &{ return *this = *this >> rhs_; }

        int128_base &operator<<=(int rhs_) &{ return *this = *this << rhs_; }

        int128_base &operator>>=(int rhs_) &{ return *this = *this >> rhs_; }

        int128_base &operator&=(int128_base rhs_) &{ return *this = *this & rhs_; }

        int128_base &operator|=(int128_base rhs_) &{ return *this = *this | rhs_; }

        int128_base &operator^=(int128_base rhs_) &{ return *this = *this ^ rhs_; }

        template<class, class>
        friend
        class int128_base;

        template<class>
        friend
        struct clz_helper;

        template<bool>
        friend
        struct detail_delegate;
    };

    inline namespace literals {
        namespace impl_ {
            template<char _Ch, int _Rad>
            struct static_digit : std::integral_constant<int,
                    '0' <= _Ch && _Ch <= '9' ? _Ch - '0' :
                    'a' <= _Ch && _Ch <= 'z' ? _Ch - 'a' + 10 :
                    'A' <= _Ch && _Ch <= 'Z' ? _Ch - 'A' + 10 : _Rad> {
                static_assert(_Rad > static_digit::value, "character not a digit");
            };

            template<class, int, char ...>
            struct int128_literal_radix;

            template<class _Tp, int _Rad, char _Ch>
            struct int128_literal_radix<_Tp, _Rad, _Ch> {
                constexpr operator _Tp() const { return _Tp(static_digit<_Ch, _Rad>::value); } // NOLINT explicit

                constexpr _Tp operator()(_Tp v) const { return v * _Tp(_Rad) + *this; }
            };

            template<class _Tp, int _Rad, char _Ch, char ..._Args>
            struct int128_literal_radix<_Tp, _Rad, _Ch, _Args...> {
                int128_literal_radix<_Tp, _Rad, _Ch> _Cur;
                int128_literal_radix<_Tp, _Rad, _Args...> _Tgt;

                constexpr operator _Tp() const { return _Tgt(_Cur); } // NOLINT explicit

                constexpr _Tp operator()(_Tp v) const { return _Tgt(_Cur(v)); }
            };

            template<class _Tp, char ..._Args>
            struct int128_literal : int128_literal_radix<_Tp, 10, _Args...> {
            };
            template<class _Tp>
            struct int128_literal<_Tp, '0'> : int128_literal_radix<_Tp, 10, '0'> {
            };
            template<class _Tp, char ..._Args>
            struct int128_literal<_Tp, '0', _Args...> : int128_literal_radix<_Tp, 8, _Args...> {
            };
            template<class _Tp, char ..._Args>
            struct int128_literal<_Tp, '0', 'x', _Args...> : int128_literal_radix<_Tp, 16, _Args...> {
            };
            template<class _Tp, char ..._Args>
            struct int128_literal<_Tp, '0', 'X', _Args...> : int128_literal_radix<_Tp, 16, _Args...> {
            };
            template<class _Tp, char ..._Args>
            struct int128_literal<_Tp, '0', 'b', _Args...> : int128_literal_radix<_Tp, 2, _Args...> {
            };
            template<class _Tp, char ..._Args>
            struct int128_literal<_Tp, '0', 'B', _Args...> : int128_literal_radix<_Tp, 2, _Args...> {
            };
        }

        template<char ..._Args>
        constexpr uint128_t operator "" _u128() { return impl_::int128_literal<uint128_t, _Args...>(); }

        template<char ..._Args>
        constexpr int128_t operator "" _l128() { return impl_::int128_literal<int128_t, _Args...>(); }

        template<char ..._Args>
        constexpr uint128_t operator "" _U128() { return impl_::int128_literal<uint128_t, _Args...>(); }

        template<char ..._Args>
        constexpr int128_t operator "" _L128() { return impl_::int128_literal<int128_t, _Args...>(); }
    }

    template<class>
    struct clz_helper;

    template<>
    struct clz_helper<unsigned long> {
        static int clz(unsigned long val_) { return __builtin_clzl(val_); }
    };

    template<>
    struct clz_helper<unsigned long long> {
        static int clz(unsigned long long val_) { return __builtin_clzll(val_); }
    };

    template<class _High, class _Low>
    struct clz_helper<int128_base<_High, _Low> > {
        static constexpr int clz(int128_base<_High, _Low> val_) {
            return val_.high_ ? clz_helper<_Low>::clz(val_.high_) : 4 * sizeof(val_) + clz_helper<_Low>::clz(val_.low_);
        }
    };

    template<bool>
    struct detail_delegate {
        template<class _Hi, class _Low>
        static constexpr bool cmp(int128_base<_Hi, _Low> lhs_, int128_base<_Hi, _Low> rhs_) {
            return lhs_.high_ < rhs_.high_ || (lhs_.high_ == rhs_.high_ && lhs_.low_ < rhs_.low_);
        }

        static constexpr uint128_t shr(uint128_t lhs_, unsigned rhs_) {
            return rhs_ & 64U ? uint128_t(0, lhs_.high_ >> (rhs_ & 63U)) :
                   rhs_ & 63U ? uint128_t(lhs_.high_ >> (rhs_ & 63U),
                                          (lhs_.high_ << (64 - (rhs_ & 63U)) | (lhs_.low_ >> (rhs_ & 63U)))) : lhs_;
        }

        static constexpr int128_t sar(int128_t lhs_, unsigned rhs_) {
            return rhs_ & 64U ? int128_t(-(lhs_.high_ < 0), uint64_t(lhs_.high_ >> (rhs_ & 63U))) : // NOLINT
                   rhs_ & 63U ? int128_t(
                           lhs_.high_ >> (rhs_ & 63U), // NOLINT signed shift
                           (uint64_t(lhs_.high_) << (64 - (rhs_ & 63U)) | (lhs_.low_ >> (rhs_ & 63U)))) : lhs_;
        }

        template<class _Hi, class _Low>
        static constexpr int128_base<_Hi, _Low> imul(int128_base<_Hi, _Low> lhs_, int128_base<_Hi, _Low> rhs_) {
            return int128_base<_Hi, _Low>(
                    _Hi(lhs_.low_ * rhs_.high_ + rhs_.low_ * lhs_.high_) + (lhs_.low_ >> 32U) * (rhs_.low_ >> 32U),
                    (lhs_.low_ & half_mask<_Low>::value) * (rhs_.low_ & half_mask<_Low>::value))
                   + (int128_base<_Hi, _Low>((lhs_.low_ >> 32U) * (rhs_.low_ & half_mask<_Low>::value)) << 32U)
                   + (int128_base<_Hi, _Low>((rhs_.low_ >> 32U) * (lhs_.low_ & half_mask<_Low>::value)) << 32U);
        }

        template<class _Hi, class _Low>
        static constexpr int128_base<_Hi, _Low> shl(int128_base<_Hi, _Low> lhs_, unsigned rhs_) {
            // [64,127], 64 {low_ << 0, 0}
            return rhs_ & 64U ? int128_base<_Hi, _Low>(_Hi(lhs_.low_ << (rhs_ & 63U)), _Low(0)) :
                   rhs_ & 63U ? int128_base<_Hi, _Low>(
                           _Hi((_Low(lhs_.high_) << (rhs_ & 63U)) | (lhs_.low_ >> (64U - (rhs_ & 63U)))),
                           lhs_.low_ << (rhs_ & 63U)) : lhs_;
        }

        static uint128_t &slow_div_(uint128_t &dividend_, uint128_t divisor_, uint128_t &quot_) {
            // assert(divisor != uint128_t(0));
            quot_ = uint128_t(0);
            if (cmp(dividend_, divisor_)) return dividend_;
            if (dividend_.high_ == 0) { // (0,x) / ???
                quot_.low_ = dividend_.low_ / divisor_.low_;
                dividend_.low_ %= divisor_.low_;
                return dividend_;
            }
            auto zend_ = clz_helper<uint128_t>::clz(dividend_), zsor_ = clz_helper<uint128_t>::clz(divisor_);
            if (zend_ > zsor_) return dividend_;
            for (zsor_ -= zend_, divisor_ <<= zsor_;; divisor_ >>= 1, quot_ <<= 1) {
                if (dividend_ >= divisor_) {
                    dividend_ -= divisor_;
                    quot_ |= uint128_t(1);
                }
                if (!zsor_--) return dividend_;
            }
        }

        static uint128_t div(uint128_t dividend_, uint128_t divisor_) {
            if (!divisor_) return {!!dividend_ / !!divisor_}; // raise signal SIGFPE
            uint128_t quot_(0);
            slow_div_(dividend_, divisor_, quot_);
            return quot_;
        }

        static int128_t div(int128_t dividend_, int128_t divisor_) {
            bool nneg_ = dividend_.high_ < 0, dneg_ = divisor_.high_ < 0;
            auto res_ = div(uint128_t(nneg_ ? -dividend_ : dividend_), uint128_t(dneg_ ? -divisor_ : divisor_));
            return int128_t(nneg_ ^ dneg_ ? -res_ : res_);
        }

        static uint128_t mod(uint128_t dividend_, uint128_t divisor_) {
            if (!divisor_) return {!!dividend_ % !!divisor_}; // raise signal SIGFPE
            uint128_t quot_(0);
            return slow_div_(dividend_, divisor_, quot_);
        }

        static int128_t mod(int128_t dividend_, int128_t divisor_) {
            bool neg_ = dividend_.high_ < 0;
            auto res_ = mod(uint128_t(neg_ ? -dividend_ : dividend_),
                            uint128_t(divisor_.high_ < 0 ? -divisor_ : divisor_));
            return int128_t(neg_ ? -res_ : res_);
        }

        static void part_div(uint128_t value_, uint64_t div_, uint64_t &high_, uint64_t &mid_, uint64_t &low_) {
            uint128_t hh_(0), md_(0);
            low_ = static_cast<uint64_t>(slow_div_(value_, div_, md_));
            mid_ = static_cast<uint64_t>(slow_div_(md_, div_, hh_));
            high_ = static_cast<uint64_t>(hh_);
        }

        template<class _Tp>
        constexpr static _Tp cast_to_float(uint128_t val_) { return std::ldexp(_Tp(val_.high_), 64) + _Tp(val_.low_); }

        template<class _Tp>
        constexpr static _Tp cast_to_float(int128_t val_) {
            return val_.high_ < 0 ? -cast_to_float<_Tp>(uint128_t(-val_)) : cast_to_float<_Tp>(uint128_t(val_));
        }
    };

#if USE_BUILTIN_INT128

    template<>
    struct detail_delegate<true> {
        typedef __int128 ti_int_;
        typedef unsigned __int128 tu_int_;

        static constexpr ti_int_ to_native(int128_t val_) { return static_cast<ti_int_>(val_); }

        static constexpr tu_int_ to_native(uint128_t val_) { return static_cast<tu_int_>(val_); }

        static constexpr int128_t from_native(ti_int_ val_) { return int128_t(val_); }

        static constexpr uint128_t from_native(tu_int_ val_) { return uint128_t(val_); }

        template<class _Hi, class _Low>
        static constexpr bool cmp(int128_base<_Hi, _Low> lhs_, int128_base<_Hi, _Low> rhs_) {
            return to_native(lhs_) < to_native(rhs_);
        }

        static constexpr uint128_t shr(uint128_t lhs_, unsigned rhs_) {
            return from_native(to_native(lhs_) >> static_cast<decltype(to_native(lhs_))>(rhs_));
        }

        static constexpr int128_t sar(int128_t lhs_, unsigned rhs_) {
            return from_native(to_native(lhs_) >> static_cast<decltype(to_native(lhs_))>(rhs_)); // NOLINT signed shift
        }

        template<class _Hi, class _Low>
        static constexpr int128_base<_Hi, _Low> imul(int128_base<_Hi, _Low> lhs_, int128_base<_Hi, _Low> rhs_) {
            return from_native(to_native(lhs_) * to_native(rhs_));
        }

        template<class _Hi, class _Low>
        static constexpr int128_base<_Hi, _Low> shl(int128_base<_Hi, _Low> lhs_, unsigned rhs_) {
            return from_native(to_native(lhs_) << static_cast<decltype(to_native(lhs_))>(rhs_)); // NOLINT signed shift
        }

        template<class _Hi, class _Low>
        static constexpr int128_base<_Hi, _Low> div(int128_base<_Hi, _Low> lhs_, int128_base<_Hi, _Low> rhs_) {
            return from_native(to_native(lhs_) / to_native(rhs_));
        }

        template<class _Hi, class _Low>
        static constexpr int128_base<_Hi, _Low> mod(int128_base<_Hi, _Low> lhs_, int128_base<_Hi, _Low> rhs_) {
            return from_native(to_native(lhs_) % to_native(rhs_));
        }

        static void part_div(uint128_t value_, uint64_t div_, uint64_t &high_, uint64_t &mid_, uint64_t &low_) {
            // on some cpu, compiler won't do optimize for us
            auto vv_ = to_native(value_);
            auto rest_ = vv_ / div_;
            low_ = static_cast<uint64_t>(vv_) - div_ * static_cast<uint64_t>(rest_);
            high_ = static_cast<uint64_t>(rest_ / div_);
            mid_ = static_cast<uint64_t>(rest_) - div_ * high_;
        }

        template<class _Tp, class _Hi, class _Low>
        static constexpr _Tp cast_to_float(int128_base<_Hi, _Low> value_) {
            return static_cast<_Tp>(to_native(value_));
        }

    };

#endif

    constexpr bool operator<(int128_t lhs_, int128_t rhs_) { return detail_delegate<>::cmp(lhs_, rhs_); }

    constexpr bool operator<(uint128_t lhs_, uint128_t rhs_) { return detail_delegate<>::cmp(lhs_, rhs_); }

    constexpr uint128_t operator>>(uint128_t lhs_, int rhs_) {
        return detail_delegate<>::shr(lhs_, static_cast<unsigned>(rhs_));
    }

    constexpr int128_t operator>>(int128_t lhs_, int rhs_) {
        return detail_delegate<>::sar(lhs_, static_cast<unsigned>(rhs_));
    }

    constexpr int128_t operator*(int128_t lhs_, int128_t rhs_) { return detail_delegate<>::imul(lhs_, rhs_); }

    constexpr uint128_t operator*(uint128_t lhs_, uint128_t rhs_) { return detail_delegate<>::imul(lhs_, rhs_); }

    constexpr uint128_t operator<<(uint128_t lhs_, int rhs_) {
        return detail_delegate<>::shl(lhs_, static_cast<unsigned>(rhs_));
    }

    constexpr int128_t operator<<(int128_t lhs_, int rhs_) {
        return detail_delegate<>::shl(lhs_, static_cast<unsigned>(rhs_));
    }

    inline uint128_t operator/(uint128_t lhs_, uint128_t rhs_) { return detail_delegate<>::div(lhs_, rhs_); }

    inline int128_t operator/(int128_t lhs_, int128_t rhs_) { return detail_delegate<>::div(lhs_, rhs_); }

    inline uint128_t operator%(uint128_t lhs_, uint128_t rhs_) { return detail_delegate<>::mod(lhs_, rhs_); }

    inline int128_t operator%(int128_t lhs_, int128_t rhs_) { return detail_delegate<>::mod(lhs_, rhs_); }

    template<class _Hi, class _Low>
    template<class _Tp>
    constexpr _Tp int128_base<_Hi, _Low>::cast_to_float() const {
        return detail_delegate<>::cast_to_float<_Tp>(*this);
    }

    template<class _CharT, class _Traits>
    inline std::basic_ostream<_CharT, _Traits> &
    print_value(std::basic_ostream<_CharT, _Traits> &out_, bool signed_integral_, uint128_t value_) {
        constexpr std::size_t buf_size_ = 45;

        typename std::basic_ostream<_CharT, _Traits>::sentry sentry_(out_);
        if (!sentry_) return out_;
        auto flags_ = out_.flags(), base_flag_ = flags_ & std::ios::basefield;
        auto adjust_field_ = flags_ & std::ios::adjustfield;
        auto show_base_ = bool(flags_ & std::ios::showbase); // work not dec
        auto show_pos_ = bool(flags_ & std::ios::showpos); // work only dec
        auto upper_case_ = bool(flags_ & std::ios::uppercase); // work only hex
        auto ns_ = out_.width(0);
        auto fl_ = out_.fill();

        char buf_[buf_size_];
        char const *prefix_ = nullptr;
        int offset_ = 0;

        switch (base_flag_) {
            case std::ios::hex: {
                if (show_base_ && value_) prefix_ = upper_case_ ? "0X" : "0x";
                if (value_ >> 64) {
                    offset_ = snprintf(buf_, buf_size_,
                                       upper_case_ ? "%" PRIX64 "%016" PRIX64 : "%" PRIx64 "%016" PRIx64,
                                       (uint64_t) (value_ >> 64), (uint64_t) value_);
                } else {
                    offset_ = snprintf(buf_, buf_size_,
                                       upper_case_ ? "%" PRIX64 : "%" PRIx64, (uint64_t) value_);
                }
                break;
            }
            case std::ios::oct: {
                constexpr uint64_t mask_ = (UINT64_C(1) << 63U) - 1;
                if (show_base_ && value_) buf_[offset_++] = '0';
                auto x_ = (uint64_t) (value_ >> 126U);
                auto y_ = (uint64_t) (value_ >> 63U) & mask_;
                auto z_ = (uint64_t) (value_) & mask_;
                if (x_) {
                    offset_ += snprintf(buf_ + offset_, buf_size_ - offset_, "%" PRIo64 "%021" PRIo64 "%021" PRIo64,
                                        x_, y_, z_);
                } else if (y_) {
                    offset_ += snprintf(buf_ + offset_, buf_size_ - offset_, "%" PRIo64 "%021" PRIo64, y_, z_);
                } else {
                    offset_ += snprintf(buf_ + offset_, buf_size_ - offset_, "%" PRIo64, z_);
                }
                break;
            }
            default: {
                if (signed_integral_) {
                    if (value_ >> 127) { // negative
                        prefix_ = "-";
                        value_ = -value_;
                    } else if (show_pos_) {
                        prefix_ = "+";
                    }
                }
                uint64_t high_, mid_, low_;
                detail_delegate<>::part_div(value_, UINT64_C(10000000000000000000), high_, mid_, low_);
                if (high_) {
                    offset_ = snprintf(buf_, buf_size_, "%" PRIu64 "%019" PRIu64 "%019" PRIu64,
                                       high_, mid_, low_);
                } else if (mid_) {
                    offset_ = snprintf(buf_, buf_size_, "%" PRIu64 "%019" PRIu64,
                                       mid_, low_);
                } else {
                    offset_ = snprintf(buf_, buf_size_, "%" PRIu64, low_);
                }
                break;
            }
        }

        _CharT o_[2 * buf_size_ - 3] = {};
        _CharT *os_;
        _CharT *op_;  // prefix here
        _CharT *oe_ = o_ + (sizeof(o_) / sizeof(o_[0]));  // end of output

        auto loc_ = out_.getloc();
        auto &ct_ = std::use_facet<std::ctype<_CharT> >(loc_);
        auto &npt_ = std::use_facet<std::numpunct<_CharT> >(loc_);
        std::string grouping_ = npt_.grouping();

        // no worry group is not empty
        auto limit_ = grouping_.size();
        if (limit_ == 0) {
            op_ = oe_ - offset_;
            ct_.widen(buf_, buf_ + offset_, op_);
        } else {
            auto thousands_sep_ = npt_.thousands_sep();
            decltype(limit_) dg_ = 0;
            auto cnt_ = static_cast<unsigned char>(grouping_[dg_]);
            unsigned char dc_ = 0;
            --limit_;
            op_ = oe_;
            for (char *p_ = buf_ + offset_; p_ != buf_; ++dc_) {
                if (cnt_ > 0 && dc_ == cnt_) {
                    *--op_ = thousands_sep_;
                    dc_ = 0;
                    if (dg_ < limit_) cnt_ = static_cast<unsigned char>(grouping_[++dg_]);
                }
                *--op_ = ct_.widen(*--p_);
            }
        }

        if (prefix_) {
            auto prefix_len_ = strlen(prefix_);
            os_ = op_ - prefix_len_;
            ct_.widen(prefix_, prefix_ + prefix_len_, os_);
        } else {
            os_ = op_;
        }

        auto sz_ = static_cast<std::streamsize>(oe_ - os_);
        // assert(sz_ <= (sizeof(o_) / sizeof(o_[0])));

        if (ns_ > sz_) {
            ns_ -= sz_;
            std::basic_string<_CharT, _Traits> sp_(ns_, fl_);
            switch (adjust_field_) {
                case std::ios::left:
                    return out_.write(os_, sz_).write(sp_.data(), ns_);
                case std::ios::internal:
                    return out_.write(os_, static_cast<std::streamsize>(op_ - os_))
                            .write(sp_.data(), ns_)
                            .write(op_, static_cast<std::streamsize>(oe_ - op_));
                default:
                    return out_.write(sp_.data(), ns_).write(os_, sz_);
            }
        }
        return out_.write(os_, sz_);
    }

    template<class _CharT, class _Traits>
    inline std::basic_ostream<_CharT, _Traits> &operator<<(std::basic_ostream<_CharT, _Traits> &out, uint128_t _Val) {
        return print_value(out, false, _Val);
    }

    template<class _CharT, class _Traits>
    inline std::basic_ostream<_CharT, _Traits> &operator<<(std::basic_ostream<_CharT, _Traits> &out, int128_t _Val) {
        return print_value(out, true, uint128_t(_Val));
    }
}

#ifdef INT128_SPECIALIZATION
namespace std {
#pragma push_macro("MAKE_TYPE")
#define MAKE_TYPE(outter, inner, parent) \
template<> struct outter<large_int::inner> : std::parent {}; \
template<> struct outter<const large_int::inner> : std::parent {}; \
template<> struct outter<volatile large_int::inner> : std::parent {}; \
template<> struct outter<const volatile large_int::inner> : std::parent {};
    MAKE_TYPE(is_integral, uint128_t, true_type)
    MAKE_TYPE(is_integral, int128_t, true_type)
    MAKE_TYPE(is_signed, uint128_t, false_type)
    MAKE_TYPE(is_signed, int128_t, true_type)
#undef MAKE_TYPE
#define MAKE_TYPE(outter, inner, target) \
template<> struct outter<large_int::inner> { typedef large_int::target type; }; \
template<> struct outter<const large_int::inner> { typedef const large_int::target type; }; \
template<> struct outter<volatile large_int::inner> { typedef volatile large_int::target type; }; \
template<> struct outter<const volatile large_int::inner> { typedef const volatile large_int::target type; };
    MAKE_TYPE(make_signed, uint128_t, int128_t)
    MAKE_TYPE(make_unsigned, int128_t, uint128_t)
#pragma pop_macro("MAKE_TYPE")

    template<class _Hi, class _Low>
    struct numeric_limits<large_int::int128_base<_Hi, _Low> > {
    private:
        typedef large_int::int128_base<_Hi, _Low> _Tp;
    public:
        static constexpr const bool is_specialized = true;
        static constexpr const bool is_signed = numeric_limits<_Hi>::is_signed;
        static constexpr const bool is_integer = true;
        static constexpr const bool is_exact = true;
        static constexpr const bool has_infinity = false;
        static constexpr const bool has_quiet_NaN = false;
        static constexpr const bool has_signaling_NaN = false;
        static constexpr const std::float_denorm_style has_denorm = std::denorm_absent;
        static constexpr const bool has_denorm_loss = false;
        static constexpr const std::float_round_style round_style = std::round_toward_zero;
        static constexpr const bool is_iec559 = false;
        static constexpr const bool is_bounded = true;
        static constexpr const bool is_modulo = numeric_limits<_Hi>::is_modulo;
        static constexpr const int digits = static_cast<int>(sizeof(_Tp) * 8 - is_signed);
        static constexpr const int digits10 = digits * 3 / 10;
        static constexpr const int max_digits10 = 0;
        static constexpr const int radix = 2;
        static constexpr const int min_exponent = 0;
        static constexpr const int min_exponent10 = 0;
        static constexpr const int max_exponent = 0;
        static constexpr const int max_exponent10 = 0;
        static constexpr const bool traps = numeric_limits<_Hi>::traps;
        static constexpr const bool tinyness_before = false;

        static constexpr _Tp min() { return is_signed ? _Tp(1) << digits : _Tp(0); }

        static constexpr _Tp lowest() { return min(); }

        static constexpr _Tp max() { return ~min(); }

        static constexpr _Tp epsilon() { return _Tp(0); }

        static constexpr _Tp round_error() { return _Tp(0); }

        static constexpr _Tp infinity() { return _Tp(0); }

        static constexpr _Tp quiet_NaN() { return _Tp(0); }

        static constexpr _Tp signaling_NaN() { return _Tp(0); }

        static constexpr _Tp denorm_min() { return _Tp(0); }
    };
}
#endif /* INT128_SPECIALIZATION */

#ifndef INT128_NO_EXPORT
#define INT128_C(val) val##_L128
#define UINT128_C(val) val##_U128
// add space between '""' and suffix identifier, or may compile failed
using namespace large_int::literals;
using large_int::uint128_t;
using large_int::int128_t;
#endif /* INT128_NO_EXPORT */
