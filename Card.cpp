#include "Card.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>

Card::Card()
:m_isFaceUp(false)
{   
    // Get the spritesheet from file  and put it on the sf::Texture m_spriteSheetTexture
    if(!m_spriteSheetTexture.loadFromFile("./assets/card-deck.png"))
    {
        std::cerr<<"Error loading spritesheet!\n";
        return;
    } 
    
    m_backSprite.setTexture(m_spriteSheetTexture);
    m_backSprite.setTextureRect(sf::IntRect(CARD_WIDTH*2, CARD_HEIGHT * 4, CARD_WIDTH, CARD_HEIGHT)); 

    

}

Card::~Card()
{

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

void Card::setCardColor(int color)
{
    m_color = color;
}

int Card::getCardColor()
{
    return m_color;
}

void Card::setCardSuit(int suit)
{
    m_suit = suit;
}

int Card::getCardSuit()
{
    return m_suit;
}

void Card::setCardRank(int rank)
{
    m_rank = rank;
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
