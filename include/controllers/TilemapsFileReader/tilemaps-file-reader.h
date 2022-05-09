#ifndef TILEMAPS_FILE_READER_H_
#define TILEMAPS_FILE_READER_H_

#include <vector>
#include <string>
#include <utility>
#include <filesystem>

namespace invasion::controllers {


class TilemapsFileReader {
public:
	explicit TilemapsFileReader(const std::string& filepath);

	std::pair<double, double> getTileDimensions() const;
	std::size_t getTilesNumber() const;
	const std::vector<std::pair<double, double>>& getTileCentersPositions() const;
	std::filesystem::path getFilePath() const;

private:
	std::filesystem::path m_filepath;
	std::pair<double, double> m_tileDimensions;
	std::size_t m_tilesNumber;
	std::vector<std::pair<double, double>> m_tileCentersPositions;
};


} // namespace invasion::controllers


#endif // TILEMAPS_FILE_READER_H_