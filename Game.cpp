#include "Game.hpp"
#include "Card.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Window.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>

Game::Game():myDeck(52),m_isDragCard(false)
{
    if(!m_pilesTexture.loadFromFile("./assets/FinalRectangle.png")){
        std::cerr<<"Error Loading FinalRectangle.png!\n";
    }
    m_pilesSprite.setTexture(m_pilesTexture);
    
    for(int i = 0; i < 5; i++){
        m_BuildPilesMap[i] = m_pilesSprite;
    }
     m_BuildPilesMap[0].setPosition(BUILD_PILE_POS_X1, BUILD_PILE_POS_Y); 
     m_BuildPilesMap[1].setPosition(BUILD_PILE_POS_X2, BUILD_PILE_POS_Y); 
     m_BuildPilesMap[2].setPosition(BUILD_PILE_POS_X3, BUILD_PILE_POS_Y); 
     m_BuildPilesMap[3].setPosition(BUILD_PILE_POS_X4, BUILD_PILE_POS_Y); 

    // Discard Draw Pile
    m_BuildPilesMap[4].setPosition(150, 20);

    
    /* testing units
    sf::IntRect srcRect1(CARD_WIDTH*ACE, CARD_HEIGHT*CLUBS, CARD_WIDTH, CARD_HEIGHT);
    myDeck[0].setCardFrontSprite(srcRect1);
    myDeck[0].setFaceUp();
    myDeck[0].setCardPosition(CARD_WIDTH*1, CARD_HEIGHT*0);
    sf::IntRect srcRect2(CARD_WIDTH*TWO, CARD_HEIGHT*HEARTS, CARD_WIDTH, CARD_HEIGHT);
    myDeck[1].setCardFrontSprite(srcRect2);    
    myDeck[1].setFaceUp();
    myDeck[1].setCardPosition(200, 0);
    */
    
    // Assigning each card to texture
    for(int i = 0; i < 52; ++i){
        myDeck[i].setCardFrontSprite(sf::IntRect((i%13)*CARD_WIDTH, (i/13)*CARD_HEIGHT, CARD_WIDTH, CARD_HEIGHT));

    }
    
    /*
    myDeck[51].setCardPosition(10, 10);

    myDeck[26].setFaceUp();
    myDeck[38].setFaceUp();
    myDeck[11].setFaceUp();


    myDeck[26].setCardPosition(200,200);
    myDeck[38].setCardPosition(200,250);
    myDeck[11].setCardPosition(200,300);
    */
    int row = 0;
    for(int i = 0; i < 52; i++){
        myDeck[i].setFaceUp();
    }
   
    for(int i = 12; i >= 0; i--){
        myDeck[i].setCardPosition(200,300 + row);
        row += 30;
    }
    
    row = 0;    
    for(int i = 25; i >= 13; i--){
        myDeck[i].setCardPosition(350, 300 + row);
        row += 30;
    }

    row = 0;
    for(int i = 38; i >= 26; i--){
        myDeck[i].setCardPosition(500, 300 + row);
        row += 30;
    }
    
    row = 0;
    for(int i = 50; i >= 39; i--){
        myDeck[i].setCardPosition(650, 300 + row);
        row += 30;
    }
    
    myDeck[51].setFaceDown();
    myDeck[51].setCardPosition(DRAW_PILE_POSX, DRAW_PILE_POSY);
}

Game::~Game()
{

}

void Game::ProcessInput(sf::RenderWindow &window, sf::Event event)
{   
    while(window.pollEvent(event)){
       if(event.type == sf::Event::Closed)
            window.close();
    
        if(event.type == sf::Event::MouseButtonPressed){        
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            sf::FloatRect rectBounds = myDeck[51].getCardSprite().getGlobalBounds();
        
            for(int i = 1; i < 12; i++){
                sf::FloatRect rectBounds = myDeck[i].getCardSprite().getGlobalBounds();
                if(rectBounds.contains(static_cast<sf::Vector2f>(mousePos))){
                            myDeck[i].setFaceUp();
                            myDeck[i].getCardSprite().setPosition(DISCARD_POSITION_X, DISCARD_POSITION_Y);
                       }
            }
            
            if(rectBounds.contains(static_cast<sf::Vector2f>(mousePos))){
                //check if mouse click inside the card  
                if(myDeck[51].getCardSprite().getGlobalBounds().contains(mousePos)){
                    myDeck[51].flipCard();
                    myDeck[51].getCardSprite().setPosition(DISCARD_POSITION_X, DISCARD_POSITION_Y);       
                    // calculate the offset from the top-left corner of the card
                    m_dragOffsetCard = mousePos - myDeck[51].getCardSprite().getPosition();   
                }

            }
        }

        if(event.type == sf::Event::MouseButtonPressed){
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            sf::FloatRect rectBounds = myDeck[0].getCardSprite().getGlobalBounds();

            if(rectBounds.contains(static_cast<sf::Vector2f>(mousePos))){
                // check if mouse click inside the card
                if(myDeck[0].getCardSprite().getGlobalBounds().contains(mousePos)){
                    m_isDragCard = true;
                    // Calculate the offset from the top-left corner of the card
                    m_dragOffsetCard = mousePos - myDeck[0].getCardSprite().getPosition();
                }
            }

        }
        if(event.type == sf::Event::MouseButtonReleased){
            if(event.mouseButton.button == sf::Mouse::Left){
                m_isDragCard = false;
            }
        }

        //mapPixelToCoords(pixelPos) is convert to world coordinates
        if(m_isDragCard){
            myDeck[0].getCardSprite().setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)) - m_dragOffsetCard);
        }        
        
           
    }
}

void Game::Update()
{
    // if Build_Pile is empty, then Ace needs to be the first in the stack.
    if(myDeck[0].getCardSprite().getGlobalBounds().intersects(m_BuildPilesMap[0].getGlobalBounds())){
        myDeck[0].getCardSprite().setPosition(BUILD_PILE_POS_X1, BUILD_PILE_POS_Y);
        m_isDragCard = false;
        myDeck[13].getCardSprite().setPosition(BUILD_PILE_POS_X2, BUILD_PILE_POS_Y);
        myDeck[26].getCardSprite().setPosition(BUILD_PILE_POS_X3, BUILD_PILE_POS_Y);
        myDeck[39].getCardSprite().setPosition(BUILD_PILE_POS_X4, BUILD_PILE_POS_Y);

        for(int i = 1; i < 13; i++){
            myDeck[i].getCardSprite().setPosition(DRAW_PILE_POSX, DRAW_PILE_POSY);
            myDeck[i].setFaceDown();
        }
    }
}

void Game::Render(sf::RenderWindow &window)
{
    window.draw(m_BuildPilesMap[4]);

    for(int i = 0; i < 4; i++){
        window.draw(m_BuildPilesMap[i]);
    }

    // window.draw(m_card.getCardSprite());
    window.draw(myDeck[51].getCardSprite());

    /*
    window.draw(myDeck[26].getCardSprite());
    window.draw(myDeck[38].getCardSprite());
    window.draw(myDeck[11].getCardSprite());
    */
    
    for(int i = 12; i >= 0; i--){
        window.draw(myDeck[i].getCardSprite());
    }

    for(int i = 25; i >= 13; i--){
        window.draw(myDeck[i].getCardSprite());
    }

    for(int i = 38; i >= 26; i--){
        window.draw(myDeck[i].getCardSprite());
    }

    for (int i = 50; i >= 39; i--){
        window.draw(myDeck[i].getCardSprite());
    }
}
