#pragma once
#include "primitives.h"

#include <boost/blank.hpp>
#include <boost/variant.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/property_tree/ptree.hpp>

namespace geojson
{
	namespace details
	{
		//////////////////////////////////////////////////////////////////////////

		template <typename T>
		struct geometry_t
		{
			using types = boost::mpl::vector<boost::blank, 
				point_t<T>, line_t<T>, polygon_t<T>, multipoint_t<T>, multiline_t<T>, multipolygon_t<T>>;
			using value_type = typename boost::make_variant_over<types>::type;

			template <typename T>
			geometry_t(T&& v)
				: _value(std::forward<T>(v))
			{
			}

			value_type get() const
			{
				return _value;
			}

			template <typename U>
			bool is_of_type() const
			{
				return is_of_type<U>(_value);
			}

			operator boost::property_tree::ptree()
			{
				constexpr auto GEOMETRY_KEY = "geometry";

				boost::property_tree::ptree node;
				node.add_child(TYPE_KEY, )
			}


		private:
			value_type _value{ boost::blank };

			template <typename U>
			bool is_of_type(typename value_type v) const
			{
				const auto rc =
					std::is_same_v<U, boost::blank> ||
					std::is_same_v<U, point_t<T>> ||
					std::is_same_v<U, line_t<T>> ||
					std::is_same_v<U, polygon_t<T>> ||
					std::is_same_v<U, multipoint_t<T>> ||
					std::is_same_v<U, multiline_t<T>> ||
					std::is_same_v<U, multipolygon_t<T>>;
				static_assert(rc, "wrong parameter passed");
				return boost::apply_visitor(is_of_type_visitor<U>{}, v);
			}

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

			struct add_to_node_visitor : boost::static_visitor<boost::property_tree::ptree>
			{
				constexpr auto TYPE_KEY = "type";
				constexpr auto COORDINATES_KEY = "coordinates";

				void operator()(const boost::blank& v) const 
				{
					boost::property_tree::ptree node;
					node.put(TYPE_KEY, "blank");
					node.put(COORDINATES_KEY, nullptr);
					return node;
				}

				void operator()(const point_t<T>& v) const
				{
					boost::property_tree::ptree node;
					node.put(TYPE_KEY, "Point");
					node.put(COORDINATES_KEY, std::vector<T>{v._x, v._y});
					return node;
				}

				void operator()(const line_t<T>& v) const
				{
					boost::property_tree::ptree node;
					node.put(TYPE_KEY, "Point");
					node.put(COORDINATES_KEY, std::vector<T>{v._x, v._y});
					return node;
				}

				void operator()(const polygon_t<T>& v) const
				{
				}

				void operator()(const multipoint_t<T>& v) const
				{
				}

				void operator()(const multiline_t<T>& v) const
				{
				}

				void operator()(const multipolygon_t<T>& v) const
				{
				}
			};
		};

		//////////////////////////////////////////////////////////////////////////
	}
}
