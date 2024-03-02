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

        const int DISCARD_POSITION_X = 150;
        const int DISCARD_POSITION_Y = 20;
        const int DRAW_PILE_POSX = 20;
        const int DRAW_PILE_POSY = 20;
        const int BUILD_PILE_POS_X1 = 400;
        const int BUILD_PILE_POS_X2 = 550;
        const int BUILD_PILE_POS_X3 = 700;
        const int BUILD_PILE_POS_X4 = 850;
        const int BUILD_PILE_POS_Y = 20;

        sf::Texture m_pilesTexture;
        sf::Sprite m_pilesSprite;
        sf::Vector2f m_dragOffsetCard;


        std::unordered_map<int, sf::Sprite> m_BuildPilesMap;
        Card m_card;
        std::vector<Card> myDeck, cardsOnTable;
        
        
        
};
#endif
