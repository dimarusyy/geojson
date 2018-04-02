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

BOOST_AUTO_TEST_CASE(object_serialize)
{
	geojson::object_t<> obj_orig{ geojson::point_t<>{10, 2} };

	std::stringstream ss;
	ss << obj_orig;

	auto result = ss.str();
	BOOST_TEST_MESSAGE(result);
	BOOST_ASSERT_MSG(!result.empty(), "serialization string should not be empty");

	geojson::object_t<> obj;
	ss >> obj;

	BOOST_TEST_MESSAGE("deserialized");
}

 BOOST_AUTO_TEST_CASE(object_deserialize)
 {
 	std::string ser_str("{ \"type\": \"LineString\", \"coordinates\": [[10,2], [11,3]]}");
 	geojson::object_t<> obj;
 
 	std::stringstream ss;
 	ss << ser_str;
 	ss >> obj;
 }

BOOST_AUTO_TEST_SUITE_END()