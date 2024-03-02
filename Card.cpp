#include "Card.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>

Card::Card()
:m_isFaceUp(false)
{
    if(!m_spriteSheetTexture.loadFromFile("./assets/card-deck.png"))
    {
        std::cerr<<"Error loading spritesheet!\n";
        return;
    } 
    
    m_backSprite.setTexture(m_spriteSheetTexture);
    m_backSprite.setTextureRect(sf::IntRect(CARD_WIDTH*2, CARD_HEIGHT * 4, CARD_WIDTH, CARD_HEIGHT)); 


    // return the card King of Diamond when it is faceUP
    m_frontSprite.setTexture(m_spriteSheetTexture);
    m_frontSprite.setTextureRect(sf::IntRect(CARD_WIDTH*(KING-1),CARD_HEIGHT*DIAMONDS, CARD_WIDTH, CARD_HEIGHT));

}

Card::~Card()
{

}
void Card::RandomInit()
{
    //TODO: Random Inititialize the front of the card
    srand(time(0));
    int randomY = rand() % 5;
    int randomX = rand() % 14;
    
    
    m_frontSprite.setTexture(m_spriteSheetTexture);
    m_frontSprite.setTextureRect(sf::IntRect(CARD_WIDTH*randomX, CARD_HEIGHT*randomY, CARD_WIDTH, CARD_HEIGHT));
    
    m_suit = randomY;
    m_rank = randomX;
}

void Card::setFaceDown()
{
    m_isFaceUp = false;
}

void Card::setFaceUp()
{
    m_isFaceUp = true;
}

void Card::flipCard()
{
    m_isFaceUp = !m_isFaceUp;
}

bool Card::getIsFaceUp()
{
    return m_isFaceUp;
}

int Card::getCardSuit()
{
    return m_suit;
}

int Card::getCardRank()
{
    return m_rank;
}

void Card::setCardFrontSprite(sf::IntRect srcRect)
{
    m_frontSprite.setTexture(m_spriteSheetTexture);
    m_frontSprite.setTextureRect(srcRect);
}

void Card::setCardPosition(float x, float y)
{
    m_backSprite.setPosition(x, y);
    m_frontSprite.setPosition(x, y);
}

sf::Sprite& Card::getCardSprite() 
{
    if(m_isFaceUp)
        return m_frontSprite;
    else
        return m_backSprite;
}
