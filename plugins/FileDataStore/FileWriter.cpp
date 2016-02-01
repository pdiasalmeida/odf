#define FILEDATASTOREPLUGIN_SOURCE 1

#include "FileWriter.hpp"

#include <iostream>
#include <sys/stat.h>

std::map<std::string, std::ofstream*> FileWriter::_files;

void FileWriter::openFile(std::string dirPath, std::string name)
{
	std::ofstream* file = new std::ofstream;
	std::string fullPath = dirPath+"/"+name;
	mkdir(dirPath.c_str(), 0777);

	file->open(fullPath.c_str(), std::ofstream::app);

	FileWriter::_files.insert(std::pair< std::string, std::ofstream* >(name, file));
}

void FileWriter::writeToFile(std::string file, std::string content)
{
	std::map< std::string, std::ofstream* >::iterator it = FileWriter::_files.find(file);
	if (it != FileWriter::_files.end()) {
		(*it->second) << content;
		(*it->second).flush();
	}
}

void FileWriter::setFileContent(std::string path, std::string content)
{
	std::ofstream file;

	mkdir(getDirName(path).c_str(), 0777);

	file.open(path.c_str(), std::ofstream::out);
	file << content;
	file.close();
}

std::string FileWriter::getDirName(std::string fullPath)
{
	int ini = fullPath.find_last_of( "/" );

	std::string result = fullPath.substr( 0, ini );

	return result;
}

std::string FileWriter::getFileNameNoExt(std::string fullPath)
{
	int ini = fullPath.find_last_of( "/" );
	int fin = fullPath.find_last_of( "." );
	std::string result = fullPath.substr(ini+1, fin-(ini+1));

	return result;
}

std::string FileWriter::getLeafDirName(std::string fullPath)
{
	int ini = fullPath.find_last_of("/");
	if (ini == (int)(fullPath.size()-1)) {
		ini = (fullPath.substr(0, fullPath.size()-1)).find_last_of( "/" );
	}

	std::string result = fullPath.substr(ini+1);

	return result;
}

void FileWriter::close()
{
	std::map< std::string, std::ofstream* >::iterator it = FileWriter::_files.begin();
	for ( ; it != FileWriter::_files.end(); it++ ) {
		it->second->close();
	}
}

FileWriter::~FileWriter()
{
	FileWriter::close();
}
