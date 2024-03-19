#ifndef CARD_HPP
#define CARD_HPP

#include <SFML/Graphics.hpp>

#define CARD_WIDTH 98.5
#define CARD_HEIGHT 153

enum Color {BLACK, RED};
enum Suit {CLUBS, DIAMONDS, HEARTS, SPADES};
enum Rank {ACE=1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING};


class Card
{
    private:
        bool m_isFaceUp;
        bool m_isPickUp;
        bool m_isParent;
        bool m_isChild;
        bool m_isOnBuildPile;
        bool m_isOnDiscardPile;

        // const int CARD_WIDTH = 98;
        // const  int CARD_HEIGHT = 153;
        
        /*
        enum Suit {CLUBS, DIAMONDS, HEARTS, SPADES}; // y-coordinate in spritesheetTexture
        enum Rank {Ace=1,Two, Three, Four, Five, Six, Seven, Eight, 
                    Nine, Ten, Jack, Queen, King }; // x-coordinate in spritesheetTexture
        */
        int m_suit, m_rank, m_color;
        float m_xpos, m_ypos;
        float m_parentXPos, m_parentYPos;

        sf::Vector2f m_dragOffsetCard;
        sf::Texture m_spriteSheetTexture;
        sf::Sprite m_backSprite;
        sf::Sprite m_frontSprite;
        
    public: 
        Card();
        ~Card();
        bool getIsCardOnDiscardPile();
        void setIsCardOnDiscardPile(bool);
        void setFaceDown();
        void setFaceUp();
        void setCardColor(int color);
        int getCardColor();
        void setCardSuit(int suit);
        int getCardSuit();
        void setCardRank(int rank);
        int getCardRank();
        void cardProcessInput(sf::RenderWindow& window, sf::Event& event);
        void flipCard();
        bool getIsFaceUp();
        bool getIsPickUp();
        void setIsPickUp(bool pickup);
        bool getIsParent();
        void setIsParent(bool);
        void setIsChild(bool);
        bool getIsChild();
        void setIsOnBuildPile(bool);
        bool getIsOnBuildPile();
        void setCardFrontSprite(sf::IntRect srcRect);
        float getCardPositionX() const;
        float getCardPositionY() const;
        void setCardPosition(float x, float y);
        sf::Sprite& getCardSprite();

};

#endif
