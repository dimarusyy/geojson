#pragma once
#include "primitives.h"
#include <boost/spirit/include/qi.hpp>
#include <sstream>

namespace geojson
{
	namespace details
	{
		//////////////////////////////////////////////////////////////////////////

		namespace qi = boost::spirit::qi;

		//////////////////////////////////////////////////////////////////////////

		template <typename S, typename T>
		struct point_translator_t
		{
			typedef S internal_type;
			typedef T external_type;

			boost::optional<external_type> get_value(const internal_type& str) 
			{
				std::pair<double, double> p;
				auto s_it = std::begin(str), e_it = std::end(str);
				auto rc = qi::phrase_parse(s_it, e_it, qi::lit("[") << qi::double_ << qi::lit(",") << qi::double_ << qi::lit("]"), qi::space, p);
				return boost::make_optional(rc && (s_it == e_it), point_t<double>(p));
			}

			boost::optional<internal_type> put_value(const external_type& obj)
			{
				std::stringstream ss;
				ss << "[" << obj._x << "," << "]";
				return ss.str();
			}
		};
		//////////////////////////////////////////////////////////////////////////
	}
}
