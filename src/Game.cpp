#include "Game.hpp"
#include <iostream>

void printKey(const sf::Event::KeyPressed* keyPressed)
{
	std::string keyName = "Unknown";
	switch (keyPressed->code)
	{
	case sf::Keyboard::Key::W:
		keyName = "W";
		break;
	case sf::Keyboard::Key::A:
		keyName = "A";
		break;
	case sf::Keyboard::Key::S:
		keyName = "S";
		break;
	case sf::Keyboard::Key::D:
		keyName = "D";
		break;
	case sf::Keyboard::Key::Up:
		keyName = "Up Arrow";
		break;
	case sf::Keyboard::Key::Down:
		keyName = "Down Arrow";
		break;
	case sf::Keyboard::Key::Left:
		keyName = "Left Arrow";
		break;
	case sf::Keyboard::Key::Right:
		keyName = "Right Arrow";
		break;
	case sf::Keyboard::Key::Space:
		keyName = "Space";
		break;
	case sf::Keyboard::Key::Escape:
		keyName = "Escape";
		break;
	default:
		keyName = "Other (" + std::to_string(static_cast<int>(keyPressed->scancode)) + ")";
	}

	std::cout << "Key pressed: " << keyName << std::endl
			  << std::flush;
}

Game::Game() : window(sf::VideoMode(sf::Vector2u(800, 600)), "Fighting Game"),
               player1(100, 400, true), player2(600, 400, false),
               healthBar1(window.getSize().x /2 - 350, 10, player1.getHealth()), 
               healthBar2(window.getSize().x /2 + 150, 10, player2.getHealth())
{
	window.setFramerateLimit(60);
	std::cout << "Game initialized. Window created with size: " << window.getSize().x << "x" << window.getSize().y << std::endl
			  << std::flush;
	
	// Store the result of setActive to avoid the warning
	bool activated = window.setActive(true);
	if (!activated) {
		std::cerr << "Warning: Failed to activate window context" << std::endl;
	}
}

void Game::run()
{
    while (window.isOpen())
    {
        processEvents();
        
        // Fixed time step for simplicity
        float deltaTime = 1.0f/60.0f;
        
        // Update game state
        player1.update(deltaTime);
        player2.update(deltaTime);
        healthBar1.update(player1.getHealth());
        healthBar2.update(player2.getHealth());
        
        // Render
        render();
    }
}

void Game::processEvents()
{
	// Process one-time events
	while (const auto event = window.pollEvent())
	{
		if (event->is<sf::Event::Closed>())
		{
			window.close();
		}
		else if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>())
		{
			printKey(keyPressed);
			
			// Handle key presses
			switch (keyPressed->code)
			{
				case sf::Keyboard::Key::W:
					player1.jump();
					break;
				case sf::Keyboard::Key::A:
					player1.moveLeft();
					break;
				case sf::Keyboard::Key::D:
					player1.moveRight();
					break;
				case sf::Keyboard::Key::Q:
					player1.punch();
					break;
				case sf::Keyboard::Key::E:
					player1.kick();
					break;
				case sf::Keyboard::Key::S:
					player1.block();
					break;
				case sf::Keyboard::Key::Up:
					player2.jump();
					break;
				case sf::Keyboard::Key::Left:
					player2.moveLeft();
					break;
				case sf::Keyboard::Key::Right:
					player2.moveRight();
					break;
				case sf::Keyboard::Key::Numpad1:
					player2.punch();
					break;
				case sf::Keyboard::Key::Numpad2:
					player2.kick();
					break;
				case sf::Keyboard::Key::Down:
					player2.block();
					break;
				default:
					break;
			}
		}
		else if (const auto *keyReleased = event->getIf<sf::Event::KeyReleased>())
		{
			// Handle key releases for movement
			switch (keyReleased->code)
			{
				case sf::Keyboard::Key::A:
				case sf::Keyboard::Key::D:
					player1.stopMovingHorizontal();
					break;
				case sf::Keyboard::Key::Left:
				case sf::Keyboard::Key::Right:
					player2.stopMovingHorizontal();
					break;
				default:
					break;
			}
		}
	}
}

void Game::update(float deltaTime)
{
	player1.update(deltaTime);
	player2.update(deltaTime);
	healthBar1.update(player1.getHealth());
	healthBar2.update(player2.getHealth());
	checkCollisions();
}

void Game::render()
{
	window.clear(sf::Color::Black);
	
	player1.draw(window);
	player2.draw(window);
	healthBar1.draw(window);
	healthBar2.draw(window);

	window.display();
}

void Game::checkCollisions()
{
	// Example collision detection between player1 and player2
	if (player1.getSprite().getGlobalBounds().findIntersection(player2.getSprite().getGlobalBounds()).has_value())
	{
		// Handle collisions, maybe trigger attack
	}
}
