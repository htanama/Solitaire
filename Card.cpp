#include "Card.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <iostream>

Card::Card()
:m_isFaceUp(false),m_isParent(false), m_isPickUp(false), m_isChild(false), m_isOnBuildPile(false)
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

bool Card::getIsGetPickUp()
{
    return m_isPickUp;
}

bool Card::getIsParent() 
{
    return m_isParent;
}

void Card::setIsParent(bool isParent)
{
    m_isParent = isParent;
}

void Card::setIsChild(bool isChild)
{
    m_isChild = isChild;
}

bool Card::getIsChild()
{
    return m_isChild;
}

void Card::setIsOnBuildPile(bool isOnBuildPile)
{
    m_isOnBuildPile = isOnBuildPile;
}

bool Card::getIsOnBuildPile()
{
    return m_isOnBuildPile;
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
    m_xpos = x;
    m_ypos = y; 
    
    m_backSprite.setPosition(m_xpos, m_ypos);
    m_frontSprite.setPosition(m_xpos, m_ypos);
}

float Card::getCardPositionX() const
{
    return m_xpos;    
}

float Card::getCardPositionY() const
{
    return m_ypos;
}


void Card::cardProcessInput(sf::RenderWindow& window, sf::Event &event)
{
    // Get the size of the window
    sf::Vector2u windowSize = window.getSize();

    // Convert the window size to float
    sf::Vector2f windowSizeF(static_cast<float>(windowSize.x), static_cast<float>(windowSize.y));

    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    
    // get the rectangle boundary of m_frontSprite (sf::Sprite) 
    sf::FloatRect rectBoundsFrontCard = m_frontSprite.getGlobalBounds();    

    // check whether the moues position is within the bounds of the rectangle Card m_frontSprite
    if(rectBoundsFrontCard.contains(static_cast<sf::Vector2f>(mousePos))){ 
        if(event.type == sf::Event::MouseButtonPressed && getIsFaceUp() == true){
            // check if mouse click inside the card
            if(m_frontSprite.getGlobalBounds().contains(mousePos)){
                m_isPickUp = !m_isPickUp;
            
                // Calculate the offset from the top-left corner of the card 
                m_dragOffsetCard = mousePos - m_frontSprite.getPosition();
            }    
            
        }
    }
    if(event.type == sf::Event::MouseButtonReleased){ 
        if(event.mouseButton.button == sf::Mouse::Left){
            m_isPickUp = false;
        }
    }





    if(m_isPickUp == true && m_isOnBuildPile == false){
        // std::cout<<"Card is pickUp"<<std::endl;
        // get the mouse position relative to the window.  
        mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window)) - m_dragOffsetCard;

        // Check if the mouse position is outside the window bounds and adjust it if necessary
        if (mousePos.x < 0)
            mousePos.x = 0;
        else if (mousePos.x > windowSizeF.x)
            mousePos.x = windowSizeF.x;

        if (mousePos.y < 0)
            mousePos.y = 0;
        else if (mousePos.y > windowSizeF.y)
            mousePos.y = windowSizeF.y;

        if (mousePos.x > windowSizeF.x - rectBoundsFrontCard.width)
            mousePos.x = windowSizeF.x - rectBoundsFrontCard.width;

        if (mousePos.y > windowSizeF.y - rectBoundsFrontCard.height)
            mousePos.y = windowSizeF.y - rectBoundsFrontCard.height;

        setCardPosition((float)mousePos.x, (float)mousePos.y);     
    }

}

sf::Sprite& Card::getCardSprite() 
{
    if(m_isFaceUp)
        return m_frontSprite;
    else
        return m_backSprite;
}
