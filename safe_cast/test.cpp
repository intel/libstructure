#include <safe_cast.hpp>
#include <catch.hpp>

TEST_CASE("Checked casts", "[cast]")
{
    SECTION ("Integers") {
        unsigned char uc;
        signed char c;
        int i;
        unsigned int ui;
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
    }

    SECTION ("FloatingPoints") {
        float f;
        double d;

        d = std::numeric_limits<float>::min();
        CHECK(safe_cast<float>(d) == Approx(d));

        d = std::numeric_limits<double>::min();
        CHECK_THROWS_AS(safe_cast<float>(d), CastError);

        d = std::numeric_limits<float>::max();
        CHECK(safe_cast<float>(d) == Approx(d));

        d = std::numeric_limits<double>::max();
        CHECK_THROWS_AS(safe_cast<float>(d), CastError);

        f = std::numeric_limits<float>::max();
        CHECK(safe_cast<double>(f) == Approx(f));

        f = std::numeric_limits<float>::min();
        CHECK(safe_cast<double>(f) == Approx(f));
    }
}
