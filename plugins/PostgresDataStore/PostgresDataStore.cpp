#define POSTGRESDATASTOREPLUGIN_SOURCE 1

#include "Config.h"
#include "../Kernel.h"

#include <cstdlib>
#include <pqxx/pqxx>

namespace PluginEngine {

	// Postgres data store
	class PostgresDataStore : public DataStoreFactory::DataStore {

		// Creates a file data store
		public: POSTGRESDATASTOREPLUGIN_API PostgresDataStore()
		{
			_trackID = -1;
			_videoID = -1;
			init(_trackID, _videoID);
		}

		public: POSTGRESDATASTOREPLUGIN_API PostgresDataStore(
				std::map<std::string, std::string> args)
		{
			std::map<std::string, std::string>::iterator it;

			it = args.find("name");
			if (it != args.end()) {
				_name = std::string(it->second);
			} else
				_name = std::string("PostgresDataStore");

			it = args.find("process_directory");
			if (it != args.end()) {
				_processDir = std::string(it->second);
			} else
				_processDir = std::string(".");

			it = args.find("dbName");
			if (it != args.end()) {
				_dbName = it->second;
			} else
				_dbName = std::string("");

			it = args.find("dbUser");
			if (it != args.end()) {
				_dbUser = it->second;
			} else
				_dbUser = std::string("");

			it = args.find("dbUserPass");
			if (it != args.end()) {
				_dbUserPass = it->second;
			} else
				_dbUserPass = std::string("");

			it = args.find("dbHostAddr");
			if (it != args.end()) {
				_dbHostAddr = it->second;
			} else
				_dbHostAddr = std::string("");

			it = args.find("dbPort");
			if (it != args.end()) {
				_dbPort = it->second;
			} else
				_dbPort = std::string("");

			it = args.find("idTrack");
			if (it != args.end()) {
				_trackID = atoi(it->second.c_str());
			} else
				_trackID = -1;

			it = args.find("idVideo");
			if (it != args.end()) {
				_videoID = atoi(it->second.c_str());
			} else
				_videoID = -1;

			std::stringstream connString;
			connString << "dbname=" << _dbName << " user=" << _dbUser
					<< " password=" << _dbUserPass << " hostaddr="
					<< _dbHostAddr << " port=" << _dbPort;
			_connString = connString.str();

			init(_trackID, _videoID);
		}

		// Destroys a postgres data store
		public: POSTGRESDATASTOREPLUGIN_API virtual ~PostgresDataStore()
		{
			_conn->disconnect();
		}

		// Gets the name of the data store
		public: POSTGRESDATASTOREPLUGIN_API virtual const std::string &getName() const {
			return _name;
		}

		void init(int idT, int idV)
		{
			try {
				_conn = new pqxx::connection(_connString);
				if (_conn->is_open()) {
					std::cout << "Opened database successfully: "
							<< _conn->dbname() << std::endl;

					if (idT==-1 && idV==-1) {
						std::stringstream commIDT;
						commIDT << "SELECT id_track FROM public.trackvideo "
								<< "WHERE nome LIKE '" << getLeafDirName(_processDir) << "%'";
						_trackID = getInt(commIDT.str());

						std::stringstream commIDV;
						commIDV << "SELECT id FROM public.video "
								<< "WHERE nome LIKE '" << getLeafDirName(_processDir) << "%'";
						_videoID = getInt(commIDV.str());

						std::cout << "Initialized postgres store with track id: " << _trackID <<
								" and video id: " << _videoID << std::endl;
					} else {
						_trackID = idT;
						_videoID = idV;
					}
				} else {
					std::cout << "Can't open database" << std::endl;
				}
			} catch (const std::exception &e) {
				std::cerr << e.what() << std::endl;
			}
		}

		int getInt(std::string command)
		{
			int result = -1;

			pqxx::nontransaction nt(*_conn);
			pqxx::result rs(nt.exec(command));

			if (!rs.empty()) {
				pqxx::result::const_iterator c = rs.begin();
				result = c[0].as<int>();
			}

			return result;
		}

		std::string getLeafDirName(std::string fullPath)
		{
			int ini = fullPath.find_last_of("/");
			if (ini == (int)(fullPath.size()-1)) {
				ini = (fullPath.substr(0, fullPath.size()-1)).find_last_of( "/" );
			}

			std::string result = fullPath.substr(ini+1);

			return result;
		}

