#include "stdafx.h"

#include <geojson/geojson.h>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(object_tests)

BOOST_AUTO_TEST_CASE(object_of_point_create)
{
	geojson::point_t<> p{ 10, 11 };
	geojson::object_t<> g(p);
	BOOST_ASSERT_MSG(g.is_of_type<geojson::point_t<double>>(), "p is not of geojson::point_t type");
}

BOOST_AUTO_TEST_CASE(object_move_create)
{
	geojson::point_t<> p{ 10, 11 };
	geojson::object_t<> g1(p);

	geojson::line_t<> l{ { 10, 2 },{ 13, 87 },{ 9, 8 } };
	geojson::object_t<> g2(l);

	g2 = g1;
	BOOST_ASSERT_MSG(g2.is_of_type<geojson::point_t<double>>(), "g2 is not of geojson::point_t type");
}

BOOST_AUTO_TEST_SUITE_END()