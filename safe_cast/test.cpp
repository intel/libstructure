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
#include "safe_cast.hpp"
#include <catch.hpp>

using namespace structure;

TEST_CASE("Checked casts", "[cast]")
{
    SECTION ("Integers") {
        unsigned char uc;
        signed char c;
        int i;
        long long ll;
        unsigned long long ull;

        uc = 1;
        CHECK(safe_cast<signed char>(uc) == 1);
        uc = 255;
        CHECK_THROWS_AS(safe_cast<signed char>(uc), CastError);

        c = 1;
        CHECK(safe_cast<unsigned char>(c) == 1);
        c = -1;
        CHECK_THROWS_AS(safe_cast<unsigned char>(c), CastError);

        i = 256;
        CHECK_THROWS_AS(safe_cast<unsigned char>(i), CastError);
        CHECK_THROWS_AS(safe_cast<signed char>(i), CastError);

        i = 255;
        CHECK(safe_cast<unsigned char>(i) == 255);
        CHECK_THROWS_AS(safe_cast<signed char>(i), CastError);

        i = 1;
        CHECK(safe_cast<unsigned char>(i) == 1);
        CHECK(safe_cast<signed char>(i) == 1);

        i = -1;
        CHECK_THROWS_AS(safe_cast<unsigned char>(i), CastError);
        CHECK(safe_cast<signed char>(i) == -1);

        c = -1;
        CHECK_THROWS_AS(safe_cast<unsigned int>(c), CastError);
        CHECK(safe_cast<int>(c) == -1);

        ll = 255;
        CHECK(safe_cast<unsigned long long>(ll) == 255);

        ull = std::numeric_limits<unsigned long long>::max();
        CHECK_THROWS_AS(safe_cast<long long>(ull), CastError);
    }

    SECTION ("FloatingPoints") {
        float f;
        double d;

        d = std::numeric_limits<float>::lowest();
        CHECK(safe_cast<float>(d) == Approx(d));

        d = std::numeric_limits<double>::lowest();
        CHECK_THROWS_AS(safe_cast<float>(d), CastError);

        d = std::numeric_limits<float>::max();
        CHECK(safe_cast<float>(d) == Approx(d));

        d = std::numeric_limits<double>::max();
        CHECK_THROWS_AS(safe_cast<float>(d), CastError);

        f = std::numeric_limits<float>::max();
        CHECK(safe_cast<double>(f) == Approx(f));

        f = std::numeric_limits<float>::lowest();
        CHECK(safe_cast<double>(f) == Approx(f));
    }
}
