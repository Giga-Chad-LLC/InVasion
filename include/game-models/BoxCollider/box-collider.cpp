#include <utility>

#include "box-collider.h"


namespace invasion::game_models {

BoxCollider::BoxCollider(Vector2D size) 
	: m_size(std::move(size)) {}


Vector2D BoxCollider::size() const {
	return m_size;
}

bool BoxCollider::collidesWith(
		[[maybe_unused]] const Vector2D& center, 
		[[maybe_unused]] const BoxCollider* const other, 
		[[maybe_unused]] const Vector2D& other_center) const {
	std::cout << m_size.getX() << " " << m_size.getY() << std::endl;
	std::cout << "Checking collision..." << std::endl;
	return true;
}


} // namespace invasion::game_models
