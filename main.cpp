#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include "Game.hpp"


int main()
{
    // Create a new render window
    sf::RenderWindow window(sf::VideoMode(1200, 900), "SFML window", sf::Style::Close);
    sf::Event event;
    Game game;

    while (window.isOpen()) {
        game.ProcessInput(window, event);

        // Clear the window with black color
        window.clear(sf::Color(0, 102, 0));

        // Draw the sprite
        game.Render(window);

        // End the current frame and display its contents on screen
        window.display();
    }

    return 0;
}

