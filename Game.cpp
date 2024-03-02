#include "Game.hpp"
#include "Card.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Window.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <algorithm>
#include <random>


Game::Game():myDeck(52),tempDeck(52), m_isDragCard(false)
{
    if(!m_pilesTexture.loadFromFile("./assets/FinalRectangle.png")){
        std::cerr<<"Error Loading FinalRectangle.png!\n";
    }
    m_pilesSprite.setTexture(m_pilesTexture);
    
    for(int i = 0; i < 5; i++){
        m_BuildPilesMap[i] = m_pilesSprite;
    }
    m_BuildPilesMap[0].setPosition(BUILD_PILE_POS_X + (BUILD_PILE_OFFSET_X * 0), BUILD_PILE_POS_Y); 
    m_BuildPilesMap[1].setPosition(BUILD_PILE_POS_X + (BUILD_PILE_OFFSET_X * 1), BUILD_PILE_POS_Y); 
    m_BuildPilesMap[2].setPosition(BUILD_PILE_POS_X + (BUILD_PILE_OFFSET_X * 2), BUILD_PILE_POS_Y); 
    m_BuildPilesMap[3].setPosition(BUILD_PILE_POS_X + (BUILD_PILE_OFFSET_X * 3), BUILD_PILE_POS_Y); 

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
    
    // Assigning each card to texture in ordered
    for(int i = 0; i < 52; ++i){
        tempDeck[i].setCardFrontSprite(sf::IntRect((i%13)*CARD_WIDTH, (i/13)*CARD_HEIGHT, CARD_WIDTH, CARD_HEIGHT));
    }
    
    // Create a random device
    std::random_device rd;
    // Initialize a generator
    std:;std::mt19937 g(rd());

    //Suffle the array
    std::shuffle(std::begin(m_cardIndex), std::end(m_cardIndex), g);
    
    for(int i = 0; i < 52; i++){     
        myDeck[i] = tempDeck[m_cardIndex[i]];
    }
    
  
    // myDeck = tempDeck;
      
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

    PutCardOnTable();
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
        // when mouse button is pressed drag the card.
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
        myDeck[0].getCardSprite().setPosition(BUILD_PILE_POS_X + (BUILD_PILE_OFFSET_X * 0), BUILD_PILE_POS_Y);
        m_isDragCard = false;
        myDeck[13].getCardSprite().setPosition(BUILD_PILE_POS_X + (BUILD_PILE_OFFSET_X * 1), BUILD_PILE_POS_Y);
        myDeck[26].getCardSprite().setPosition(BUILD_PILE_POS_X + (BUILD_PILE_OFFSET_X * 2), BUILD_PILE_POS_Y);
        myDeck[39].getCardSprite().setPosition(BUILD_PILE_POS_X + (BUILD_PILE_OFFSET_X * 3), BUILD_PILE_POS_Y);

        for(int i = 1; i < 13; i++){
            myDeck[i].getCardSprite().setPosition(DRAW_PILE_POSX, DRAW_PILE_POSY);
            myDeck[i].setFaceDown();
        }
    }
}

