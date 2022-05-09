#include <vector>
#include <string>
#include <utility>
#include <fstream>
#include <stdexcept>

#include "tilemaps-file-reader.h"


namespace invasion::controllers {

TilemapsFileReader::TilemapsFileReader(const std::string& filepath)
	: m_filename(filepath), m_tilesNumber(0) {
	std::ifstream file(filepath);

	if(!file.is_open()) {
		throw std::runtime_error("Cannot open file '" + filepath + "'");
	}

	std::pair<double, double> tileDimensions;
	if(!(file >> tileDimensions.first >> tileDimensions.second)) {
		throw std::runtime_error("Cannot read tile dimensions from file '" + filepath + "'");
	}

	std::size_t tilesNumber;
	if(!(file >> tilesNumber)) {
		throw std::runtime_error("Cannot read number of tiles from file '" + filepath + "'");
	}

	std::vector<std::pair<double, double>> tileCentersPositions(tilesNumber);
	for(int i = 0; i < tilesNumber; i++) {
		std::pair<double, double> center;
		
		if(!(file >> center.first >> center.second)) {
			throw std::runtime_error("Cannot read tile center position from file '" + filepath + "'");	
		}

		tileCentersPositions[i] = std::move(center);
	}

	file.close();

	m_tileDimensions = std::move(tileDimensions);
	m_tilesNumber = tilesNumber;
	m_tileCentersPositions = std::move(tileCentersPositions);
}


std::pair<double, double> TilemapsFileReader::getTileDimensions() const {
	return m_tileDimensions;
}


std::size_t TilemapsFileReader::getTilesNumber() const {
	return m_tilesNumber;
}


const std::vector<std::pair<double, double>>& TilemapsFileReader::getTileCentersPositions() const {
	return m_tileCentersPositions;
}


std::string TilemapsFileReader::getFileName() const {
	return m_filename;
}


} // namespace invasion::controllers
