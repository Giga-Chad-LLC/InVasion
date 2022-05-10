#include <filesystem>
#include <string>
#include <vector>

#include "directory-files-container.h"


namespace invasion::controllers {

namespace fs = std::filesystem;
	
DirectoryFilesContainer::DirectoryFilesContainer(const std::string& directory)
	: m_directory(directory) {}

std::vector<fs::directory_entry> DirectoryFilesContainer::obtainFilesWithExtension(
	const std::string& extention) const {
	
	std::vector<fs::directory_entry> entries;
	
	for(const auto& entry : fs::directory_iterator{m_directory}) {
		if(entry.exists() && entry.is_regular_file() && entry.path().extension() == extention) {
			entries.push_back(entry);
		}
	}

	return entries;
}


} // namespace invasion::controllers
