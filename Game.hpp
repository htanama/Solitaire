#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Card.hpp"
#include <vector>
#include <unordered_map>

class Game
{
    public:
        Game();
        ~Game();
        void ProcessInput(sf::RenderWindow &window, sf::Event event);
        void Update();
        void Render(sf::RenderWindow &window);

    private:
        bool m_isDragCard;

        sf::Texture m_pilesTexture;
        sf::Sprite m_pilesSprite;
        sf::Vector2f m_dragOffsetCard;


        std::unordered_map<int, sf::Sprite> m_BuildPilesMap;
        Card m_card;
        std::vector<Card> myDeck;
        
};
#endif
