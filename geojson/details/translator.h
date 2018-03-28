#pragma once
#include "primitives.h"

#ifdef _DEBUG
#define BOOST_SPIRIT_DEBUG
#if !defined(BOOST_SPIRIT_DEBUG_OUT)
#define BOOST_SPIRIT_DEBUG_OUT std::cerr
#endif
#endif

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
						qi::lit("[") >> (point_t_parser % qi::lit(",")) >> qi::lit("]")
					)
					[
						qi::_val = phx::construct<line_t<double>>(phx::begin(qi::_1), phx::end(qi::_1))
					];
				BOOST_SPIRIT_DEBUG_NODE(start);
			}

		private:
			point_t_grammar<Iterator, Skipper> point_t_parser;
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

		template <typename T = double>
		struct line_translator_t
		{
			using internal_type = std::string;
			using external_type = line_t<T>;

			boost::optional<external_type> get_value(const internal_type& str)
			{
				line_t<double> p{};
				auto s_it = std::begin(str), e_it = std::end(str);
				line_t_grammar<internal_type::const_iterator, qi::ascii::space_type> g{};
				auto rc = qi::phrase_parse(s_it, e_it, g, qi::ascii::space, p);
				return boost::make_optional(rc && (s_it == e_it), p);
			}

			boost::optional<internal_type> put_value(const external_type& obj)
			{
				point_translator_t<T> t;
				std::stringstream ss;
				ss << "[";
				for (const auto& v : obj.get())
				{
					ss << t.put_value(v);
				}
				ss << "]" ;
				return ss.str();
			}
		};
		//////////////////////////////////////////////////////////////////////////
	}
}
