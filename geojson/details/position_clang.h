#pragma once

#include <boost/variant.hpp>
#include <boost/blank.hpp>

#include <boost/mpl/vector.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/find.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/next_prior.hpp>

#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/sequence/intrinsic/at_c.hpp>
#include <boost/fusion/include/at_c.hpp>

#include <vector>
#include <list>
#include <numeric>
#include <type_traits>

namespace geojson
{
	namespace details
	{
		//////////////////////////////////////////////////////////////////////////

		namespace primitives
		{
			template <typename T>
			struct point_t
			{
				T _x;
				T _y;

				point_t() = default;

				template <typename U1 = T, typename U2 = T>
				point_t(const U1& x, const U2& y) : _x(x), _y(y)
				{
				}

				template <typename U>
				point_t(std::initializer_list<U> il)
				{
					if constexpr (std::is_arithmetic_v<U>)
					{
						if (il.size() == 2)
						{
							_x = *(std::begin(il));
							_y = *(std::begin(il) + 1);
						}
						else
						{
							throw std::runtime_error("point_t::point_t() - initialization failure");
						}
					}
					else
					{
						static_assert(false, "point_t::point_t() - initialization failure");
					}
				}
			};
		}

		template <typename T>
		struct primitive_types
		{
			using point_t = primitives::point_t<T>;

			using line_t = std::vector<point_t>;
			using multipoint_t = std::list<point_t>;

			using polygon_t = std::vector<line_t>;
			using multiline_t = std::list<line_t>;

			using multipolygon_t = std::list<polygon_t>;
		};

		// default
		template <typename T, typename U>
		struct position_impl_t;

		// point_t
		template <template T>//, typename = std::enable_if_t<std::is_arithmetic_v<T>*/>
		struct position_impl_t<T, typename primitive_types<T>::point_t>
		{
			using point_t = typename primitive_types<T>::point_t;
			using value_type = typename primitive_types<T>::point_t;

			position_impl_t(T x, double y)
				: _value(x, y)
			{
			}

			position_impl_t(const std::pair<double, double>& il)
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

		private:
			value_type _value;
		};

		// line_t
		template <typename T>
		struct position_impl_t<T, typename primitive_types<T>::line_t>
		{
			using point_t = typename primitive_types<T>::point_t;
			using value_type = typename primitive_types<T>::line_t;
			using position_point_t = typename position_impl_t<typename primitive_types<T>::point_t>;

			position_impl_t(std::initializer_list<position_point_t> il)
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

		private:
			value_type _value;
		};

		position_impl_t(double, double) -> position_impl_t<primitive_types<T>::point_t>;
		position_impl_t(std::pair<double, double>) -> position_impl_t<primitive_types<T>::point_t>;

		//////////////////////////////////////////////////////////////////////////
	}
}
