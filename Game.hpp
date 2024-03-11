#ifndef GAME_HPP 
#define GAME_HPP 
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Card.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <vector>
#include <unordered_map>

class Game
{
    public:
        Game();
        ~Game();
        void CardInit();
        void ProcessInput(sf::RenderWindow &window, sf::Event event);
        void Update();
        void Render(sf::RenderWindow &window);
        void ResetButton();
        void PutCardOnTable();
        void CheckBuildPile();
        void CheckCardsOnTable();
        void CheckDiscardPile();
        void setIsDiscardPileEmpty(bool);
        bool getIsDiscardPileEmpty();
        

    private:
        bool m_isDragCard1;
        // m_isDragCard2, m_isDragCard3,
        // m_isDragCard4, m_isDragCard5, m_isDragCard6, m_isDragCard7, m_isDragDiscardPile;

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
        
        bool m_isDiscardPileEmpty;
        int BuildPile0_Index, BuildPile1_Index, BuildPile2_Index, BuildPile3_Index;

        // Discard Pile Position
        const int DISCARD_POSITION_X = 150;
        const int DISCARD_POSITION_Y = 20;
        
        // Draw Pile Card Position
        const int DRAW_PILE_POSX = 20;
        const int DRAW_PILE_POSY = 20;
        
        // Build Pile Card Position
        const int BUILD_PILE_POS_X = 550;
        const int BUILD_PILE_POS_Y = 20;
        const int BUILD_PILE_OFFSET_X = 150;
        
        // Position for the Card on the Table
        const int TABLE_COL_POS_X = 20;
        const int TABLE_COL_POS_Y = 200;
        const int TABLE_OFFSET_POS_X = 130;
        const int TABLE_OFFSET_POS_Y = 40;
        const int TABLE_NUM_COL = 7;
        const int TABLE_NUM_CARDS = 28; // there are 28 cards on table but index start with zero
        

        sf::Texture m_pilesTexture;
        sf::Texture m_resetTexture;
        sf::Sprite m_pilesSprite;
        sf::Sprite m_resetButton;
        sf::Vector2f m_dragOffsetCard;


        std::unordered_map<int, sf::Sprite> m_BuildPilesMap;
        
        Card m_card;
        std::vector<Card> myDeck,tempDeck, BuildPile0, BuildPile1, BuildPile2, BuildPile3;
        std::vector<Card> DiscardCard, CardsOnTable;
        
        
        
};
#endif
