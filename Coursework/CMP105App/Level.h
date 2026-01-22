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

	bool m_isGameOver = false;  // true when the snake hits a wall
	int m_score = 0;            // number of food eaten
	float m_timeElapsed = 0.f;  // time since game start in seconds

	// player speed
	float m_speed = 200.f;

	float m_minSpeed = 200.f;   // minimum allowed speed
	float m_maxSpeed = 600.f;   // maximum allowed speed

	// movement direction
	enum class Direction { UP, DOWN, LEFT, RIGHT };
	Direction m_direction = Direction::RIGHT;

	// spawn food
	void spawnFood();

	// level reset
	void resetGame();

	// Default variables for level class.
	
};