#include <vector>
#include <string>
#include <utility>
#include <fstream>
#include <filesystem>
#include <stdexcept>

#include "static-objects-file-reader.h"


namespace invasion::controllers {

StaticObjectsFileReader::ObjectData::ObjectData() : m_shape(0, 0), m_position(0, 0) {}


StaticObjectsFileReader::ObjectData::ObjectData(std::pair<double, double> shape, std::pair<double, double> position)
	: m_shape(std::move(shape)), m_position(std::move(position)) {}


std::pair<double, double> StaticObjectsFileReader::ObjectData::getShape() const {
	return m_shape;
}


std::pair<double, double> StaticObjectsFileReader::ObjectData::getPosition() const {
	return m_position;
}




StaticObjectsFileReader::StaticObjectsFileReader(const std::string& filepath)
	: m_filepath(filepath), m_objectsNumber(0) {
	std::ifstream file(filepath);

	if(!file.is_open()) {
		throw std::runtime_error("Cannot open file '" + filepath + "'");
	}

	std::size_t objectsNumber;
	if(!(file >> objectsNumber)) {
		throw std::runtime_error("Cannot read number of objects from file '" + filepath + "'");
	}

	std::vector<StaticObjectsFileReader::ObjectData> objects(objectsNumber);

	for(int i = 0; i < objectsNumber; i++) {
		std::pair<double, double> shape;
		std::pair<double, double> position;
		
		if(!(file >> shape.first >>shape.second)) {
			throw std::runtime_error("Cannot read object's shape from file '" + filepath + "'");	
		}

		if(!(file >> position.first >> position.second)) {
			throw std::runtime_error("Cannot read object's position from file '" + filepath + "'");	
		}

		objects[i] = StaticObjectsFileReader::ObjectData(std::move(shape), std::move(position));
	}

	file.close();

	m_objectsNumber = objectsNumber;
	m_objects = std::move(objects);
}


std::size_t StaticObjectsFileReader::getObjectsNumber() const {
	return m_objectsNumber;
}


const std::vector<StaticObjectsFileReader::ObjectData>& StaticObjectsFileReader::getObjectsData() const {
	return m_objects;
}

std::filesystem::path StaticObjectsFileReader::getFilePath() const {
	return m_filepath;
}


} // namespace invasion::controllers
