#ifndef RESPAWN_POINTS_FILE_READER_H_
#define RESPAWN_POINTS_FILE_READER_H_

#include <vector>
#include <string>
#include <filesystem>
#include <utility>


namespace invasion::controllers {


class RespawnPointsFileReader {
public:

	class ObjectData {
	public:
		ObjectData() = default; 
		explicit ObjectData(std::pair<double, double> position);
		std::pair<double, double> getPosition() const;

	private:
		std::pair<double, double> m_position;
	};

	explicit RespawnPointsFileReader(const std::string& filepath);

	const std::vector<ObjectData>& getObjectsData() const;
	std::filesystem::path getFilePath() const;

private:
	std::filesystem::path m_filepath;
	std::vector<ObjectData> m_objects;
};


} // namespace invasion::controllers


#endif // RESPAWN_POINTS_FILE_READER_H_
