#ifndef PLUGINENGINE_PLUGIN_H
#define PLUGINENGINE_PLUGIN_H

#include "Config.h"
#include "SharedLibrary.h"

#include <string>
#include <map>
#include <iostream>

namespace PluginEngine {
	class Kernel;

	// Representation of a plugin
	class Plugin {

		// Signature for the version query function
		private: typedef int GetEngineVersionFunction();
		// Signature for the plugin's registration function
		private: typedef void RegisterPluginFunction(Kernel &,
				std::map<std::string, std::string> &args);

		// Initializes and loads a plugin
		public: PLUGINENGINE_API Plugin(const std::string &filename);
		// Copies an existing plugin instance
		public: PLUGINENGINE_API Plugin(const Plugin &other);
		// Unloads the plugin
		public: PLUGINENGINE_API ~Plugin();

		// Queries the plugin for its expected engine version
		public: PLUGINENGINE_API int getEngineVersion() const {
			return this->getEngineVersionAddress();
		}

		// Register the plugin to a kernel
		public: PLUGINENGINE_API void registerPlugin(Kernel &kernel,
				std::map<std::string, std::string> &args)
		{
			this->registerPluginAddress(kernel, args);
		}

		// Creates a copy of the plugin instance
		public: Plugin &operator =(const Plugin &other);

		// Handle of the loaded shared library
		private: SharedLibrary::HandleType sharedLibraryHandle;
		// Number of references that exist to the shared library
		private: size_t *referenceCount;
		// Function to query for the expected engine version
		private: GetEngineVersionFunction *getEngineVersionAddress;
		// Registers the plugin with the kernel
		private: RegisterPluginFunction *registerPluginAddress;

	};
// ----------------------------------------------------------------------- //
} // namespace PluginEngine

#endif // PLUGINENGINE_PLUGIN_H
