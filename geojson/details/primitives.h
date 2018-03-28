#pragma once

#include <iostream>
#include <vector>
#include <type_traits>
#include <exception>

namespace geojson
{
	//////////////////////////////////////////////////////////////////////////

	namespace details
	{
		template <typename T>
		struct point_t
		{
			using value_type = typename T;
			
			value_type _x;
			value_type _y;

			point_t() = default;

			template <typename U1 = value_type, typename U2 = value_type>
			point_t(const U1& x, const U2& y) : _x(x), _y(y)
			{
			}

			template <typename U1 = value_type, typename U2 = value_type>
			point_t(const std::pair<U1, U2>& il) : _x(il.first), _y(il.second)
			{
			}

			bool operator==(const point_t<T>& other) const
			{
				return _x == other._x && _y == other._y;
			}

			bool operator!=(const point_t<T>& other) const
			{
				return !(*this == other);
			}

			friend std::ostream &operator<<(std::ostream &os, const point_t<T> &obj)
			{
				os << "{" << obj._x << "," << obj._y << "}";
				return os;
			}
		};

		template <typename T>
		struct line_t
		{
			using value_type = std::vector<point_t<T>>;
			
			line_t() = default;

			template <typename...Args>
			line_t(Args...args)
			{
				static_assert(std::conjunction_v<std::is_constructible<point_t<T>, Args>...>, __FUNCTION__);	
				static_assert(sizeof...(Args) > 1, "expected more then 1 argument");

				std::initializer_list<point_t<T>> il{ args... };
				_value = value_type(std::begin(il), std::end(il));
			}

			template <typename Iterator>
			line_t(Iterator b_it, Iterator e_it, typename std::enable_if_t<!std::is_constructible_v<point_t<T>, Iterator>>* = 0)
			{
				_value = value_type(b_it, e_it);
			}

			line_t(std::initializer_list<point_t<T>> il)
			{
				_value = value_type(std::begin(il), std::end(il));
			}

			value_type get() const
			{
				return _value;
			}

			friend std::ostream &operator<<(std::ostream &os, const line_t<T> &obj)
			{
				os << "{";
				for (const auto& el : obj.get())
					os << "{" << el << "}" << ",";
				os << "}";
				return os;
			}
		private:
			value_type _value;
		};

		template <typename T>
		struct polygon_t
		{
			static const size_t POLYGON_MIN_ELEMENTS = 4U;
			using value_type = std::vector<line_t<T>>;

			template <typename...Args>
			polygon_t(Args...args)
			{
				static_assert(std::conjunction_v<std::is_constructible<line_t<T>, Args>...>, __FUNCTION__);

				std::initializer_list<line_t<T>> il{ args... };
				_value = value_type(std::begin(il), std::end(il));

				for (const auto& le : _value) is_ring(le);
			}

			template <typename Iterator>
			polygon_t(Iterator b_it, Iterator e_it, typename std::enable_if_t<!std::is_constructible_v<line_t<T>, Iterator>>* = 0)
			{
				_value = value_type(b_it, e_it);
			}

			polygon_t(std::initializer_list<line_t<T>> il)
			{
				_value = value_type(std::begin(il), std::end(il));
			}

			value_type get() const
			{
				return _value;
			}

			friend std::ostream &operator<<(std::ostream &os, const polygon_t<T> &obj)
			{
				os << "{";
				for (const auto& el : obj.get())
					os << "{" << el << "}" << ",";
				os << "}";
				return os;
			}

		private:
			bool is_ring(const line_t<T>& l) const
			{
				if (l.get().size() < POLYGON_MIN_ELEMENTS)
				{
					throw std::runtime_error("is_ring() : expected [" + std::to_string(POLYGON_MIN_ELEMENTS) 
						+ "] elements, got [" + std::to_string(l.get().size()) + "]");
				}
				if (*std::begin(l.get()) != *std::end(l.get()))
				{
					throw std::runtime_error("is_ring() : first and last elements should be the same");
				}
				return true;
			}

			value_type _value;
		};

		//////////////////////////////////////////////////////////////////////////

		template <typename T, typename U, typename Container = std::vector<U>>
		struct multiobject_t
		{
			using value_type = Container;

			template <typename...Args>
			multiobject_t(Args...args)
			{
				static_assert(std::conjunction_v<std::is_constructible<U, Args>...>, __FUNCTION__);

				std::initializer_list<U> il{ args... };
				_value = value_type(args...);
			}

			multiobject_t(std::initializer_list<U> il)
			{
				_value = value_type(std::begin(il), std::end(il));
			}

			value_type get() const
			{
				return _value;
			}

		private:
			value_type _value;
		};

		template <typename T = double>
		using multipoint_t = multiobject_t<T, point_t<T>>;

		template <typename T = double>
		using multiline_t = multiobject_t<T, line_t<T>>;

		template <typename T = double>
		using multipolygon_t = multiobject_t<T, polygon_t<T>>;

		//////////////////////////////////////////////////////////////////////////
	}
	
	//////////////////////////////////////////////////////////////////////////
}