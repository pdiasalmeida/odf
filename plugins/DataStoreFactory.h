#ifndef PLUGINENGINE_DATA_STORE_FACTORY_H
#define PLUGINENGINE_DATA_STORE_FACTORY_H

#include "Config.h"
#include "Common.h"

#include <vector>
#include <string>
#include <map>
#include <memory>
#include <stdexcept>

namespace PluginEngine {

  // ----------------------------------------------------------------------- //

  // Manages storage related stuff
  class DataStoreFactory {

  	  // Manages output from our object detector
  	  public: class DataStore {

  		  // Releases any resources owned by the store
      	  public: virtual ~DataStore() {}

      	  // Gets the name of the data store
      	  public: virtual const std::string &getName() const = 0;

      	  // Stores the information received
      	  virtual void writeAnnotations(
      			  std::string name,
				  std::string classe,
      			  std::vector<Rect> objs) = 0;
      	  virtual void writeAnnotations(
      			  std::string name,
      			  std::vector<std::pair<Rect, std::string> > objs) = 0;

      	  virtual std::map<std::string, std::vector<std::pair<Rect, std::string> > >
      	  getAnnotations(std::string processDir) = 0;
  	  };

  	  // A list of data stores
  	  private: typedef std::vector<DataStore *> DataStoreVector;

  	  // Destroys the data store factory
  	  public: PLUGINENGINE_API ~DataStoreFactory() {
  		  for (DataStoreVector::const_iterator it = this->dataStores.begin();
  				  it != this->dataStores.end();
  				  ++it)
  		  {
  			  delete *it;
  		  }
  	  }

  	  // Allows plugins to add new data stores
  	  public: PLUGINENGINE_API void addDataStore(
  			  std::auto_ptr<DataStore> dataStore) {
  		  this->dataStores.push_back(dataStore.release());
  	  }
    
  	  // Gets the total number of registered data stores
  	  public: PLUGINENGINE_API size_t getDataStoreCount() const {
  		  return this->dataStores.size();
  	  }

  	  // Accesses a dataStore by its name
  	  public: PLUGINENGINE_API DataStore &getDataStore(std::string name) {
  		  size_t index = 0;
  		  for (DataStoreVector::const_iterator it = this->dataStores.begin();
  				  it != this->dataStores.end();
  				  ++it, ++index)
  		  {
  			  if ((*it)->getName().compare(name)) {
  				  return *this->dataStores.at(index);
  			  }
  		  }
  		  throw std::runtime_error("No data store plugin registered for: "+name);
  	  }

        // All available data stores
    private: DataStoreVector dataStores;

  };

  // ----------------------------------------------------------------------- //

} // namespace PluginEngine

#endif // PLUGINENGINE_DATA_STORE_FACTORY_H
