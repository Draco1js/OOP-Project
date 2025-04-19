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
			   healthBar1(10, 10, player1.getHealth()), healthBar2(10, 50, player2.getHealth())
{
	window.setFramerateLimit(60);
	std::cout << "Game initialized. Window created with size: " << window.getSize().x << "x" << window.getSize().y << std::endl
			  << std::flush;
}

void Game::run()
{
	while (window.isOpen())
	{
		processEvents();
		update();
		render();
	}
}

void Game::processEvents()
{
	while (const auto event = window.pollEvent())
	{
		if (event->is<sf::Event::Closed>())
		{
			window.close();
		}
		// Debug keyboard events
		else if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>())
		{
			printKey(keyPressed);
		}
	}
}

void Game::update()
{
	player1.update();
	player2.update();
	healthBar1.update(player1.getHealth());
	healthBar2.update(player2.getHealth());
	checkCollisions();
}

void Game::render()
{
	window.clear(sf::Color::Black);

	// std::cout << "Rendering frame..." << std::endl;
	
	// // Debug: Print player positions before drawing
	// std::cout << "Player1 position before draw: " << player1.getSprite().getPosition().x << ", " 
	// 		  << player1.getSprite().getPosition().y << std::endl;
	// std::cout << "Player2 position before draw: " << player2.getSprite().getPosition().x << ", " 
	// 		  << player2.getSprite().getPosition().y << std::endl;

	player1.draw(window);
	player2.draw(window);
	healthBar1.draw(window);
	healthBar2.draw(window);

	window.display();
	std::cout << "Frame displayed" << std::endl << std::flush;
}

void Game::checkCollisions()
{
	// Example collision detection between player1 and player2
	if (player1.getSprite().getGlobalBounds().findIntersection(player2.getSprite().getGlobalBounds()).has_value())
	{
		// Handle collisions, maybe trigger attack
	}
}
