#include <iostream>

#include "Vector2D/vector2d.h"

int main() {
	std::cout << "Hello World!" << " " << __cplusplus << std::endl;

	using invasion::game_models::Vector2D;

	Vector2D vec1;
	vec1.setX(100);
	vec1.setY(123);

	std::cout << vec1.getX() << " " << vec1.getY() << std::endl;

	return 0;
}