#ifndef CARD_HPP
#define CARD_HPP

#include <SFML/Graphics.hpp>

#define CARD_WIDTH 98.5
#define CARD_HEIGHT 153

enum Suit {CLUBS, DIAMONDS, HEARTS, SPADES};
enum Rank {ACE=1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING};


class Card
{
    private:
        bool m_isFaceUp;
        // const int CARD_WIDTH = 98;
        // const  int CARD_HEIGHT = 153;
        
        /*
        enum Suit {CLUBS, DIAMONDS, HEARTS, SPADES}; // y-coordinate in spritesheetTexture
        enum Rank {Ace=1,Two, Three, Four, Five, Six, Seven, Eight, 
                    Nine, Ten, Jack, Queen, King }; // x-coordinate in spritesheetTexture
        */

        int m_suit, m_rank;

        sf::Texture m_spriteSheetTexture;
        sf::Sprite m_backSprite;
        sf::Sprite m_frontSprite;
    public: 
        Card();
        ~Card();
        void RandomInit();
        void setFaceDown();
        void setFaceUp();
        int getCardSuit();
        int getCardRank();
        void flipCard();
        bool getIsFaceUp();
        void setCardFrontSprite(sf::IntRect srcRect);
        void setCardPosition(float x, float y);
        sf::Sprite getCardSprite() const;

};

#endif
