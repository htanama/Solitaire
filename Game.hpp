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
        
        void PutCardOnTable();
        void CheckBuildPile();
        void CheckTableCol();


    private:
        bool m_isDragCard;
        int m_cardIndex[52] = {
            1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 
            11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 
            21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
            31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
            41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 
            51, 52
        };

        const int DISCARD_POSITION_X = 150;
        const int DISCARD_POSITION_Y = 20;
        const int DRAW_PILE_POSX = 20;
        const int DRAW_PILE_POSY = 20;
        const int BUILD_PILE_POS_X = 550;
        const int BUILD_PILE_POS_Y = 20;
        const int BUILD_PILE_OFFSET_X = 150;
        const int TABLE_COL_POS_X = 20;
        const int TABLE_COL_POS_Y = 200;
        const int TABLE_OFFSET_POS_X = 130;
        const int TABLE_OFFSET_POS_Y = 30;

        sf::Texture m_pilesTexture;
        sf::Sprite m_pilesSprite;
        sf::Vector2f m_dragOffsetCard;


        std::unordered_map<int, sf::Sprite> m_BuildPilesMap;
        Card m_card;
        std::vector<Card> myDeck, tempDeck;
        
        
        
};
#endif
