#pragma once

#include <boost/variant.hpp>
#include <boost/blank.hpp>

#include <boost/mpl/vector.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/find.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/next_prior.hpp>

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

#if 0
		// default
		template <typename T, typename U>
		struct position_impl_t;

		// point_t
		template <typename T>
		struct position_impl_t<T, typename primitive_types<T>::point_t>
		{
			using value_type = typename primitive_types<T>::point_t;

			template <typename U1 = T, typename U2 = T>
			position_impl_t(U1 x, U2 y)
				: _value(x, y)
			{
			}

			template <typename U1 = T, typename U2 = T>
			position_impl_t(const std::pair<U1, U2> il,
				typename std::enable_if_t<std::is_arithmetic_v<U1> && std::is_arithmetic_v<U2>>* = 0)
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
			using position_point_t = typename position_impl_t<T, typename primitive_types<T>::point_t>;

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

		// 		position_t(std::pair<int, int>)->position_t<Point>;
		// 		position_t(const position_t<Point>&, const position_t<Point>&)->position_t<Line>;
#endif

		template <typename TPrimitive = boost::blank, typename T = double>
		struct position_t
		{
			using point_t = typename primitive_types<T>::point_t;
			using line_t = typename primitive_types<T>::line_t;
			using polygon_t = typename primitive_types<T>::polygon_t;

			using multipoint_t = typename primitive_types<T>::multipoint_t;
			using multiline_t = typename primitive_types<T>::multiline_t;
			using multipolygon_t = typename primitive_types<T>::multipolygon_t;

			using types = boost::mpl::vector<boost::blank, point_t, line_t, polygon_t, multipoint_t, multiline_t, multipolygon_t>;
			using variant_type = typename boost::make_variant_over<types>::type;

			using next_types = boost::mpl::map<
				boost::mpl::pair<point_t, multipoint_t>,
				boost::mpl::pair<line_t, multiline_t>,
				boost::mpl::pair<polygon_t, multipolygon_t>
			>;

			template <typename U>
			position_t(const U& v, 
				std::enable_if_t<std::is_constructible_v<point_t, U>>* = 0)
			{
				_value = point_t(std::move(v));
			}

			template <typename U1 = T, typename U2 = T>
			position_t(const U1& x, const U2& y, 
				std::enable_if_t<std::is_arithmetic_v<U1> && std::is_arithmetic_v<U2>>* = 0)
			{
				_value = point_t(x, y);
			}

			template <typename ...Args,
				typename = typename std::enable_if_t<std::conjunction_v<std::is_constructible<point_t, Args>...>>>
				position_t(Args...args)
			{
				std::initializer_list<point_t> points{ point_t(args)... };
				if constexpr(std::is_same_v<TPrimitive, boost::blank>)
				{
					using point_t_next_t = typename boost::mpl::at<next_types, point_t>::type;
					_value = point_t_next_t(points);
				}
				else
				{
					_value = TPrimitive(points);
				}
			}

			template <typename ...Args,
				typename = typename std::enable_if_t<std::conjunction_v<std::is_constructible<point_t, Args>...>>>
				position_t(Args...args)
			{
// 				std::initializer_list<point_t> points{ point_t(args)... };
// 				if constexpr(std::is_same_v<TPrimitive, boost::blank>)
// 				{
// 					using point_t_next_t = typename boost::mpl::at<next_types, point_t>::type;
// 					_value = point_t_next_t(points);
// 				}
// 				else
// 				{
// 					_value = TPrimitive(points);
// 				}
			}

			template <typename Default = TPrimitive>
			position_t(std::initializer_list<position_t<Default, T>> il)
			{
				if (is_of_type<point_t>(il))
				{
					construct<point_t, TPrimitive>(il);
				}
				else 
				if (is_of_type<line_t>(il))
				{
					construct<line_t, TPrimitive>(il);
				}
				else 
				if (is_of_type<polygon_t>(il))
				{
					construct<polygon_t, TPrimitive>(il);
				}
				else
				{
					throw std::runtime_error("position_t::position_t() - not initialized");
				}
			}

			template <typename U>
			bool is_of_type(typename variant_type v) const
			{
				const auto rc =	std::is_same_v<U, boost::blank> || 
								std::is_same_v<U, point_t> ||
								std::is_same_v<U, line_t> ||
								std::is_same_v<U, polygon_t> ||
								std::is_same_v<U, multipoint_t> ||
								std::is_same_v<U, multiline_t> ||
								std::is_same_v<U, multipolygon_t>;
				static_assert(rc, "position_t::is_of_type() - wrong parameter passed");
				return boost::apply_visitor(is_of_type_visitor<U>{}, v);
			}

			template <typename U>
			bool is_of_type() const
			{
				return is_of_type<U>(_value);
			}

			template <typename U, typename Default = TPrimitive>
			bool is_of_type(std::initializer_list<position_t<Default, T>> il) const
			{
				return std::accumulate(std::begin(il), std::end(il),
					true, [&](bool b, const auto& v) { return b && is_of_type<U>(v.get()); });
			}

			typename variant_type get() const
			{
				return _value;
			}

			template <typename U>
			operator U() const
			{
				return boost::get<U>(_value);
			}

		protected:
			template <typename Original, typename Result, typename Default = TPrimitive>
			void construct(const std::initializer_list<position_t<Default, T>>& il)
			{
				if constexpr(std::is_same_v<Result, boost::blank>)
				{
					static_assert(boost::mpl::has_key<next_types, Original>::value, "position_t::construct() - can't deduce resulting type");

					using point_t_next_t = typename boost::mpl::at<next_types, Original>::type;
					point_t_next_t l{};
					std::for_each(std::begin(il), std::end(il), [&](const auto& v) 
					{ 
						l.push_back(v); 
					});
					_value = l;
				}
				else
				{
					Result l{};
					std::for_each(std::begin(il), std::end(il), [&](const auto& v) 
					{ 
						l.push_back(v); 
					});
					_value = l;
				}
			}

		private:
			typename variant_type _value{ boost::blank{} };

			template <typename TTrue>
			struct is_of_type_visitor : boost::static_visitor<bool>
			{
				bool operator()(const TTrue& v)  const
				{
					return true;
				}

				template <typename TFalse>
				bool operator()(const TFalse& v) const
				{
					return false;
				}
			};
		};

		//////////////////////////////////////////////////////////////////////////
	}
}
