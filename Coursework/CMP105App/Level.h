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
	// Default functions for rendering to the screen
	// default variables for level class

	// player representation
	sf::CircleShape m_player;

	// food representation
	sf::CircleShape m_food;

	// true when the snake hits a wall
	bool m_isGameOver = false;
	// number of food eaten
	int m_score = 0;        
	// time since game start in seconds
	float m_timeElapsed = 0.f;  

	// player speed
	float m_speed = 200.f;

	// minimum allowed speed
	float m_minSpeed = 200.f;   
	// maximum allowed speed
	float m_maxSpeed = 600.f;   

	// movement direction
	enum class Direction { UP, DOWN, LEFT, RIGHT };
	Direction m_direction = Direction::RIGHT;

	// spawn food
	void spawnFood();

	// level reset
	void resetGame();

	// player wins after eating 10 food
	int m_scoreToWin = 10;  

	// number of lives
	int m_lives = 3;

	// game win condition
	bool m_isGameWon = false;

	// player's original size
	float m_originalRadius = 10.f;

	// toggle flashing on/off
	bool m_flashToggle = false;      
	// timer to control flash speed
	float m_flashTimer = 0.f;         
	// flash every 0.5 second
	float m_flashInterval = 0.5f;     
	// store original snake color
	sf::Color m_originalColor;  

	// end of game message
	bool m_hasPrintedEndMessage = false;
	
};