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
        bool m_isDragCard1, m_isDragCard2, m_isDragCard3,
             m_isDragCard4, m_isDragCard5, m_isDragCard6, m_isDragCard7, m_isDragDiscardPile;
        
        int m_tempIndex[52] = {42, 51, 10, 32, 5, 21, 25, 28, 11, 9, 45, 13, 16, 26, 
            30, 37, 22, 52, 7, 35, 44, 8, 48, 50, 38, 46, 34, 1, 3, 43, 12, 18, 15, 14,
            39, 41, 20, 24, 33, 29, 6, 27, 23, 2, 47, 4, 31, 36, 17, 40, 19, 49,
        }; // missing Ace of Clubs 

        int m_tempIndex2[52] = {45, 1, 40, 7, 25, 24, 5, 51, 30, 27, 52, 20, 48, 28, 10,
            43, 41, 6, 2, 37, 3, 38, 35, 16, 17, 19, 36, 11, 31, 14, 46, 26, 50, 18, 47, 
            4, 44, 32, 13, 12, 9, 42, 29, 23, 33, 39, 49, 34, 8, 15, 22, 21}; 
            // missing Ace of Clubs because of index 0;

        int m_cardIndex[52] = {
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 
            11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 
            21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
            31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
            41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 
            51
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
