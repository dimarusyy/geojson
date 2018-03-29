#pragma once
#include "primitives.h"

#ifdef _DEBUG
#define BOOST_SPIRIT_DEBUG
#if !defined(BOOST_SPIRIT_DEBUG_OUT)
#define BOOST_SPIRIT_DEBUG_OUT std::cerr
#endif
#endif

#include <boost/algorithm/string.hpp>
#include <boost/range/adaptors.hpp>
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
					(qi::lit("[") >> qi::double_ >> qi::lit(",") >> qi::double_ >> qi::lit("]"))
 					[qi::_val = phx::construct<point_t<double>>(qi::_1, qi::_2)];
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
					(qi::lit("[") >> (point_t_parser % qi::lit(",")) >> qi::lit("]"))
					[qi::_val = phx::construct<line_t<double>>(phx::begin(qi::_1), phx::end(qi::_1))];
				BOOST_SPIRIT_DEBUG_NODE(start);
			}

		private:
			point_t_grammar<Iterator, Skipper> point_t_parser;
			qi::rule<Iterator, line_t<double>(), Skipper> start;
		};

		template <typename Iterator, typename Skipper>
		struct polygon_t_grammar : qi::grammar<Iterator, polygon_t<double>(), Skipper>
		{
			polygon_t_grammar() : polygon_t_grammar::base_type(start, "polygon_t_grammar")
			{
				start =
					(qi::lit("[") >> (line_t_parser % qi::lit(",")) >> qi::lit("]"))
					[qi::_val = phx::construct<polygon_t<double>>(phx::begin(qi::_1), phx::end(qi::_1))];
				BOOST_SPIRIT_DEBUG_NODE(start);
			}

		private:
			line_t_grammar<Iterator, Skipper> line_t_parser;
			qi::rule<Iterator, polygon_t<double>(), Skipper> start;
		};

		//////////////////////////////////////////////////////////////////////////

		template <typename T = double>
		struct point_translator_t
		{
			using internal_type = std::string;
			using external_type = point_t<T>;

			boost::optional<external_type> get_value(const internal_type& str) 
			{
				point_t<double> out_value{};
				auto s_it = std::begin(str), e_it = std::end(str);
				point_t_grammar<internal_type::const_iterator, qi::ascii::space_type> g{};
				auto rc = qi::phrase_parse(s_it, e_it, g, qi::ascii::space, out_value);
				return boost::make_optional(rc && (s_it == e_it), out_value);
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
				line_t<double> out_value{};
				auto s_it = std::begin(str), e_it = std::end(str);
				line_t_grammar<internal_type::const_iterator, qi::ascii::space_type> g{};
				auto rc = qi::phrase_parse(s_it, e_it, g, qi::ascii::space, out_value);
				return boost::make_optional(rc && (s_it == e_it), out_value);
			}

			boost::optional<internal_type> put_value(const external_type& obj)
			{
				point_translator_t<T> t;
				std::stringstream ss;
				ss << "[" << std::endl;
				ss << boost::algorithm::join(obj.get() | boost::adaptors::transformed([&](const auto& p) 
				{ 
					auto rc = t.put_value(p);
					if (!rc.is_initialized())
					{
						std::stringstream sout;
						sout << "can't serialize" << p;
						throw std::runtime_error(sout.str());
					}
					return rc.value(); 
				}), ",");
				ss << std::endl << "]" ;
				return ss.str();
			}
		};

		template <typename T = double>
		struct polygon_translator_t
		{
			using internal_type = std::string;
			using external_type = polygon_t<T>;

			boost::optional<external_type> get_value(const internal_type& str)
			{
				polygon_t<double> out_value{};
				auto s_it = std::begin(str), e_it = std::end(str);
				polygon_t_grammar<internal_type::const_iterator, qi::ascii::space_type> g{};
				auto rc = qi::phrase_parse(s_it, e_it, g, qi::ascii::space, out_value);
				return boost::make_optional(rc && (s_it == e_it), out_value);
			}

			boost::optional<internal_type> put_value(const external_type& obj)
			{
				line_translator_t<T> t;
				std::stringstream ss;
				ss << "[" << std::endl;
				ss << boost::algorithm::join(obj.get() | boost::adaptors::transformed([&](const auto& l)
				{
					auto rc = t.put_value(l);
					if (!rc.is_initialized())
					{
						std::stringstream sout;
						sout << "can't serialize [" << l;
						throw std::runtime_error(sout.str());
					}
					return rc.value();
				}), ",\n");
				ss << std::endl << "]";
				return ss.str();
			}
		};

		//////////////////////////////////////////////////////////////////////////
	}
}
