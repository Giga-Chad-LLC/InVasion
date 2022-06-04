#include <vector>
#include <string>
#include <filesystem>
#include <utility>
#include <fstream>

#include "respawn-points-file-reader.h"


namespace invasion::controllers {
	
// RespawnPointsFileReader::ObjectData
RespawnPointsFileReader::ObjectData::ObjectData(std::pair<double, double> position)
	: m_position(std::move(position)) {}


std::pair<double, double> RespawnPointsFileReader::ObjectData::getPosition() const {
	return m_position;
}


// RespawnPointsFileReader
RespawnPointsFileReader::RespawnPointsFileReader(const std::string& filepath)
	: m_filepath(filepath) {

	std::ifstream file(filepath);

	if(!file.is_open()) {
		throw std::runtime_error("Cannot open file '" + filepath + "'");
	}

	std::size_t n;
	if(!(file >> n)) {
		throw std::runtime_error("Cannot read number of objects from file '" + filepath + "'");
	}

	std::vector<RespawnPointsFileReader::ObjectData> objects;
	objects.reserve(n);

	for(std::size_t i = 0; i < n; i++) {
		std::pair<double, double> position;
		
		if(!(file >> position.first >> position.second)) {
			throw std::runtime_error("Cannot read object's position from file '" + filepath + "'");	
		}

		objects.emplace_back(RespawnPointsFileReader::ObjectData(std::move(position)));
	}

	file.close();
	m_objects = std::move(objects);
}


const std::vector<RespawnPointsFileReader::ObjectData>& RespawnPointsFileReader::getObjectsData() const {
	return m_objects;
}


std::filesystem::path RespawnPointsFileReader::getFilePath() const {
	return m_filepath;
}


} // namespace invasion::controllers
