#ifndef TILEMAPS_FILE_READER_H_
#define TILEMAPS_FILE_READER_H_

#include <vector>
#include <string>
#include <utility>

namespace invasion::controllers {
	

class TilemapsFileReader {
public:
	explicit TilemapsFileReader(const std::string& filepath);

	std::pair<double, double> getTileDimensions() const;
	std::size_t getTilesNumber() const;
	const std::vector<std::pair<double, double>>& getTileCentersPositions() const;
	std::string getFileName() const;

private:
	std::string m_filename;
	std::pair<double, double> m_tileDimensions;
	std::size_t m_tilesNumber;
	std::vector<std::pair<double, double>> m_tileCentersPositions;
};


} // namespace invasion::controllers


#endif // TILEMAPS_FILE_READER_H_