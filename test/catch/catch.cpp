/** Accelerate test build by not recompiling catch when a test change.
 * See: https://github.com/philsquared/Catch/blob/master/docs/slow-compiles.md
 */
#define CATCH_CONFIG_MAIN
#include <catch.hpp>