		std::string getFileName(std::string fullPath)
		{
			int ini = fullPath.find_last_of("/");
			std::string result = fullPath.substr(ini+1);

			return result;
		}

		POSTGRESDATASTOREPLUGIN_API std::map<std::string, std::vector<std::pair<Rect, std::string> > >
		getAnnotations(std::string processDir)
		{
			std::map<std::string, std::vector<std::pair<Rect, std::string> > > result;

			std::stringstream commMeta;
			commMeta << "SELECT frame,x,y,width,height,classe FROM public.trackframes "
					<< "WHERE id_track=" << _trackID;

			pqxx::nontransaction nt(*_conn);
			pqxx::result rs(nt.exec(commMeta));

			if (!rs.empty()) {
				std::cout << "Got " << rs.size() << " results." << std::endl;
				pqxx::result::const_iterator c = rs.begin();
				for ( ; c != rs.end(); c++) {
					std::map<std::string, std::vector<std::pair<Rect, std::string> > >::iterator it =
							result.find(c[0].as<std::string>());

					Rect object;
					object.x = c[1].as<int>(); object.y = c[2].as<int>();
					object.width = c[3].as<int>(); object.height = c[4].as<int>();
					if (it == result.end()) {
						std::vector<std::pair<Rect, std::string> > objs;
						objs.push_back(std::make_pair(object, c[5].as<std::string>()));

						result.insert(std::make_pair(c[0].as<std::string>(), objs));
					} else {
						it->second.push_back(std::make_pair(object, c[5].as<std::string>()));
					}
				}
			}

			return result;
		}

		POSTGRESDATASTOREPLUGIN_API void writeAnnotations(std::string name,
				std::string classe, std::vector<Rect> objs)
		{
			std::vector<Rect>::iterator wf = objs.begin();
			for ( ; wf != objs.end(); wf++) {
				std::stringstream ss;

				ss << "INSERT INTO public.trackframes(id_track,id_video,frame,x,y,width,height,classe) ";
				ss << "VALUES (" << _trackID << "," << _videoID << ",'" << getFileName(name) << "',";
				ss << wf->x << "," << wf->y << "," << wf->width << "," << wf->height << ",'" << classe << "');";
				try {
					pqxx::work trans(*_conn);

					trans.exec( ss );
					trans.commit();
				} catch (const std::exception &e) {
					std::cerr << e.what() << std::endl;
				}

			}
		}

		POSTGRESDATASTOREPLUGIN_API void writeAnnotations(std::string name,
				std::vector<std::pair<Rect, std::string > > objs)
		{
			std::vector<std::pair<Rect, std::string> >::iterator wf = objs.begin();
			for ( ; wf != objs.end(); wf++) {
				std::stringstream ss;

				ss << "INSERT INTO public.trackframes(id_track,id_video,frame,x,y,width,height,classe) ";
				ss << "VALUES (" << _trackID << "," << _videoID << ",'" << name << "',";
				ss << wf->first.x << "," << wf->first.y << ","
						<< wf->first.width << "," << wf->first.height << ",'" << wf->second << "');";
				try {
					pqxx::work trans(*_conn);

					trans.exec( ss );
					trans.commit();
				} catch (const std::exception &e) {
					std::cerr << e.what() << std::endl;
				}
			}
		}

		public:
			std::string _name;

			std::string _processDir;
			std::string _dbName;
			std::string _dbUser;
			std::string _dbUserPass;
			std::string _dbHostAddr;
			std::string _dbPort;
			std::string _connString;

			pqxx::connection* _conn;

			int _trackID;
			int _videoID;
	};

	// Retrieve the engine version we're going to expect
	extern "C" POSTGRESDATASTOREPLUGIN_API int getEngineVersion() {
		return 1;
	}

	// Register the plugin to an engine kernel
	extern "C" POSTGRESDATASTOREPLUGIN_API void registerPlugin(Kernel &kernel,
			std::map<std::string, std::string> &args)
	{
		kernel.getDataStoreFactory().addDataStore(
				std::auto_ptr<DataStoreFactory::DataStore>(new PostgresDataStore(args))
		);
	}

} // namespace PluginEngine
