#include "Game.hpp"
#include "Card.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <iostream>

Game::Game():myDeck(52)
{
    if(!m_pilesTexture.loadFromFile("./assets/FinalRectangle.png")){
        std::cerr<<"Error Loading FinalRectangle.png!\n";
    }
    m_pilesSprite.setTexture(m_pilesTexture);
    
    int offsetX = 150;
    for(int i = 0; i < 5; i++){
        m_BuildPilesMap[i] = m_pilesSprite;
        m_BuildPilesMap[i].setPosition(300+offsetX, 20);
        offsetX += 150;
    }

    m_BuildPilesMap[4].setPosition(150, 20);

    // Assigning each card to texture
    /* 
    sf::IntRect srcRect1(CARD_WIDTH*ACE, CARD_HEIGHT*CLUBS, CARD_WIDTH, CARD_HEIGHT);
    myDeck[0].setCardFrontSprite(srcRect1);
    myDeck[0].setFaceUp();
    myDeck[0].setCardPosition(CARD_WIDTH*1, CARD_HEIGHT*0);
    
    
    sf::IntRect srcRect2(CARD_WIDTH*TWO, CARD_HEIGHT*HEARTS, CARD_WIDTH, CARD_HEIGHT);
    myDeck[1].setCardFrontSprite(srcRect2);    
    myDeck[1].setFaceUp();
    myDeck[1].setCardPosition(200, 0);
    */

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
    for(int i = 26; i >= 13; i--){
        myDeck[i].setCardPosition(300, 300 + row);
        row += 30;
    }

    row = 0;
    for(int i = 39; i >= 27; i--){
        myDeck[i].setCardPosition(400, 300 + row);
        row += 30;
    }

    for(int i = 39; i < 52; i++){
        myDeck[i].setCardPosition(20*i, 600);
    }
    
    myDeck[51].setFaceDown();
    myDeck[51].setCardPosition(20, 20);
}

Game::~Game()
{

}

void Game::ProcessInput(sf::RenderWindow &window, sf::Event event)
{
    while(window.pollEvent(event)){
        if(event.type == sf::Event::Closed)
            window.close();
    }
}

void Game::Update()
{

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

    for(int i = 0; i < 52; i++){
        window.draw(myDeck[i].getCardSprite());
    }
    
    for(int i = 12; i >= 0; i--){
        window.draw(myDeck[i].getCardSprite());
    }

    for(int i = 26; i >= 13; i--){
        window.draw(myDeck[i].getCardSprite());
    }

    for(int i = 39; i >= 27; i--){
    
    }
}
