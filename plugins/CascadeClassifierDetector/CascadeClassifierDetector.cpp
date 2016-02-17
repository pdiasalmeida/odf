#define CASCADECLASSIFIERDETECTORPLUGIN_SOURCE 1

#include "Config.h"
#include "../Kernel.h"

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>

namespace PluginEngine {

	// Detector
	class CascadeClassifierDetector : public DetectorFactory::Detector {
		public:
			double _scale;
			int _minNeighbours;
			cv::Size _minSize;
			cv::Size _maxSize;

			std::string _path;
			cv::CascadeClassifier cascade;

		// Creates the detector
		public: CASCADECLASSIFIERDETECTORPLUGIN_API CascadeClassifierDetector()
		{
			_scale = 1.04;
			_minNeighbours = 3;
			_minSize = cv::Size(35, 15);
			_maxSize = cv::Size(350, 150);

			_path = std::string("./cascade.xml");

			init();
		}

		public: CASCADECLASSIFIERDETECTORPLUGIN_API CascadeClassifierDetector(
				std::map<std::string, std::string> args)
		{
			std::map<std::string, std::string>::iterator it;

			it = args.find("scale");
			if (it != args.end()) {
				_scale = atof(it->second.c_str());
			} else
				_scale = 1.04;

			it = args.find("minNeighbours");
			if (it != args.end()) {
				_minNeighbours = atoi(it->second.c_str());
			} else
				_minNeighbours = 3;

			it = args.find("minSizeWidth");
			if (it != args.end()) {
				int minSizeWidth = atoi(it->second.c_str());
				it = args.find("minSizeHeight");
				if (it != args.end()) {
					int minSizeHeight = atoi(it->second.c_str());
					_minSize = cv::Size(minSizeWidth, minSizeHeight);
				}
			} else
				_minSize = cv::Size(35, 15);

			it = args.find("maxSizeWidth");
			if (it != args.end()) {
				int maxSizeWidth = atoi(it->second.c_str());
				it = args.find("maxSizeHeight");
				if (it != args.end()) {
					int maxSizeHeight = atoi(it->second.c_str());
					_maxSize = cv::Size(maxSizeWidth, maxSizeHeight);
				}
			} else
				_maxSize = cv::Size(350, 150);

			it = args.find("filePath");
			if (it != args.end()) {
				_path = std::string(it->second);
			} else
				_path = std::string("./cascade.xml");

			init();
		}

		// Destroys the detector
		public: CASCADECLASSIFIERDETECTORPLUGIN_API virtual ~CascadeClassifierDetector() {}

		// Gets the name of the detector
		public: CASCADECLASSIFIERDETECTORPLUGIN_API virtual const std::string &getName() const {
			static std::string sName("CascadeClassifierDetector");
			return sName;
		}

		CASCADECLASSIFIERDETECTORPLUGIN_API void init()
		{
			if (!cascade.load(_path)) {
				std::cout << "--(!) Error loading cascade " + _path
						<< "Check if file exists in path." << std::endl;
			};
		}

		CASCADECLASSIFIERDETECTORPLUGIN_API cv::Mat bytesToMat(byte *bytes,
				int width, int height, int channelType)
		{
		    cv::Mat image = cv::Mat(height, width, channelType, bytes).clone();
		    return image;
		}

		CASCADECLASSIFIERDETECTORPLUGIN_API std::vector<Rect>
		detect(byte *imageData, int width, int height, int channelType)
		{
			std::vector<cv::Rect> objects;
			std::vector<Rect> resultObjects;
			cv::Mat frame = bytesToMat(imageData, width, height, channelType);
			cv::Mat frame_gray;
			switch (channelType) {
				case CV_8UC3:
					cv::cvtColor( frame, frame_gray, cv::COLOR_BGR2GRAY );
					break;
				default:
					frame_gray = frame.clone();
					break;
			}

			std::cout << "\tDetecting object with " << getName()
					<< "..." << std::endl;

			cascade.detectMultiScale(frame_gray, objects, _scale,
					_minNeighbours, 0, _minSize, _maxSize);

			std::cout << "\tFinished detecting objects." << std::endl;

			std::vector<cv::Rect>::iterator it = objects.begin();
			for (; it != objects.end(); it++) {
				Rect r;
				r.width = it->width; r.height = it->height;
				r.x = it->x; r.y = it->y;
				resultObjects.push_back(r);
			}

			return resultObjects;
		}
	};

	// Retrieve the engine version we're going to expect
	extern "C" CASCADECLASSIFIERDETECTORPLUGIN_API int getEngineVersion() {
		return 1;
	}

	// Register the plugin to an engine kernel
	extern "C" CASCADECLASSIFIERDETECTORPLUGIN_API void registerPlugin(
			Kernel &kernel, std::map<std::string, std::string> &args)
	{
		kernel.getDetectorFactory().addDetector(
				std::auto_ptr<DetectorFactory::Detector>(
						new CascadeClassifierDetector(args))
		);
	}

} // namespace PluginEngine
