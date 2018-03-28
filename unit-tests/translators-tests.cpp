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

	auto deser_data = t.get_value(ser_data.get());
	BOOST_ASSERT_MSG(deser_data.is_initialized(), "deser_data is not of geojson::point_t type");
	BOOST_ASSERT_MSG(deser_data.get()._x == p._x && deser_data.get()._y == p._y, "");
}

BOOST_AUTO_TEST_CASE(translator_point_t_deserialize)
{
	geojson::details::point_translator_t<> t{};
	auto deser_data = t.get_value(" [ 10 , 11 ] ");
	BOOST_ASSERT_MSG(deser_data.is_initialized(), "deserialization failed");
}

BOOST_AUTO_TEST_CASE(translator_line_t_serialize)
{
	geojson::line_t<> l{ { 10, 2 }, { 13, 87 }, { 9, 8 } };
	geojson::details::line_translator_t<> t{};
	auto ser_data = t.put_value(l);
	BOOST_ASSERT_MSG(ser_data.is_initialized(), "serialization failed");

	auto deser_data = t.get_value(ser_data.get());
	BOOST_ASSERT_MSG(deser_data.is_initialized(), "deser_data is not of geojson::line_t type");
}

BOOST_AUTO_TEST_CASE(translator_line_t_deserialize)
{
	geojson::details::line_translator_t<> t{};
	auto deser_data = t.get_value(" [ [ 10 , 11 ], [11, 12], [19, 20] ] ");
	BOOST_ASSERT_MSG(deser_data.is_initialized(), "deserialization failed");
	BOOST_ASSERT_MSG((deser_data.value().get().at(0) == geojson::point_t<double>{10,11}), "deserialization failed");
	BOOST_ASSERT_MSG((deser_data.value().get().at(1) == geojson::point_t<double>{11,12}), "deserialization failed");
	BOOST_ASSERT_MSG((deser_data.value().get().at(2) == geojson::point_t<double>{19,20}), "deserialization failed");
}

BOOST_AUTO_TEST_SUITE_END()