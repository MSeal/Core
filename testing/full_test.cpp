#define BOOST_TEST_MAIN
#define BOOST_LIB_NAME boost_unit_test_framework
#define BOOST_TEST_MODULE CoreTestSuite
#define BOOST_TEST_LOG_LEVEL message

// Don't listen to warnings about boost on msvc
#if defined(_MSC_VER) && (_MSC_VER >= 1500)
#   pragma warning(push, 0)
#endif
#include <boost/test/unit_test.hpp>
#if defined(_MSC_VER) && (_MSC_VER >= 1500)
#   pragma warning(pop)
#endif

#include <stdio.h>

#include "test_num_util.hpp"
#include "test_string_util.hpp"
#include "test_exceptions.hpp"
#include "test_ts_queue.hpp"
#include "test_pp_types.hpp"
#include "test_smart_pointer.hpp"
#include "test_loops.hpp"
#include "test_singleton.hpp"
#include "test_time.hpp"
#include "test_factory.hpp"
#include "test_enum.hpp"
