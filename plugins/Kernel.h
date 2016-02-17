#ifndef PLUGINENGINE_KERNEL_H
#define PLUGINENGINE_KERNEL_H

#include "Config.h"
#include "DataStoreFactory.h"
#include "DetectorFactory.h"
#include "Plugin.h"

#include <string>
#include <map>
#include <iostream>

namespace PluginEngine {

	// The engine's core
	class Kernel {

		// Map of plugins by their associated file names
		typedef std::map< std::string, Plugin > PluginMap;

		// Accesses the data store factory
		public: PLUGINENGINE_API DataStoreFactory &getDataStoreFactory() {
			return this->dataStoreFactory;
		}

		// Accesses the detector factory
		public: PLUGINENGINE_API DetectorFactory &getDetectorFactory() {
			return this->detectorFactory;
		}

		// Loads a plugin by its name
		public: PLUGINENGINE_API void loadPlugin(const std::string &filename,
				std::map<std::string, std::string> &args)
		{
			if (this->loadedPlugins.find(filename) == this->loadedPlugins.end()) {
				this->loadedPlugins.insert(
						PluginMap::value_type(filename, Plugin(filename))
				).first->second.registerPlugin(*this, args);
			}
		}

		// All plugins currently loaded
		private: PluginMap loadedPlugins;
		// Manages storage-related tasks for the engine
		private: DataStoreFactory dataStoreFactory;
		// Manages detection-related tasks for the engine
		private: DetectorFactory detectorFactory;
	};

} // namespace PluginEngine

#endif // PLUGINENGINE_KERNEL_H
