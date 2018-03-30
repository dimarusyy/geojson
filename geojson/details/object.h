#pragma once
#include "primitives.h"
#include "translator.h"

#include <boost/blank.hpp>
#include <boost/variant.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/fusion/container/map.hpp>
#include <boost/fusion/include/map.hpp>
#include <boost/property_tree/ptree.hpp>

namespace geojson
{
	namespace details
	{
		//////////////////////////////////////////////////////////////////////////

		template <typename T>
		struct object_t
		{
			using types = boost::mpl::vector<boost::blank, 
				point_t<T>, line_t<T>, polygon_t<T>, multipoint_t<T>, multiline_t<T>, multipolygon_t<T>>;
			using value_type = typename boost::make_variant_over<types>::type;

			template <typename T>
			object_t(T&& v)
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
 				boost::property_tree::ptree node;
				boost::apply_visitor(add_to_node_visitor{node}, _value);
				return node;
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
				static constexpr auto TYPE_KEY = "type";
				static constexpr auto COORDINATES_KEY = "coordinates";

				static const boost::fusion::map
				<
					boost::fusion::pair<point_t<T>, std::string>,
					boost::fusion::pair<line_t<T>, std::string>,
					boost::fusion::pair<polygon_t<T>, std::string>,
					boost::fusion::pair<multipoint_t<T>, std::string>,
					boost::fusion::pair<multiline_t<T>, std::string>,
					boost::fusion::pair<multipolygon_t<T>, std::string>
				>
				node_type_names
				{
					boost::fusion::make_pair<point_t<T>>("Point"),
					boost::fusion::make_pair<line_t<T>>("LineString"),
					boost::fusion::make_pair<polygon_t<T>>("Polygon"),
					boost::fusion::make_pair<multipoint_t<T>>("MultiPoint"),
					boost::fusion::make_pair<multiline_t<T>>("MultiLineString"),
					boost::fusion::make_pair<multipolygon_t<T>>("MultiPolygon"),
				};
				
				add_to_node_visitor(boost::property_tree::ptree& node)
					: _node(node)
				{
				}

				void operator()(const boost::blank& v) const 
				{
					throw std::runtime_error("can't add blank node");
				}

				template <typename Primitive>
				void operator()(const Primitive& v) const
				{
					details::translator_t<Primitive> t{};
					_node.put(TYPE_KEY, boost::fusion::at_key<Primitive>(node_type_names));
					_node.put(COORDINATES_KEY, t.put_value(v));
				}

				private:
					boost::property_tree::ptree& _node;
			};
		};

		//////////////////////////////////////////////////////////////////////////
	}
}
