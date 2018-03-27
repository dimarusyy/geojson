#pragma once

#include "details/primitives.h"
#include "details/geometry.h"

namespace geojson
{
	//////////////////////////////////////////////////////////////////////////

	template <typename T = double>
	using point_t = details::point_t<T>;

	template <typename T = double>
	using line_t = details::line_t<T>;

	template <typename T = double>
	using polygon_t = details::polygon_t<T>;

	template <typename T = double>
	using multipoint_t = details::multipoint_t<T>;

	template <typename T = double>
	using multiline_t = details::multiline_t<T>;

	template <typename T = double>
	using multipolygon_t = details::multipolygon_t<T>;

	//////////////////////////////////////////////////////////////////////////

	template <typename T = double>
	using geometry_t = details::geometry_t<T>;

	//////////////////////////////////////////////////////////////////////////
}
