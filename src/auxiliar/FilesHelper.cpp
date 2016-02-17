#include "FilesHelper.hpp"

#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>

#include <algorithm>
#include <dirent.h>

bool FilesHelper::debug = false;

std::string FilesHelper::toLowerCase(std::string in)
{
	std::transform(in.begin(), in.end(), in.begin(), ::tolower);

	return in;
}

void FilesHelper::getFilesInDirectory(const std::string& dirName,
		std::vector<std::string>& fileNames,
		const std::vector<std::string>& validExtensions)
{
	if (FilesHelper::debug)
		std::cout << "Opening directory " << dirName << "..." << std::endl;

	struct dirent* ep;
	size_t extensionLocation;

	DIR* dp = opendir(dirName.c_str());
	if (dp != NULL) {
		while ((ep = readdir(dp))) {
			if (ep->d_type & DT_DIR) {
				continue;
			}

			extensionLocation = std::string(ep->d_name).find_last_of(".");
			std::string tempExt = toLowerCase(std::string(ep->d_name).substr(extensionLocation + 1 ));

			if (find(validExtensions.begin(), validExtensions.end(), tempExt) != validExtensions.end()) {
				if (FilesHelper::debug)
					std::cout << "Found matching data file '" << ep->d_name
					<< "'." << std::endl;

				fileNames.push_back(ep->d_name);
			} else {
				if (FilesHelper::debug)
					std::cout << "Found file does not match required file type, skipping: '"
					<< ep->d_name << "'!" << std::endl;
			}
		}
		closedir(dp);
	} else {
		if (FilesHelper::debug)
			std::cout << "Error opening directory " << dirName << "!" << std::endl;
	}
	return;
}

bool FilesHelper::fileAvailable(std::string dirName, std::string fileName,
		std::vector<std::string>& validExtensions)
{
	bool result = false;

	struct dirent* ep;
	size_t extensionLocation;

	DIR* dp = opendir(dirName.c_str());
	if (dp != NULL) {
		while ((ep = readdir(dp))) {
			if (ep->d_type & DT_DIR) {
				continue;
			}

			extensionLocation = std::string(ep->d_name).find_last_of(".");
			std::string tempExt = toLowerCase(std::string(ep->d_name).substr(extensionLocation + 1));

			if (find(validExtensions.begin(), validExtensions.end(), tempExt) != validExtensions.end()) {
				if(fileName.compare(std::string(ep->d_name)) == 0)
					result = true;
			}
		}
			closedir(dp);
	}

	return result;
}

std::string FilesHelper::getFileName(std::string fullPath)
{
	int ini = fullPath.find_last_of("/");
	std::string result = fullPath.substr(ini+1);

	return result;
}

std::string FilesHelper::getFileNameNoExt(std::string fullPath)
{
	int ini = fullPath.find_last_of("/");
	int fin = fullPath.find_last_of(".");
	std::string result = fullPath.substr(ini+1, fin-(ini+1));

	return result;
}

std::string FilesHelper::getDirName(std::string fullPath)
{
	int ini = fullPath.find_last_of("/");

	std::string result = fullPath.substr(0, ini);

	return result;
}

std::string FilesHelper::getLeafDirName(std::string fullPath)
{
	int ini = fullPath.find_last_of("/");
	if (ini == (int)(fullPath.size()-1)) {
		ini = (fullPath.substr(0, fullPath.size()-1)).find_last_of( "/" );
	}

	std::string result = fullPath.substr(ini+1);

	return result;
}

void FilesHelper::parseKeyValue(std::string file, char delimiter,
		std::map<std::string, std::vector<std::string> >& kValues)
{
	std::ifstream infile;
	std::vector<std::string> tokens;

	infile.open(file.c_str());
	if (infile.is_open()) {
		while (infile) {
			std::string line;
			getline(infile, line);
			std::istringstream iss(line);

			tokens = streamSplit(iss, delimiter);
			if (tokens.size() > 1) {
				std::string key(tokens.at(0));
				tokens.erase(tokens.begin());

				std::map<std::string, std::vector<std::string> >::iterator it;
				it = kValues.find(key);
				if (it == kValues.end()) {
					kValues.insert(std::pair<std::string, std::vector<std::string> >(key, tokens));
				} else {
					it->second.reserve(it->second.size()+tokens.size());
					it->second.insert(it->second.end(), tokens.begin(),tokens.end());
				}
			}
		}
	}

	infile.close();
}

void
FilesHelper::parseGroundTruth(std::string file, char delimiter,
		std::vector<std::pair<std::string, std::pair<cv::Rect, std::string> > >& objs)
{
	std::ifstream infile;
	std::vector<std::string> tokens;

	infile.open(file.c_str());
	if (infile.is_open()) {
		while(infile) {
			std::string line;
			getline(infile, line);
			std::istringstream iss(line);

			tokens = streamSplit(iss, delimiter);
			if (tokens.size() >= 5) {
				std::string image = getFileName(tokens.at(0));
				int x = atoi(tokens.at(1).c_str());
				int y = atoi(tokens.at(2).c_str());
				int w = atoi(tokens.at(3).c_str());
				int h = atoi(tokens.at(4).c_str());
				cv::Rect imR = cv::Rect(x, y, w, h);

				if (tokens.size()==5)
					objs.push_back(std::make_pair(image, std::make_pair(imR, "")));
				else
					objs.push_back(std::make_pair(image, std::make_pair(imR, tokens.at(5))));
			}
		}
	}

	infile.close();
}

void FilesHelper::printKeyValue(std::map<std::string, std::vector<std::string> >& kValues)
{
	std::cout << "Found " << kValues.size() << " files." << std::endl;

	std::map<std::string, std::vector<std::string> >::iterator itKV = kValues.begin();
	for ( ; itKV != kValues.end(); itKV++) {
		std::cout << itKV->first << std::endl;
		std::cout << "\t";
		std::vector<std::string>::iterator itSV = itKV->second.begin();
		for ( ; itSV != itKV->second.end(); itSV++) {
			std::cout << " "; std::cout << *itSV;
		}
		std::cout << std::endl;
	}
}

void FilesHelper::filterChars(std::string& str, char* chars)
{
	for (unsigned int i = 0; i < strlen(chars); ++i) {
		str.erase(std::remove(str.begin(), str.end(), chars[i]), str.end());
	}
}

int FilesHelper::countAlpha(std::string& str)
{
	int result = 0;
	for (int i = 0; str[i]; i++ ) {
		if (isalpha(str[i]))
			result++;
	}

	return result;
}

int FilesHelper::countDigit(std::string& str)
{
	int result = 0;
	for (int i = 0; str[i]; i++) {
		if (isdigit(str[i]))
			result++;
	}

	return result;
}

std::vector< std::string > FilesHelper::streamSplit(std::istringstream& instream,
		char delimiter)
{
	std::string token;
	std::vector<std::string> result;

	while (getline(instream, token, delimiter)) {
		result.push_back(token);
	}

	return result;
}

std::vector<std::string> &split(const std::string &s, char delim,
		std::vector<std::string> &elems)
{
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }

    return elems;
}


int FilesHelper::stringSplit(const std::string &s, char delim,
		std::vector<std::string>& elems)
{
	std::stringstream ss(s);
	std::string item;

	int n = 0;
	while (std::getline(ss, item, delim)) {
		if (!item.empty()) {
			elems.push_back(item);
			n++;
		}
	}

    return n;
}
