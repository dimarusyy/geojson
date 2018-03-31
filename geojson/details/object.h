#pragma once
#include "primitives.h"
#include "translator.h"

#include <boost/blank.hpp>
#include <boost/variant.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/fusion/container/map.hpp>
#include <boost/fusion/include/map.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

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

			object_t(const boost::property_tree::ptree& root)
			{
				auto root_type = t.get_child<std::string>(TYPE_KEY);
				if (root_type == get_type_name<point_t<T>>())
				{
					construct_impl<point_t<T>>();
				}
				else if (root_type == get_type_name<line_t<T>>())
				{
					construct_impl<line_t<T>>();
				}
				else if (root_type == get_type_name<polygon_t<T>>())
				{
					construct_impl<polygon_t<T>>();
				}
				else if (root_type == get_type_name<multipoint_t<T>>())
				{
					construct_impl<multipoint_t<T>>();
				}
				else if (root_type == get_type_name<multiline_t<T>>())
				{
					construct_impl<multiline_t<T>>();
				}
				else if (root_type == get_type_name<multipolygon_t<T>>())
				{
					construct_impl<multipolygon_t<T>>();
				}
				else
				{
					throw std::runtime_error("can't create object_t from ptree");
				}
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
				return boost::apply_visitor(add_to_node_visitor{}, _value);
			}

			friend std::ostream& operator<< (std::ostream& os, object_t<T>& obj)
			{
				boost::property_tree::write_json(os, obj.operator boost::property_tree::ptree());
				return os;
			}

			friend std::istream& operator>> (std::istream& is, object_t<T>& obj)
			{
				boost::property_tree::ptree t;
				boost::property_tree::read_json(os, t);
				return is;
			}
		
		protected:
			template <typename T>
			void construct_impl(const boost::property_tree::ptree& root)
			{
				impl::translator_t<T> tr{};
				_value = root.get<T>(COORDINATES_KEY, tr);
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

				add_to_node_visitor()
				{
				}

				boost::property_tree::ptree operator()(const boost::blank& v) const
				{
					throw std::runtime_error("can't add blank node");
				}

				template <typename Primitive>
				boost::property_tree::ptree operator()(const Primitive& v) const
				{
					boost::property_tree::ptree node;
					details::translator_t<T, Primitive> t{};
					node.put(TYPE_KEY, get_type_name<Primitive>());
					node.put(COORDINATES_KEY, t.put_value(v));
					return node;
				}

			private:
				using object_type_names_t = boost::fusion::map
				<
					boost::fusion::pair<point_t<T>, std::string>,
					boost::fusion::pair<line_t<T>, std::string>,
					boost::fusion::pair<polygon_t<T>, std::string>,
					boost::fusion::pair<multipoint_t<T>, std::string>,
					boost::fusion::pair<multiline_t<T>, std::string>,
					boost::fusion::pair<multipolygon_t<T>, std::string>
				>;

				template <typename Primitive>
				std::string get_type_name() const
				{
					static object_type_names_t _object_type_names
					{
						boost::fusion::make_pair<point_t<T>>("Point"),
						boost::fusion::make_pair<line_t<T>>("LineString"),
						boost::fusion::make_pair<polygon_t<T>>("Polygon"),
						boost::fusion::make_pair<multipoint_t<T>>("MultiPoint"),
						boost::fusion::make_pair<multiline_t<T>>("MultiLineString"),
						boost::fusion::make_pair<multipolygon_t<T>>("MultiPolygon"),
					};
					return boost::fusion::at_key<Primitive>(_object_type_names);
				}
			};
		};

		//////////////////////////////////////////////////////////////////////////
	}
}
