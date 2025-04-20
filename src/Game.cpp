#include "Game.hpp"
#include <iostream>
#include <fstream>

void printKey(const sf::Event::KeyPressed *keyPressed)
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

Game::Game() : window(sf::VideoMode({800, 600}), "Fighting Game", sf::Style::Default),
			   player1(100, 400, true),
			   player2(600, 400, false),
			   healthBar1(window.getSize().x / 2 - 350, 10, player1.getHealth()),
			   healthBar2(window.getSize().x / 2 + 150, 10, player2.getHealth()),
			   backgroundTexture(),
			   backgroundSprite(backgroundTexture) // Initialize with empty texture
{
	std::cout << "Game constructor started" << std::endl;

	try
	{
		// Set window properties
		window.setFramerateLimit(60);
		std::cout << "Framerate set to 60" << std::endl;

		// Load background texture
		std::cout << "Attempting to load background texture..." << std::endl;
		if (!backgroundTexture.loadFromFile("assets/sprites/insbg.gif"))
		{
			std::cout << "ERROR: Failed to load background image!" << std::endl;

			// Create a fallback texture - solid color
			backgroundTexture.resize({800, 600});
			sf::Image img;
			img.resize({800, 600}, sf::Color(50, 50, 100));
			backgroundTexture.update(img);
			std::cout << "Created fallback texture" << std::endl;
		}
		else
		{
			std::cout << "SUCCESS: Background image loaded" << std::endl;
		}

		// Now set the texture to the sprite
		backgroundSprite.setTexture(backgroundTexture);

		// Scale the background to fit the window
		float scaleX = static_cast<float>(window.getSize().x) / backgroundTexture.getSize().x;
		float scaleY = static_cast<float>(window.getSize().y) / backgroundTexture.getSize().y;
		backgroundSprite.setScale({scaleX, scaleY});

		std::cout << "Background scaled: " << scaleX << " x " << scaleY << std::endl;
		std::cout << "Game constructor completed successfully" << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cerr << "EXCEPTION in Game constructor: " << e.what() << std::endl;
		throw; // Re-throw to be caught in main
	}
	catch (...)
	{
		std::cerr << "UNKNOWN EXCEPTION in Game constructor" << std::endl;
		throw; // Re-throw to be caught in main
	}
}

void Game::run()
{
	std::cout << "Game::run() started" << std::endl;

	try
	{
		sf::Clock clock;

		// Make sure the window is still open before entering the loop
		if (!window.isOpen())
		{
			std::cout << "ERROR: Window is not open at start of run()" << std::endl;
			return;
		}

		std::cout << "Entering main game loop" << std::endl;

		// Main game loop
		int frameCount = 0;
		while (window.isOpen())
		{
			if (frameCount % 60 == 0)
			{ // Log every 60 frames (roughly every second)
				std::cout << "Frame " << frameCount << std::endl;
			}

			processEvents();

			// Calculate actual deltaTime
			float deltaTime = clock.restart().asSeconds();

			// Call the update method
			update(deltaTime);

			// Render
			render();

			frameCount++;
		}

		std::cout << "Game loop ended after " << frameCount << " frames" << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cerr << "EXCEPTION in Game::run: " << e.what() << std::endl;
		throw; // Re-throw to be caught in main
	}
	catch (...)
	{
		std::cerr << "UNKNOWN EXCEPTION in Game::run" << std::endl;
		throw; // Re-throw to be caught in main
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

	// Draw background first
	window.draw(backgroundSprite);

	// Then draw other game elements
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

Game::~Game()
{
	// Make sure to clean up any SFML resources before exiting
	window.close();

	// Reset any static resources
	// This might help with the mutex issue
	AnimationManager::cleanup();
}
