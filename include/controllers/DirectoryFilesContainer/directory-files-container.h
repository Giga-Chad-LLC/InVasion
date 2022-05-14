#ifndef DIRECTORY_FILES_ITERATOR_H_
#define DIRECTORY_FILES_ITERATOR_H_

#include <filesystem>
#include <string>
#include <vector>

namespace invasion::controllers {

namespace fs = std::filesystem;

class DirectoryFilesContainer {
public:
	explicit DirectoryFilesContainer(const std::string& directory);
	std::vector<fs::directory_entry> obtainFilesWithExtension(const std::string& extention) const;

private:
	fs::path m_directory;
};

} // namespace invasion::controllers



#endif // DIRECTORY_FILES_ITERATOR_H_
