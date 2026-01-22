#include "Level.h"
#include <iostream>

Level::Level(sf::RenderWindow& hwnd, Input& in) :
	BaseLevel(hwnd, in)
{
	// initialise game objects

	m_player.setRadius(10.f);
	m_player.setFillColor(sf::Color::Green);
	m_player.setPosition({ 300.f, 300.f });

	// store original size/color for resets and flashing
	m_originalColor = m_player.getFillColor();   
	m_originalRadius = m_player.getRadius();     

	// setup food
	m_food.setRadius(5.f);
	m_food.setFillColor(sf::Color::Red);

	spawnFood();
}

// handle user input
void Level::handleInput(float dt)
{
	if (m_input.isLeftMousePressed())
	{
		std::cout << "left mouse pressed" << std::endl;
	}

	if (m_input.isRightMousePressed())
	{
		std::cout << "right mouse pressed" << std::endl;
	}

	if (m_input.isKeyDown(sf::Keyboard::Scancode::D))
		m_direction = Direction::RIGHT;

	if (m_input.isKeyDown(sf::Keyboard::Scancode::A))
		m_direction = Direction::LEFT;

	if (m_input.isKeyDown(sf::Keyboard::Scancode::W))
		m_direction = Direction::UP;

	if (m_input.isKeyDown(sf::Keyboard::Scancode::S))
		m_direction = Direction::DOWN;

	// game reset input
	if ((m_isGameOver || m_isGameWon) && m_input.isKeyDown(sf::Keyboard::Scancode::R))
	{
		resetGame();
	}

}

// Update game objects
void Level::update(float dt)
{
	// Always accumulate time
	m_timeElapsed += dt;

	// Check for end of game (win or lose)
	if (m_isGameOver || m_isGameWon)
	{
		// Only print the end message once
		if (!m_hasPrintedEndMessage)
		{
			if (m_isGameOver)
			{
				std::cout << "GAME OVER\n";
			}
			else // player won
			{
				std::cout << "YOU WIN!\n";
				m_player.setRadius(m_originalRadius * 3.f); // grow for celebration
			}

			std::cout << "Score: " << m_score << "\n";
			std::cout << "Time: " << m_timeElapsed << " seconds\n";
			std::cout << "Press R to Restart the game!\n";

			m_hasPrintedEndMessage = true;
		}

		// handle win flashing
		if (m_isGameWon)
		{
			m_flashTimer += dt;
			if (m_flashTimer >= m_flashInterval)
			{
				m_flashTimer = 0.f;
				m_flashToggle = !m_flashToggle;
				m_player.setFillColor(m_flashToggle ? sf::Color::Yellow : m_originalColor);
			}
		}

		// stop all movement and collisions
		return;
	}

	// sustained movement
	switch (m_direction)
	{
	case Direction::UP:
		m_player.move({ 0.f, -m_speed * dt });
		break;

	case Direction::DOWN:
		m_player.move({ 0.f, m_speed * dt });
		break;

	case Direction::LEFT:
		m_player.move({ -m_speed * dt, 0.f });
		break;

	case Direction::RIGHT:
		m_player.move({ m_speed * dt, 0.f });
		break;
	}

	// Boundary check
	sf::Vector2f pos = m_player.getPosition();
	float radius = m_player.getRadius();
	sf::Vector2u windowSize = m_window.getSize();

	// Check game field bounds
	if (pos.x > windowSize.x - radius * 2 || pos.x < 0 ||
		pos.y > windowSize.y - radius * 2 || pos.y < 0)
	{
		// mark game over
		m_isGameOver = true;
	}

	// Collision with food
	sf::Vector2f playerCentre = m_player.getPosition() + sf::Vector2f(m_player.getRadius(), m_player.getRadius());
	sf::Vector2f foodCentre = m_food.getPosition() + sf::Vector2f(m_food.getRadius(), m_food.getRadius());

	// Compute squared distance
	float x_dist = playerCentre.x - foodCentre.x;
	float y_dist = playerCentre.y - foodCentre.y;
	float squared_dist = (x_dist * x_dist) + (y_dist * y_dist);

	// Sum of radii
	float radii_sum = m_player.getRadius() + m_food.getRadius();

	// Check collision
	if (squared_dist < radii_sum * radii_sum)
	{
		// move the food to a new random location
		spawnFood(); 
		// increase speed by 10%
		m_speed *= 1.1f;   
		// clamp min/max
		m_speed = std::clamp(m_speed, m_minSpeed, m_maxSpeed);  

		// increment score
		m_score++;  

		// print score and current speed in debug window
		std::cout << "Food eaten! Score: " << m_score << ", Speed: " << m_speed << " px/s" << std::endl;
	}

	// game win condition
	if (m_score >= m_scoreToWin && !m_isGameWon)
	{
		m_isGameWon = true;
		std::cout << "YOU WIN!\n";
		std::cout << "Final Score: " << m_score
			<< "\nTime: " << m_timeElapsed << " seconds\n";

		// Trigger celebration effect
		// Grow the snake
		m_player.setRadius(m_player.getRadius() * 3.f);

		// Store original color to flash later
		m_originalColor = m_player.getFillColor();
	}
}

// food spawn
void Level::spawnFood()
{
	// Get centre position of the snake
	sf::Vector2f snakeCentre =
		m_player.getPosition() +
		sf::Vector2f(m_player.getRadius(), m_player.getRadius());

	float x = static_cast<float>(rand() % m_window.getSize().x);
	float y = static_cast<float>(rand() % m_window.getSize().y);

	// Minimum distance from snake (¼ of smallest window dimension)
	float minDist = std::min(
		m_window.getSize().x * 0.25f,
		m_window.getSize().y * 0.25f
	);

	// Keep generating positions while too close to snake
	while (std::abs(x - (snakeCentre.x + m_food.getRadius())) <= minDist &&
		std::abs(y - (snakeCentre.y + m_food.getRadius())) <= minDist)
	{
		x = static_cast<float>(rand() % m_window.getSize().x);
		y = static_cast<float>(rand() % m_window.getSize().y);
	}

	m_food.setPosition({ x, y });
}

// Render level
void Level::render()
{
	beginDraw();

	m_window.draw(m_food);
	m_window.draw(m_player);

	endDraw();
}

// reset level
void Level::resetGame()
{
	// Reset player
	m_player.setPosition({ m_window.getSize().x * 0.5f, m_window.getSize().y * 0.5f });
	// reset to starting speed
	m_speed = 200.f;               
	m_direction = Direction::RIGHT;

	// Reset game state
	m_score = 0;
	m_timeElapsed = 0.f;
	m_isGameOver = false;
	m_isGameWon = false;
	m_flashTimer = 0.f;
	m_player.setFillColor(m_originalColor);
	m_player.setRadius(m_originalRadius);
	m_hasPrintedEndMessage = false;

	// Spawn new food
	spawnFood();

	std::cout << "Game restarted!" << std::endl;
}

