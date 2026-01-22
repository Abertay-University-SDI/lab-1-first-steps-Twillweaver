#pragma once

#include "Framework/BaseLevel.h"
#include <SFML/Graphics.hpp>

class Level : public BaseLevel{
public:
	Level(sf::RenderWindow& window, Input& input);
	~Level() override = default;

	void handleInput(float dt) override;
	void update(float dt) override;
	void render() override;

private:
	// Default functions for rendering to the screen.

	// player representation
	sf::CircleShape m_player;

	// food representation
	sf::CircleShape m_food;

	// player speed
	float m_speed = 200.f;

	float m_minSpeed = 100.f;   // minimum allowed speed
	float m_maxSpeed = 800.f;   // maximum allowed speed

	// movement direction
	enum class Direction { UP, DOWN, LEFT, RIGHT };
	Direction m_direction = Direction::RIGHT;

	void spawnFood();

	// Default variables for level class.
	
};