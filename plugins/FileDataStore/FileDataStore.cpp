#define FILEDATASTOREPLUGIN_SOURCE 1

#include "Config.h"
#include "../Kernel.h"
#include "FileWriter.hpp"

#include <sstream>
#include <iostream>

namespace PluginEngine {

	// File data store
	class FileDataStore : public DataStoreFactory::DataStore {

		// Creates a file data store
		public: FILEDATASTOREPLUGIN_API FileDataStore()
		{
			_name = std::string("FileDataStore");

			_processDir = std::string(".");
			_outDir = std::string(".");

			FileWriter::openFile(_outDir, FileWriter::getLeafDirName(_processDir)+".out");
		}

		public: FILEDATASTOREPLUGIN_API FileDataStore(std::map<std::string, std::string> args)
		{
			std::map<std::string, std::string>::iterator it;

			it = args.find("name");
			if (it != args.end()) {
				_name = std::string(it->second);
			} else
				_name = std::string("FileDataStore");

			it = args.find("process_directory");
			if (it != args.end()) {
				_processDir = std::string(it->second);
			} else
				_processDir = std::string(".");

			it = args.find("output_directory");
			if (it != args.end()) {
				_outDir = it->second;
			} else
				_outDir = std::string(".");

			FileWriter::openFile(_outDir, FileWriter::getLeafDirName(_processDir)+".out");
		}

		// Destroys a file data store
		public: FILEDATASTOREPLUGIN_API virtual ~FileDataStore()
		{
			//FileWriter::close();
		}

		// Gets the name of the data store
		public: FILEDATASTOREPLUGIN_API virtual const std::string &getName() const {
			return _name;
		}

		FILEDATASTOREPLUGIN_API std::map<std::string, std::vector<std::pair<Rect, std::string> > >
		getAnnotations(std::string processDir)
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
			for ( ; wf != objs.end(); wf++) {
				ss << name << "," << classe << "," << wf->x << "," << wf->y
						<< "," << wf->width << "," << wf->height;

				ss << std::endl;
			}
			FileWriter::writeToFile(FileWriter::getLeafDirName(_processDir)+".out", ss.str());
			FileWriter::setFileContent(_outDir+"/"+FileWriter::getFileNameNoExt(name)+".dat", ss.str());
		}

		FILEDATASTOREPLUGIN_API void writeAnnotations(std::string name,
				std::vector<std::pair<Rect, std::string > > objs)
		{
			std::vector<std::pair<Rect, std::string > >::iterator wf = objs.begin();
			std::stringstream ss;

			if (objs.empty())
				ss << name;
			for ( ; wf != objs.end(); wf++) {
				ss << name << "," << wf->first.x << "," << wf->first.y << ","
						<< wf->first.width << "," << wf->first.height << "," << wf->second;

				ss << std::endl;
			}

			FileWriter::writeToFile(FileWriter::getLeafDirName(_processDir)+".out", ss.str());
			FileWriter::setFileContent(_outDir+"/"+FileWriter::getFileNameNoExt(name)+".dat", ss.str());
		}

		public:
			std::string _name;

			std::string _processDir;
			std::string _outDir;
	};

	// Retrieve the engine version we're going to expect
	extern "C" FILEDATASTOREPLUGIN_API int getEngineVersion() {
		return 1;
	}

	// Register the plugin to an engine kernel
	extern "C" FILEDATASTOREPLUGIN_API void registerPlugin(Kernel &kernel,
			std::map<std::string, std::string> &args)
	{
		kernel.getDataStoreFactory().addDataStore(
				std::auto_ptr<DataStoreFactory::DataStore>(new FileDataStore(args))
		);
	}

} // namespace PluginEngine
