#include "stdafx.h"

#include <geojson/geojson.h>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(translators_tests)

BOOST_AUTO_TEST_CASE(translator_point_t_serialize)
{
	geojson::point_t<> p{ 10, 11 };
	
	geojson::details::point_translator_t<> t{};
	auto ser_data = t.put_value(p);
	BOOST_ASSERT_MSG(ser_data.is_initialized(), "serialization failed");
}

BOOST_AUTO_TEST_CASE(translator_point_t_deserialize)
{
	geojson::details::point_translator_t<> t{};
	auto deser_data = t.get_value(" [ 10 , 11 ] ");
	BOOST_ASSERT_MSG(deser_data.is_initialized(), "deser_data is not of geojson::point_t type");
}

// BOOST_AUTO_TEST_CASE(geometry_move_create)
// {
// 	geojson::point_t<> p{ 10, 11 };
// 	geojson::geometry_t<> g1(p);
// 
// 	geojson::line_t<> l{ { 10, 2 },{ 13, 87 },{ 9, 8 } };
// 	geojson::geometry_t<> g2(l);
// 
// 	g2 = g1;
// 	BOOST_ASSERT_MSG(g2.is_of_type<geojson::point_t<double>>(), "g2 is not of geojson::point_t type");
// }

BOOST_AUTO_TEST_SUITE_END()