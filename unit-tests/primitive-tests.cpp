#include "stdafx.h"

#include <geojson/geojson.h>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(primitive_tests)

BOOST_AUTO_TEST_CASE(test_point_creation)
{
	geojson::point_t<> p1{ 10, 11 };
	geojson::point_t<> p2{ std::make_pair(13, 14) };

	BOOST_STATIC_ASSERT_MSG(std::is_same_v<decltype(p1)::value_type, double>, "");
	BOOST_STATIC_ASSERT_MSG(std::is_same_v<decltype(p2)::value_type, double>, "");
}

BOOST_AUTO_TEST_CASE(test_line_creation)
{
	geojson::line_t<> l1{ { 10, 2 }, { 13, 87 }, {9, 8} };
	BOOST_STATIC_ASSERT_MSG(std::is_same_v<decltype(l1)::value_type, geojson::line_t<double>::value_type>, "");

	geojson::point_t<> p1(10, 2);
	geojson::point_t<> p2(11, 3);
	geojson::point_t<> p3(12, 4);
	geojson::line_t<> l2(p1, p2, p3);
	BOOST_STATIC_ASSERT_MSG(std::is_same_v<decltype(l2)::value_type, geojson::line_t<double>::value_type>, "");
}

BOOST_AUTO_TEST_CASE(test_polygon_creation)
{
	geojson::line_t<> l1{ { 10, 2 },{ 13, 87 },{ 9, 8 },{ 10, 2 } };
	geojson::line_t<> l2{ { 11, 12 },{ 13, 17 },{ 9, 18 }, { 11, 12 } };
	geojson::polygon_t<> poly1{ l1, l2 };
	BOOST_STATIC_ASSERT_MSG(std::is_same_v<decltype(poly1)::value_type, geojson::polygon_t<double>::value_type>, "");

	geojson::polygon_t<> poly2{
		geojson::line_t<>{ {10, 2},{ 11, 3 },{ 10, 2 } },
		geojson::line_t<>{ {10, 21},{ 11, 3 },{ 10, 21 } },
	};
	BOOST_STATIC_ASSERT_MSG(std::is_same_v<decltype(poly2)::value_type, geojson::polygon_t<double>::value_type>, "");
}

BOOST_AUTO_TEST_CASE(test_wrong_polygon_creation)
{
	geojson::line_t<> l1{ { 10, 2 },{ 13, 87 },{ 9, 8 } };
	geojson::line_t<> l2{ { 11, 12 },{ 13, 17 },{ 9, 18 } };
	BOOST_CHECK_THROW(geojson::polygon_t<> poly1( l1, l2 ), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(test_multipoint_creation)
{
	geojson::multipoint_t<> mp1{ { 10, 2 },{ 13, 87 } };
	BOOST_STATIC_ASSERT_MSG(std::is_same_v<decltype(mp1)::value_type, geojson::multipoint_t<double>::value_type>, "");
}

BOOST_AUTO_TEST_CASE(test_multiline_creation)
{
	geojson::line_t<> l1{ { 10, 2 },{ 1, 8 },{ 9, 8 } };
	geojson::line_t<> l2{ { 101, 21 },{ 13, 87 },{ 91, 81 } };
	geojson::multiline_t<> ml1{ l1, l2 };
	BOOST_STATIC_ASSERT_MSG(std::is_same_v<decltype(ml1)::value_type, geojson::multiline_t<double>::value_type>, "");

	geojson::multiline_t<> ml2{ 
		geojson::line_t<>{ { 10, 2 },{ 1, 8 },{ 9, 8 } },
		geojson::line_t<>{ { 101, 21 },{ 13, 87 },{ 91, 81 } }
	};
	BOOST_STATIC_ASSERT_MSG(std::is_same_v<decltype(ml2)::value_type, geojson::multiline_t<double>::value_type>, "");
}

BOOST_AUTO_TEST_CASE(test_multipolygon_creation)
{
	geojson::line_t<> l1{ { 10, 2 },{ 13, 87 },{ 9, 8 },{ 10, 2 } };
	geojson::line_t<> l2{ { 11, 12 },{ 13, 17 },{ 9, 18 },{ 11, 12 } };
	geojson::multiline_t<> mpoly1{ l1, l2 };
	BOOST_STATIC_ASSERT_MSG(std::is_same_v<decltype(mpoly1)::value_type, geojson::multiline_t<double>::value_type>, "");

	geojson::multiline_t<> mpoly2{
		geojson::line_t<>{ {10, 2},{ 11, 3 },{ 10, 2 } },
		geojson::line_t<>{ {10, 21},{ 11, 3 },{ 10, 21 } },
	};
	BOOST_STATIC_ASSERT_MSG(std::is_same_v<decltype(mpoly2)::value_type, geojson::multiline_t<double>::value_type>, "");
}


BOOST_AUTO_TEST_SUITE_END()