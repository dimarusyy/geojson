#pragma once
#include "primitives.h"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

#include <sstream>

namespace geojson
{
	namespace details
	{
		//////////////////////////////////////////////////////////////////////////

		namespace qi = boost::spirit::qi;
		namespace phx = boost::phoenix;

		//////////////////////////////////////////////////////////////////////////

		template <typename Iterator, typename Skipper>
		struct point_t_grammar : qi::grammar<Iterator, point_t<double>(), Skipper>
		{
			point_t_grammar() : point_t_grammar::base_type(start, "point_t_grammar")
			{
				start =
					(
						qi::lit("[") >> qi::double_ >> qi::lit(",") >> qi::double_ >> qi::lit("]")
					)
 					[
						qi::_val = phx::construct<point_t<double>>(qi::_1, qi::_2)
					];
				BOOST_SPIRIT_DEBUG_NODE(start);
			}
		
		private:
			qi::rule<Iterator, point_t<double>(), Skipper> start;
		};

		template <typename Iterator, typename Skipper>
		struct line_t_grammar : qi::grammar<Iterator, line_t<double>(), Skipper>
		{
			line_t_grammar() : line_t_grammar::base_type(start, "line_t_grammar")
			{
				start =
					(
						qi::lit("[") >> qi::double_ >> qi::lit(",") >> qi::double_ >> qi::lit("]")
						)
					[
						qi::_val = phx::construct<point_t<double>>(qi::_1, qi::_2)
					];
				BOOST_SPIRIT_DEBUG_NODE(start);
			}

		private:
			qi::rule<Iterator, line_t<double>(), Skipper> start;
		};

		//////////////////////////////////////////////////////////////////////////

		template <typename T = double>
		struct point_translator_t
		{
			using internal_type = std::string;
			using external_type = point_t<T>;

			boost::optional<external_type> get_value(const internal_type& str) 
			{
				point_t<double> p{};
				auto s_it = std::begin(str), e_it = std::end(str);
				point_t_grammar<internal_type::const_iterator, qi::ascii::space_type> g{};
				auto rc = qi::phrase_parse(s_it, e_it, g, qi::ascii::space, p);
				return boost::make_optional(rc && (s_it == e_it), p);
			}

			boost::optional<internal_type> put_value(const external_type& obj)
			{
				std::stringstream ss;
				ss << "[" << obj._x << "," << obj._y << "]";
				return ss.str();
			}
		};

		//////////////////////////////////////////////////////////////////////////
	}
}
