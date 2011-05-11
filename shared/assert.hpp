#define BOOST_ENABLE_ASSERT_HANDLER
#include <boost/assert.hpp>

#define spinxml_assert(x) BOOST_ASSERT((x))
#define nanAssert(x) BOOST_ASSERT((x != x))

typedef void (*AssertHandler_t)(char const * expr, char const * function, char const * file, long line);

void SetAssertHandler(AssertHandler_t handler);
