#pragma once

#include "details/position.h"

namespace geojson
{
	//////////////////////////////////////////////////////////////////////////

	template <typename T = double>
	using point_t = typename details::primitive_types<T>::point_t;

	template <typename T = double>
	using line_t = typename details::primitive_types<T>::line_t;

	template <typename T = double>
	using polygon_t = typename details::primitive_types<T>::polygon_t;

	template <typename T = double>
	using multipoint_t = typename details::primitive_types<T>::multipoint_t;

	template <typename T = double>
	using multiline_t = typename details::primitive_types<T>::multiline_t;

	template <typename T = double>
	using multipolygon_t = typename details::primitive_types<T>::multipolygon_t;

	template <typename TPrimitive = boost::blank, typename T = double>
	using position_t = details::position_t<TPrimitive, T>;

	//////////////////////////////////////////////////////////////////////////
}
