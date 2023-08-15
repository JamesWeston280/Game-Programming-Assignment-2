#pragma once

#include "Vec2.h"
#include <SFML/Graphics.hpp>

class CTransform
{
public:
	Vec2 pos = { 0.0, 0.0 };
	Vec2 velocity = { 0.0, 0.0 };
	float angle = 0;

	CTransform(const Vec2& pos, const Vec2& velocity, float angle)
		: pos(pos), velocity(velocity), angle(angle)
	{
	}

};

class CShape
{
public:
	sf::CircleShape circle;

	CShape(float radius, int points, const sf::Color& fill, 
		const sf::Color& outline, float thickness)

		:circle(radius,radius)
	{
		circle.setFillColor(fill);
		circle.setOutlineColor(outline);
		circle.setOutlineThickness(thickness);
		circle.setOrigin(radius, radius);
		circle.setPointCount(points);
	}

};

class CCollision
{
public:
	float radius = 0;
	CCollision(float radius)
		: radius(radius)
	{}

};

class CScore
{
public:
	int score = 0;
	CScore(int score)
		: score(score)
	{}

};

class CLifespan
{
public:
	int remaining = 0; // lifespan remaining
	int total = 0; // total inital lifespan
	CLifespan(int total)
		:remaining(total), total(total)
	{}

};

class CInput
{
public:
	bool up = false;
	bool left = false;
	bool right = false;
	bool down = false;
	bool shoot = false;

	CInput()
	{}

};