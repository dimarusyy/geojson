#pragma once
#include "object.h"
#include <boost/property_tree/ptree.hpp>

namespace geojson
{
	namespace details
	{
		//////////////////////////////////////////////////////////////////////////

		template <typename T>
		struct feature_t
		{
			static constexpr auto TYPE_KEY = "type";
			static constexpr auto TYPE_VALUE = "Feature";
			static constexpr auto GEOMETRY_KEY = "geometry";
			static constexpr auto PROPERTIES_KEY = "properties";

			feature_t(object_t<T>&& obj)
				: _geometry(std::move(obj))
			{
			}

			template <typename T>
			void add_property(const std::string& key, const T& value)
			{
				_properties.add_child(key, value);
			}

			operator boost::property_tree::ptree()
			{
				boost::property_tree::ptree node;
				node.add_child(TYPE_KEY, TYPE_VALUE);
				node.add_child(GEOMETRY_KEY, _geometry);
				node.add_child(PROPERTIES_KEY, _properties);
				return node;
			}



		private:
			object_t<T> _geometry;
			boost::property_tree::ptree _properties;
		};

		//////////////////////////////////////////////////////////////////////////
	}
}

