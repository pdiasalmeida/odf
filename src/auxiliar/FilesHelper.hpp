#ifndef FILESHELPER_HPP_
#define FILESHELPER_HPP_

#include <opencv2/core/core.hpp>

#include <string>
#include <vector>
#include <map>

class FilesHelper
{
public:
	static std::string toLowerCase(std::string in);

	static void getFilesInDirectory(const std::string& dirName,
			std::vector<std::string>& fileNames,
			const std::vector<std::string>& validExtensions);

	static std::string getFileName(std::string fullPath);
	static std::string getFileNameNoExt(std::string fullPath);
	static std::string getDirName(std::string fullPath);
	static std::string getLeafDirName(std::string fullPath);
	static bool fileAvailable(std::string dirName, std::string fileName,
			std::vector<std::string>& validExtensions);

	static void parseKeyValue(std::string file, char delimiter,
			std::map<std::string, std::vector<std::string> >& kValues);
	static void parseGroundTruth(std::string file, char delimiter,
			std::vector<std::pair<std::string, std::pair<cv::Rect, std::string> > >& objs);
	static void printKeyValue(std::map<std::string, std::vector<std::string> >& kValues);

	static void filterChars(std::string& str, char* chars);
	static int countAlpha(std::string& str);
	static int countDigit(std::string& str);

	static std::vector<std::string> streamSplit(std::istringstream& instream,
			char delimiter);
	static int stringSplit(const std::string &s, char delim,
			std::vector<std::string>& elems);

	static bool debug;

	virtual ~FilesHelper();
private:
	FilesHelper();
};

#endif /* FILESHELPER_HPP_ */
