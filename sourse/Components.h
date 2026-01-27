#pragma once

#include "Vec2.h"
#include<SFML/Graphics.hpp>

class CTransform
{
public:
	Vec2 previousPos = { 0.0, 0.0 };
	Vec2 pos = { 0.0, 0.0 };
	Vec2 velocity = { 0.0, 0.0 };
	float angle = 0;

	CTransform(const Vec2& pp, const Vec2& p, const Vec2& v, float a)
		: previousPos(pp), pos(p), velocity(v), angle(a) {
	}
	CTransform(const Vec2& p, const Vec2& v, float a)
		: pos(p), velocity(v), angle(a) {
	}

};

class CShape
{
public:
	sf::CircleShape circle;

	CShape(float radius, int points, const sf::Color& fill, const sf::Color& outline, float thickness)
		: circle(radius, points)
	{
		circle.setFillColor(fill);
		circle.setOutlineColor(outline);
		circle.setOutlineThickness(thickness);
		circle.setOrigin(radius, radius);
	}
};

class CCollision
{
public:
	float radius = 0;
	CCollision(float r)
		: radius(r) {
	}
};

class CScore
{
public:
	int score = 0;
	CScore(int s)
		: score(s) {
	}
};

class CLifespan
{
public:
	int remaining = 0;
	int total = 0;
	CLifespan(int total)
		:remaining(total), total(total) {
	}
};

class CInput
{
public:
	bool up = false;
	bool left = false;
	bool right = false;
	bool down = false;
	bool shoot = false;
};

// Transform component: position, velocity, scale, and rotation angle
class CTransform1 {
public:
	Vec2 pos;
	Vec2 prevPos;
	Vec2 velocity;
	Vec2 scale = Vec2(1.0f, 1.0f); // Default scale to (1.0f, 1.0f)
	double angle = 0.0; // Default angle to 0.0

	// Default constructor
	CTransform1() {}

	// Constructor with position
	CTransform1(const Vec2& p)
		: pos(p), velocity(0, 0) {
	} // Initialize velocity to (0, 0)

// Constructor with position and velocity
	CTransform1(const Vec2& p, const Vec2& vel)
		: pos(p), velocity(vel) {
	}

	// Constructor with position, velocity, scale, and angle
	CTransform1(const Vec2& p, const Vec2& vel, const Vec2& s, double a)
		: pos(p), velocity(vel), scale(s), angle(a) {
	}
};

// Bounding box component: size (width, height)
class CBoundingBox
{
public:
	Vec2 size;
	Vec2 halfSize;

	CBoundingBox() {}
	CBoundingBox(const Vec2& s)
		: size(s), halfSize(s.x / 2.0f, s.y / 2.0f) {
	}
};