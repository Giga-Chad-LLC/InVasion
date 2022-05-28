#include <utility>
#include <cmath>
#include <iostream>

#include "box-collider.h"


namespace invasion::game_models {

BoxCollider::BoxCollider(Vector2D size) 
	: m_size(std::move(size)) {}


Vector2D BoxCollider::size() const {
	return m_size;
}

bool BoxCollider::collidesWith(const Vector2D& center, const BoxCollider& other, const Vector2D& otherCenter) const {
	
	const double xmin1 = center.getX() - m_size.getX() / 2.0;
	const double xmax1 = center.getX() + m_size.getX() / 2.0;

	const double ymin1 = center.getY() - m_size.getY() / 2.0;
	const double ymax1 = center.getY() + m_size.getY() / 2.0;

	const double xmin2 = otherCenter.getX() - other.m_size.getX() / 2.0;
	const double xmax2 = otherCenter.getX() + other.m_size.getX() / 2.0;

	const double ymin2 = otherCenter.getY() - other.m_size.getY() / 2.0;
	const double ymax2 = otherCenter.getY() + other.m_size.getY() / 2.0;

	if(xmax1 < xmin2 || xmin1 > xmax2)
		return false;
	
	if(ymax1 < ymin2 || ymin1 > ymax2)
		return false;

	return true;
}


double BoxCollider::calculateClosestDistanceBetween(
	const Vector2D& center, const BoxCollider& other, const Vector2D& otherCenter) const {
	
	const double dx = std::abs(center.getX() - otherCenter.getX());
	const double dy = std::abs(center.getY() - otherCenter.getY());

	const double widthSumHalf = (m_size.getX() + other.m_size.getX()) / 2.0;
	const double heightSumHalf = (m_size.getY() + other.m_size.getY()) / 2.0;

	double minDistance = 0.0;

	// Rectangles do not intersect:

	if(dx < widthSumHalf && dy >= heightSumHalf) {
		// There are two partially overlapping rectangles in X-axis direction
		// Min distance is distance between bottom line of upper rectangle and top line of lower rectangle
		minDistance = dy - heightSumHalf;
	}
	else if(dx >= widthSumHalf && dy < heightSumHalf) {
		// There are two partially overlapping rectangles in Y-axis direction
		// Minimum distance is distance between right line of left rectangle and left line of right rectangle
		minDistance = dx - widthSumHalf; 
	}
	else if(dx >= widthSumHalf && dy >= heightSumHalf) {
		// Two rectangles do not intersect in X and Y axis directions
		// Min distance is distance between two nearest vertices (Pythagorean theorem)
		const double deltaX = dx - widthSumHalf;
		const double deltaY = dy - heightSumHalf;
		minDistance = std::sqrt(deltaX * deltaX + deltaY * deltaY);
	}

	return minDistance;
}


} // namespace invasion::game_models
