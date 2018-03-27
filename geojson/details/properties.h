#pragma once
#include <boost/property_tree/ptree.hpp>

namespace geojson
{
	namespace details
	{
		//////////////////////////////////////////////////////////////////////////

		template <typename T>
		struct properties_t
		{
			properties_t() = default;

			template <typename T>
			void add_property(const std::string& key, T&& value)
			{
				_props.add(key, std::forward<T>(value));
			}

		private:
			boost::property_tree::ptree _props;
		};
		
		//////////////////////////////////////////////////////////////////////////
	}
}
