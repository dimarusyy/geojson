#pragma once

//#include "details/position_clang.h"

#include <boost/mpl/vector.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/find.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/next_prior.hpp>

#include <vector>
#include <list>
#include <type_traits>

namespace geojson
{
	//////////////////////////////////////////////////////////////////////////

#if 1 

	template <typename T>
	struct primitives
	{
		struct point_t
		{
			using value_type = T;

			value_type _x;
			value_type _y;

			point_t() = default;

			point_t(const value_type& x, const value_type& y) : _x(x), _y(y)
			{
			}

			point_t(const std::pair<value_type, value_type>& p) : _x(p.first), _y(p.second)
			{
			}
		};

		using line_t = std::vector<point_t>;
		using multipoint_t = std::list<point_t>;

		using polygon_t = std::vector<line_t>;
		using multiline_t = std::list<line_t>;

		using multipolygon_t = std::list<polygon_t>;
	};

	template <typename T>
	using constructed_to_types = boost::mpl::map
	<
		boost::mpl::pair<primitives<T>::point_t, primitives<T>::multipoint_t>,
		boost::mpl::pair<primitives<T>::line_t, primitives<T>::multiline_t>,
		boost::mpl::pair<primitives<T>::polygon_t, primitives<T>::multipolygon_t>
	>;

	template <typename T>
	using constructed_from_types = boost::mpl::map
	<
		boost::mpl::pair<primitives<T>::point_t, T>,
		boost::mpl::pair<primitives<T>::line_t, primitives<T>::point_t>,
		boost::mpl::pair<primitives<T>::multipoint_t, primitives<T>::point_t>,
		boost::mpl::pair<primitives<T>::polygon_t, primitives<T>::line_t>,
		boost::mpl::pair<primitives<T>::multiline_t, primitives<T>::line_t>,
		boost::mpl::pair<primitives<T>::multipolygon_t, primitives<T>::polygon_t>
	>;

	// default
	template <typename T, typename THolder>
	struct position_t;

	// point_t
	template <typename T>
	struct position_t<T, typename primitives<T>::point_t>
	{
		using base_type = typename boost::mpl::at<constructed_from_types<T>, typename primitives<T>::point_t>::type;
		using value_type = typename primitives<T>::point_t;

		position_t(T x, T y)
			: _value(x, y)
		{
		}

		position_t(const std::pair<T, T>& il)
			: _value(il.first, il.second)
		{
		}

		value_type get() const
		{
			return _value;
		}

		operator value_type() const
		{
			return _value;
		}

		void dump() const
		{
			std::cout << "base_type=[" << boost::typeindex::type_id<base_type>().pretty_name() << "]" << std::endl;
			std::cout << "value_type=[" << boost::typeindex::type_id<value_type>().pretty_name() << "]" << std::endl;
		}

	private:
		value_type _value;
	};

	// line_t...
	template <typename T, typename THolder>
	struct position_t
	{
		using base_type = typename boost::mpl::at<constructed_from_types<T>, THolder>::type;
		using value_type = THolder;

		using position_iter_t = typename position_t<T, base_type>;
		position_t(std::initializer_list<position_iter_t> il, value_type holder = value_type{})
		{
			_value = value_type(std::begin(il), std::end(il));
		}

		value_type get() const
		{
			return _value;
		}

		operator value_type() const
		{
			return _value;
		}

		void dump() const
		{
			std::cout << "base_type=[" << boost::typeindex::type_id<base_type>().pretty_name() << "]" << std::endl;
			std::cout << "value_type=[" << boost::typeindex::type_id<value_type>().pretty_name() << "]" << std::endl;
			std::cout << "position_iter_t=[" << boost::typeindex::type_id<position_iter_t>().pretty_name() << "]" << std::endl;
		}

	private:
		value_type _value;
	};

	// user deduction guide
	template <typename T>
	position_t(T, T) -> position_t<T, primitives<T>::point_t>;

	template <typename T>
	position_t(const std::pair<T, T>&) -> position_t<T, primitives<T>::point_t>;

	template <typename T>
	position_t(std::initializer_list<position_t<T, primitives<T>::point_t>>) -> position_t<T, primitives<T>::line_t>;

	template <typename T>
	position_t(std::initializer_list<position_t<T, primitives<T>::line_t>>)->position_t<T, primitives<T>::multiline_t>;

#else

	////////////////////////////////////////////////////////////

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

	////////////////////////////////////////////////////////////

#endif /*__clang__ */
}
