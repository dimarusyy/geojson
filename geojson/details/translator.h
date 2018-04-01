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
#include <boost/mpl/map.hpp>
#include <boost/mpl/find.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/next_prior.hpp>

#include <sstream>

namespace geojson
{
	namespace details
	{
		//////////////////////////////////////////////////////////////////////////

		namespace qi = boost::spirit::qi;
		namespace phx = boost::phoenix;

		//////////////////////////////////////////////////////////////////////////

		template <typename T>
		using constructed_from_types = boost::mpl::map
			<
			boost::mpl::pair<typename point_t<T>, T>,
			boost::mpl::pair<typename line_t<T>, typename point_t<T>>,
			boost::mpl::pair<typename multipoint_t<T>, typename point_t<T>>,
			boost::mpl::pair<typename polygon_t<T>, typename line_t<T>>,
			boost::mpl::pair<typename multiline_t<T>, typename line_t<T>>,
			boost::mpl::pair<typename multipolygon_t<T>, typename polygon_t<T>>
			>;
		//////////////////////////////////////////////////////////////////////////


		template <typename Iterator, typename Skipper, typename Attribute>
		struct parser_t : qi::grammar<Iterator, Attribute(), Skipper>
		{
			using sub_type = typename boost::mpl::at<constructed_from_types<typename Attribute::base_type>, Attribute>::type;
			using sub_parser_t = parser_t<Iterator, Skipper, sub_type>;

			parser_t() : parser_t::base_type(_start)
			{
				_start =
					(qi::lit("[") >> (_sub_parser % qi::lit(",")) >> qi::lit("]"))
					[qi::_val = phx::construct<Attribute>(phx::begin(qi::_1), phx::end(qi::_1))];
				BOOST_SPIRIT_DEBUG_NODE(_start);
			}

		private:
			sub_parser_t _sub_parser;
			qi::rule<Iterator, Attribute(), Skipper> _start;
		};

		template <typename Iterator, typename Skipper, typename T>
		struct parser_t<Iterator, Skipper, point_t<T>> : qi::grammar<Iterator, point_t<T>(), Skipper>
		{
			parser_t() : parser_t::base_type(_start)
			{
				_start =
					(qi::lit("[") >> qi::double_ >> qi::lit(",") >> qi::double_ >> qi::lit("]"))
					[qi::_val = phx::construct<point_t<T>>(qi::_1, qi::_2)];
				BOOST_SPIRIT_DEBUG_NODE(_start);
			}

		private:
			qi::rule<Iterator, point_t<T>(), Skipper> _start;
		};

		//////////////////////////////////////////////////////////////////////////

		template <typename TExternal>
		struct translator_t
		{
			using internal_type = std::string;
			using external_type = TExternal;

			boost::optional<external_type> get_value(const internal_type& str)
			{
				external_type out_value{};
				auto s_it = std::begin(str), e_it = std::end(str);

				parser_t<internal_type::const_iterator, qi::ascii::space_type, external_type> g{};
				auto rc = qi::phrase_parse(s_it, e_it, g, qi::ascii::space, out_value);

				return boost::make_optional(rc && (s_it == e_it), out_value);
			}

			boost::optional<internal_type> put_value(const external_type& obj)
			{
				using sub_type = typename boost::mpl::at<constructed_from_types<typename TExternal::base_type>, external_type>::type;
				translator_t<sub_type> t;
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
				return boost::make_optional(ss.good(), ss.str());
			}
		};

		template <typename T>
		struct translator_t<point_t<T>>
		{
			using internal_type = std::string;
			using external_type = point_t<T>;

			boost::optional<external_type> get_value(const internal_type& str)
			{
				external_type out_value{};
				auto s_it = std::begin(str), e_it = std::end(str);
				parser_t<internal_type::const_iterator, qi::ascii::space_type, external_type> g{};
				auto rc = qi::phrase_parse(s_it, e_it, g, qi::ascii::space, out_value);
				return boost::make_optional(rc && (s_it == e_it), out_value);
			}

			boost::optional<internal_type> put_value(const external_type& obj)
			{
				std::stringstream ss;
				ss << "[" << obj._x << "," << obj._y << "]";
				return boost::make_optional(ss.good(), ss.str());
			}
		};


		// 		template<typename T, typename TExternal>
		// 		using translator_t = impl<T>::translator_t<TExternal>;

				//////////////////////////////////////////////////////////////////////////
	}
}
