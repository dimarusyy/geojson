#pragma once

#include <boost/algorithm/string.hpp>
#include <boost/range/adaptors.hpp>

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
			using base_type = T;
			using value_type = T;
			using container_type = T;
			
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
			using base_type = T;
			using value_type = std::vector<point_t<T>>;
			using container_type = std::vector<typename point_t<T>::container_type>;
			
			line_t() = default;

			template <typename Iterator>
			line_t(Iterator b_it, Iterator e_it/*, typename std::enable_if_t<!std::is_constructible_v<point_t<T>, Iterator>>* = 0*/)
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
				os << boost::algorithm::join(obj.get() | boost::adaptors::transformed([&](const auto& p)
				{
					std::stringstream ss;
					ss << p;
					return ss.str();
				}), ",");
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
			using base_type = T;
			using value_type = std::vector<line_t<T>>;
			using container_type = std::vector<typename line_t<T>::container_type>;

			polygon_t() = default;

			template <typename Iterator>
			polygon_t(Iterator b_it, Iterator e_it/*, typename std::enable_if_t<!std::is_constructible_v<line_t<T>, Iterator>>* = 0*/)
			{
				_value = value_type(b_it, e_it);
				for (const auto& l : _value) is_ring(l);
			}

			polygon_t(std::initializer_list<line_t<T>> il)
			{
				_value = value_type(std::begin(il), std::end(il));
				for (const auto& l : _value) is_ring(l);
			}

			value_type get() const
			{
				return _value;
			}

			friend std::ostream &operator<<(std::ostream &os, const polygon_t<T> &obj)
			{
				os << "{";
				os << boost::algorithm::join(obj.get() | boost::adaptors::transformed([&](const auto& p)
				{
					std::stringstream ss;
					ss << p;
					return ss.str();
				}), ",");
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
				if (l.get().front() != l.get().back())
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
			using base_type = T;
			using value_type = Container;
			using container_type = std::vector<typename U::container_type>;

			multiobject_t() = default;

			template <typename Iterator>
			multiobject_t(Iterator b_it, Iterator e_it)
			{
				_value = value_type(b_it, e_it);
			}

			multiobject_t(std::initializer_list<U> il)
			{
				_value = value_type(std::begin(il), std::end(il));
			}

			value_type get() const
			{
				return _value;
			}

			friend std::ostream &operator<<(std::ostream &os, const multiobject_t<T, U, Container> &obj)
			{
				os << "{";
				os << boost::algorithm::join(obj.get() | boost::adaptors::transformed([&](const auto& p)
				{
					std::stringstream ss;
					ss << p;
					return ss.str();
				}), ",");
				os << "}";
				return os;
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