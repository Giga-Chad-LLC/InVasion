#ifndef TILEMAPS_FILE_READER_H_
#define TILEMAPS_FILE_READER_H_

#include <vector>
#include <string>
#include <utility>
#include <filesystem>

namespace invasion::controllers {


class StaticObjectsFileReader {
public:

	class ObjectData {
	public:
		explicit ObjectData();
		explicit ObjectData(std::pair<double, double> shape, std::pair<double, double> position);

		std::pair<double, double> getShape() const;
		std::pair<double, double> getPosition() const;

	private:
		std::pair<double, double> m_shape;
		std::pair<double, double> m_position;
	};
	

	explicit StaticObjectsFileReader(const std::string& filepath);

	std::size_t getObjectsNumber() const;
	const std::vector<ObjectData>& getObjectsData() const;
	std::filesystem::path getFilePath() const;

private:
	std::filesystem::path m_filepath;
	std::size_t m_objectsNumber;
	std::vector<ObjectData> m_objects;
};


} // namespace invasion::controllers


#endif // TILEMAPS_FILE_READER_H_