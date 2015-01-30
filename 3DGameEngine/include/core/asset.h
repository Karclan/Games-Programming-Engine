#ifndef ASSET_H
#define ASSET_H

#include <string>

/*! \brief Asset class

	All assets should inherit from this so that they can store path (Init table needs this to
	write data from component).
*/
class Asset
{
public:
	void setFilePath (std::string filePath) { _filePath = filePath; }
	std::string getFilePath () { return _filePath; }

private:
	std::string _filePath;

};



#endif