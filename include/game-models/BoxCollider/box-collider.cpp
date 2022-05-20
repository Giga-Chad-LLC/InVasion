#include <utility>
#include <iostream>

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
		[[maybe_unused]] const Vector2D& otherCenter) const {
	
	const double xmin1 = center.getX() - m_size.getX() / 2.0;
	const double xmax1 = center.getX() + m_size.getX() / 2.0;

	const double ymin1 = center.getY() - m_size.getY() / 2.0;
	const double ymax1 = center.getY() + m_size.getY() / 2.0;

	const double xmin2 = otherCenter.getX() - other->m_size.getX() / 2.0;
	const double xmax2 = otherCenter.getX() + other->m_size.getX() / 2.0;

	const double ymin2 = otherCenter.getY() - other->m_size.getY() / 2.0;
	const double ymax2 = otherCenter.getY() + other->m_size.getY() / 2.0;

	if(xmax1 < xmin2 || xmin1 > xmax2)
		return false;
	
	if(ymax1 < ymin2 || ymin1 > ymax2)
		return false;

	return true;
}


} // namespace invasion::game_models