void Game::PutCardOnTable()
{
    // Table Col one, one card, flip open. 
    myDeck[0].setFaceUp();
    myDeck[0].getCardSprite().setPosition(TABLE_COL_POS_X, TABLE_COL_POS_Y);

    // Table Col two, two cards, one closed, one flip open.
    myDeck[25].setFaceDown();
    myDeck[24].setFaceUp();
    myDeck[25].getCardSprite().setPosition(TABLE_COL_POS_X + (TABLE_OFFSET_POS_X * 0), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 0));
    myDeck[24].getCardSprite().setPosition(150, 230);

    // Table Col three, three cards, all closed, one at the bottom flip open.
    myDeck[23].setFaceDown(); 
    myDeck[22].setFaceDown(); 
    myDeck[21].setFaceUp();
    
    myDeck[23].getCardSprite().setPosition(280, 200);
    myDeck[22].getCardSprite().setPosition(280, 230);
    myDeck[21].getCardSprite().setPosition(280, 260);

    //Table Col 4, 4 cards, all closed, one at the bottom flip open. 
    myDeck[20].setFaceDown(); 
    myDeck[19].setFaceDown(); 
    myDeck[18].setFaceDown();
    myDeck[17].setFaceUp();

    myDeck[20].getCardSprite().setPosition(TABLE_COL_POS_X + (3*TABLE_OFFSET_POS_X), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 0));
    myDeck[19].getCardSprite().setPosition(TABLE_COL_POS_X + (3*TABLE_OFFSET_POS_X), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 1)); 
    myDeck[18].getCardSprite().setPosition(TABLE_COL_POS_X + (3*TABLE_OFFSET_POS_X), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 2)); 
    myDeck[17].getCardSprite().setPosition(TABLE_COL_POS_X + (3*TABLE_OFFSET_POS_X), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 3)); 

    //Table Col 5, 5 cards, all closed, one at the bottom flip open. 
    myDeck[16].setFaceDown(); 
    myDeck[15].setFaceDown(); 
    myDeck[14].setFaceDown();
    myDeck[13].setFaceDown();
    myDeck[12].setFaceUp();

    myDeck[16].getCardSprite().setPosition(TABLE_COL_POS_X + (4*TABLE_OFFSET_POS_X), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 0));
    myDeck[15].getCardSprite().setPosition(TABLE_COL_POS_X + (4*TABLE_OFFSET_POS_X), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 1)); 
    myDeck[14].getCardSprite().setPosition(TABLE_COL_POS_X + (4*TABLE_OFFSET_POS_X), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 2));
    myDeck[13].getCardSprite().setPosition(TABLE_COL_POS_X + (4*TABLE_OFFSET_POS_X), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 3)); 
    myDeck[12].getCardSprite().setPosition(TABLE_COL_POS_X + (4*TABLE_OFFSET_POS_X), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 4)); 

    //Table Col 6, 6 cards, all closed, one at the bottom flip open. 
    myDeck[11].setFaceDown(); 
    myDeck[10].setFaceDown(); 
    myDeck[9].setFaceDown();
    myDeck[8].setFaceDown();
    myDeck[7].setFaceDown(); 
    myDeck[6].setFaceUp();

    myDeck[11].getCardSprite().setPosition(TABLE_COL_POS_X + (5*TABLE_OFFSET_POS_X), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 0));
    myDeck[10].getCardSprite().setPosition(TABLE_COL_POS_X + (5*TABLE_OFFSET_POS_X), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 1)); 
    myDeck[9].getCardSprite().setPosition(TABLE_COL_POS_X + (5*TABLE_OFFSET_POS_X), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 2));
    myDeck[8].getCardSprite().setPosition(TABLE_COL_POS_X + (5*TABLE_OFFSET_POS_X), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 3)); 
    myDeck[7].getCardSprite().setPosition(TABLE_COL_POS_X + (5*TABLE_OFFSET_POS_X), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 4)); 
    myDeck[6].getCardSprite().setPosition(TABLE_COL_POS_X + (5*TABLE_OFFSET_POS_X), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 5)); 

    //Table Col 7, 7 cards, all closed, one at the bottom flip open. 
    myDeck[32].setFaceDown(); 
    myDeck[31].setFaceDown(); 
    myDeck[30].setFaceDown();
    myDeck[29].setFaceDown();
    myDeck[28].setFaceDown(); 
    myDeck[27].setFaceDown();
    myDeck[26].setFaceUp();

    myDeck[32].getCardSprite().setPosition(TABLE_COL_POS_X + (6*TABLE_OFFSET_POS_X), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 0));
    myDeck[31].getCardSprite().setPosition(TABLE_COL_POS_X + (6*TABLE_OFFSET_POS_X), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 1)); 
    myDeck[30].getCardSprite().setPosition(TABLE_COL_POS_X + (6*TABLE_OFFSET_POS_X), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 2));
    myDeck[29].getCardSprite().setPosition(TABLE_COL_POS_X + (6*TABLE_OFFSET_POS_X), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 3)); 
    myDeck[28].getCardSprite().setPosition(TABLE_COL_POS_X + (6*TABLE_OFFSET_POS_X), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 4)); 
    myDeck[27].getCardSprite().setPosition(TABLE_COL_POS_X + (6*TABLE_OFFSET_POS_X), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 5)); 
    myDeck[26].getCardSprite().setPosition(TABLE_COL_POS_X + (6*TABLE_OFFSET_POS_X), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 6)); 





}

void Game::Render(sf::RenderWindow &window)
{
    for(int i = 0; i < 4; i++){
        window.draw(m_BuildPilesMap[i]);
    }
    // Discard Pile Next to the Draw Pile
    window.draw(m_BuildPilesMap[4]); 

    // Draw Pile
    window.draw(myDeck[51].getCardSprite());

    // Draw Card on the Table Col Two 
    window.draw(myDeck[25].getCardSprite());
    window.draw(myDeck[24].getCardSprite());  

    // Draw Card on the Table Col Three
    window.draw(myDeck[23].getCardSprite());
    window.draw(myDeck[22].getCardSprite());
    window.draw(myDeck[21].getCardSprite());
    
    // Draw Card on the Table Col Four 
    window.draw(myDeck[20].getCardSprite());
    window.draw(myDeck[19].getCardSprite());
    window.draw(myDeck[18].getCardSprite());  
    window.draw(myDeck[17].getCardSprite());  

    // Draw Card on the Table Col Five 
    window.draw(myDeck[16].getCardSprite());
    window.draw(myDeck[15].getCardSprite());
    window.draw(myDeck[14].getCardSprite());  
    window.draw(myDeck[13].getCardSprite());  
    window.draw(myDeck[12].getCardSprite());  
   
    // Draw Card on the Table Col Six 
    window.draw(myDeck[11].getCardSprite());
    window.draw(myDeck[10].getCardSprite());
    window.draw(myDeck[9].getCardSprite());  
    window.draw(myDeck[8].getCardSprite());  
    window.draw(myDeck[7].getCardSprite());  
    window.draw(myDeck[6].getCardSprite());  
    
    // Draw Card on the Table Col Seven 
    window.draw(myDeck[32].getCardSprite());
    window.draw(myDeck[31].getCardSprite());
    window.draw(myDeck[30].getCardSprite());  
    window.draw(myDeck[29].getCardSprite());  
    window.draw(myDeck[28].getCardSprite());  
    window.draw(myDeck[27].getCardSprite());  
    window.draw(myDeck[26].getCardSprite());  
 
   
    /* example card rendering
    for(int i = 38; i >= 26; i--){
        window.draw(myDeck[i].getCardSprite());
    }*/
    
    // Table Col One
    window.draw(myDeck[0].getCardSprite());
    
}
