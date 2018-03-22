#include "stdafx.h"

#include <geojson/geojson.h>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(position_tests)

BOOST_AUTO_TEST_CASE(test_point_creation)
{
	geojson::position_t<> p1(10, 2);
 	geojson::position_t<> p2{ 11, 3 };
 	geojson::position_t<> p3(std::initializer_list<double>{ 21, 22 });

	BOOST_ASSERT_MSG(p1.is_of_type<geojson::point_t<>>(), "p1 is not a point_t");
	BOOST_ASSERT_MSG(p2.is_of_type<geojson::point_t<>>(), "p2 is not a point_t");
	BOOST_ASSERT_MSG(p3.is_of_type<geojson::point_t<>>(), "p3 is not a point_t");
}

BOOST_AUTO_TEST_CASE(test_line_creation)
{
 	geojson::position_t<geojson::line_t<>> l2{ { 10, 2 }, { 13, 87 } };

	geojson::position_t<geojson::line_t<>> l4(std::initializer_list<double>{ 10, 2 }, std::initializer_list<double>{ 13, 87 });

	geojson::position_t<> p1(10, 2);
	geojson::position_t<> p2{ 11, 3 };
	geojson::position_t<geojson::line_t<>> l5_1( p1, p2 );

	geojson::position_t<geojson::line_t<>> l5_2{ p1, p2 };

	geojson::position_t<geojson::line_t<>> l6(geojson::position_t<>{ 11, 3 }, geojson::position_t<>{ 12, 4 });

	BOOST_ASSERT_MSG(l2.is_of_type<geojson::line_t<>>(), "l2 is not a line_t");
	BOOST_ASSERT_MSG(l4.is_of_type<geojson::line_t<>>(), "l4 is not a line_t");
	BOOST_ASSERT_MSG(l5_1.is_of_type<geojson::line_t<>>(), "l5_1 is not a line_t");
	BOOST_ASSERT_MSG(l5_1.is_of_type<geojson::line_t<>>(), "l5_2 is not a line_t");
	BOOST_ASSERT_MSG(l6.is_of_type<geojson::line_t<>>(), "l6 is not a line_t");
}

BOOST_AUTO_TEST_CASE(test_polygon_creation)
{
	geojson::position_t<geojson::line_t<>> l1{ {10, 2}, { 11, 3 } };
	geojson::position_t<geojson::line_t<>> l2{ {13, 87}, { 14, 8 } };
	geojson::position_t<geojson::polygon_t<>> poly1{ l1, l2 };

	geojson::position_t<geojson::polygon_t<>> poly2 { 
		geojson::position_t<geojson::line_t<>>{ {10, 2},{ 11, 3 } }, 
		geojson::position_t<geojson::line_t<>>{ { 13, 87 },{ 14, 8 } } 
	};

	BOOST_ASSERT_MSG(poly1.is_of_type<geojson::polygon_t<>>(), "poly1 is not a polygon_t");
	BOOST_ASSERT_MSG(poly2.is_of_type<geojson::polygon_t<>>(), "poly2 is not a polygon_t");
}

BOOST_AUTO_TEST_CASE(test_multipoint_creation)
{
	geojson::position_t<> mp1{ { 10, 2 },{ 13, 87 } };
	geojson::position_t<geojson::multipoint_t<>> mp2{ { 10, 2 },{ 13, 87 } };

	BOOST_ASSERT_MSG(mp1.is_of_type<geojson::multipoint_t<>>(), "mp1 is not a mupltipoint_t");
	BOOST_ASSERT_MSG(mp2.is_of_type<geojson::multipoint_t<>>(), "mp2 is not a mupltipoint_t");
}

BOOST_AUTO_TEST_CASE(test_multiline_creation)
{
	geojson::position_t<geojson::line_t<>> l1{ { 10, 2 },{ 11, 3 } };
	geojson::position_t<geojson::line_t<>> l2{ { 13, 87 },{ 14, 8 } };
	geojson::position_t<geojson::multiline_t<>> ml1{ l1, l2 };

	geojson::position_t<geojson::multiline_t<>> ml2{
		geojson::position_t<geojson::line_t<>>{ {10, 2},{ 11, 3 } },
		geojson::position_t<geojson::line_t<>>{ { 13, 87 },{ 14, 8 } }
	};

	BOOST_ASSERT_MSG(ml1.is_of_type<geojson::multiline_t<>>(), "ml1 is not a multiline_t");
	BOOST_ASSERT_MSG(ml2.is_of_type<geojson::multiline_t<>>(), "ml2 is not a multiline_t");
}

BOOST_AUTO_TEST_CASE(test_multipolygon_creation)
{
	geojson::position_t<geojson::polygon_t<>> ml1{
		geojson::position_t<geojson::line_t<>>{ {10, 2},{ 11, 3 } },
		geojson::position_t<geojson::line_t<>>{ { 13, 87 },{ 14, 8 } }
	};

	geojson::position_t<geojson::polygon_t<>> ml2{
		geojson::position_t<geojson::line_t<>>{ {101, 21},{ 111, 31 } },
		geojson::position_t<geojson::line_t<>>{ { 131, 871 },{ 141, 81 } }
	};

	geojson::position_t<geojson::multipolygon_t<>> mp1{ ml1, ml2 };

	BOOST_ASSERT_MSG(mp1.is_of_type<geojson::multipolygon_t<>>(), "mp1 is not a multiline_t");
}

BOOST_AUTO_TEST_SUITE_END()