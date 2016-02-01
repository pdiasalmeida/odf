#define FILEDATASTOREPLUGIN_SOURCE 1

#include "Config.h"
#include "../Kernel.h"
#include "FileWriter.hpp"

#include <sstream>

namespace PluginEngine {

	// File data store
	class FileDataStore : public DataStoreFactory::DataStore {

		// Destroys a file data store
		public: FILEDATASTOREPLUGIN_API virtual ~FileDataStore() {}

		// Gets the name of the data store
		public: FILEDATASTOREPLUGIN_API virtual const std::string &getName() const {
			static std::string sName("FileDataStore");
			return sName;
		}

		FILEDATASTOREPLUGIN_API std::map<std::string, std::vector<std::pair<Rect, std::string> > >
		getAnnotations( std::string processDir )
		{
			std::map<std::string, std::vector<std::pair<Rect, std::string> > > result;

			return result;
		}

		FILEDATASTOREPLUGIN_API void writeAnnotations(std::string name,
				std::string classe, std::vector<Rect> objs)
		{
			std::vector<Rect>::iterator wf = objs.begin();
			std::stringstream ss;

			if (objs.empty()){
				ss << name;
				ss << std::endl;
			}
			for ( ; wf != objs.end(); wf++ ) {
				ss << name << "," << classe << "," << wf->x << "," << wf->y
						<< "," << wf->width << "," << wf->height;

				ss << std::endl;
			}
			FileWriter::writeToFile( FileWriter::getLeafDirName(_processDir)+".dat", ss.str() );
			FileWriter::setFileContent( _outDir+"/"+FileWriter::getFileNameNoExt(name)+".dat", ss.str() );
		}

		FILEDATASTOREPLUGIN_API void writeAnnotations(std::string name,
				std::vector<std::pair<Rect, std::string > > objs)
		{
			std::vector<std::pair<Rect, std::string > >::iterator wf = objs.begin();
			std::stringstream ss;

			if (objs.empty())
				ss << name;
			for ( ; wf != objs.end(); wf++ ) {
				ss << name << "," << wf->first.x << "," << wf->first.y << ","
						<< wf->first.width << "," << wf->first.height << "," << wf->second;

				ss << std::endl;
			}

			FileWriter::writeToFile( FileWriter::getLeafDirName(_processDir)+".dat", ss.str() );
			FileWriter::setFileContent( _outDir+"/"+FileWriter::getFileNameNoExt(name)+".dat", ss.str() );
		}

		public:
			std::string _processDir = std::string(".");
			std::string _outDir = std::string(".");
	};

	// Retrieve the engine version we're going to expect
	extern "C" FILEDATASTOREPLUGIN_API int getEngineVersion() {
		return 1;
	}

	// Register the plugin to an engine kernel
	extern "C" FILEDATASTOREPLUGIN_API void registerPlugin(Kernel &kernel) {
		kernel.getDataStoreFactory().addDataStore(
				std::auto_ptr<DataStoreFactory::DataStore>(new FileDataStore())
		);
	}

} // namespace PluginEngine
