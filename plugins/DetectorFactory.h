#ifndef PLUGINENGINE_DETECTOR_FACTORY_H
#define PLUGINENGINE_DETECTOR_FACTORY_H

#include "Config.h"

#include <vector>
#include <string>
#include <map>
#include <memory>
#include <stdexcept>

namespace PluginEngine {

  // ----------------------------------------------------------------------- //

  // Manages object detection implementations
  class DetectorFactory {

  	  // Outputs detections in an image
  	  public: class Detector {

  	  	  // Releases any resources owned by the detector
  	  	  public: virtual ~Detector() {}

  	  	  // Gets the name of the detector
  	  	  public: virtual const std::string &getName() const = 0;

      	  // Detects objects in an image
      	  public: virtual std::vector<Rect>
      	  detect(byte *imageData, int width, int height, int channels) = 0;
  	  };

  	  // A list of detectors
  	  private: typedef std::vector<Detector *> DetectorVector;

  	  // Destroys the detectors
  	  public: PLUGINENGINE_API ~DetectorFactory() {
  		  for (DetectorVector::const_iterator it = this->detectors.begin();
  				  it != this->detectors.end();
  				  ++it)
  		  {
  			  delete *it;
  		  }
  	  }

  	  // Allows plugins to add new detectors
  	  public: PLUGINENGINE_API void addDetector(
  			  std::auto_ptr<Detector> dataStore) {
  		  this->detectors.push_back(dataStore.release());
  	  }
    
  	  // Gets the total number of registered data stores
  	  public: PLUGINENGINE_API size_t getDetectorCount() const {
  		  return this->detectors.size();
  	  }

  	  // Accesses a detector by its name
  	  public: PLUGINENGINE_API Detector &getDetector(std::string name) {
  		  size_t index = 0;
  		  for (DetectorVector::const_iterator it = this->detectors.begin();
  				  it != this->detectors.end();
  				  ++it, ++index) {
  			  if ((*it)->getName().compare(name)) {
  				  return *this->detectors.at(index);
  			  }
  		  }
  		  throw std::runtime_error("No data store plugin registered for: "+name);
  	  }

        // All available detectors
    private: DetectorVector detectors;

  };

  // ----------------------------------------------------------------------- //

} // namespace PluginEngine

#endif // PLUGINENGINE_DETECTOR_FACTORY_H
