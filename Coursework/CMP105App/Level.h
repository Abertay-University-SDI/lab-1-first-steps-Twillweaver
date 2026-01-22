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

	// player speed
	float m_speed = 200.f;

	// movement direction
	enum class Direction { UP, DOWN, LEFT, RIGHT };
	Direction m_direction = Direction::RIGHT;

	// Default variables for level class.
	
};