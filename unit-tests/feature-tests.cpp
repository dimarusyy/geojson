#include "stdafx.h"

#include <geojson/geojson.h>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(feature_tests)

BOOST_AUTO_TEST_CASE(object_of_point_create)
{
	geojson::feature_t<> ft(geojson::point_t<>{10, 11});
	//BOOST_ASSERT_MSG(g.is_of_type<geojson::point_t<double>>(), "p is not of geojson::point_t type");
}

BOOST_AUTO_TEST_SUITE_END()