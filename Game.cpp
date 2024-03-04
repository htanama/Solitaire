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


Game::Game():myDeck(52),tempDeck(52), m_isDragCard1(false),m_isDragDiscardPile(false),
m_isDragCard2(false), m_isDragCard3(false), m_isDragCard4(false), m_isDragCard6(false) 
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
    int rank = 1;
    int suit = 0;

    // Assigning each card to texture in ordered
    for(int i = 0; i < 52; ++i){
        
        if(suit >= 4) suit = 0;
        if(rank >= 14) rank = 1;
       
        tempDeck[i].setCardFrontSprite(sf::IntRect((i%13)*CARD_WIDTH, (i/13)*CARD_HEIGHT, CARD_WIDTH, CARD_HEIGHT));
       
        tempDeck[i].setCardRank(rank);
        tempDeck[i].setCardSuit(suit);   
        
        if(suit == 1)
            tempDeck[i].setCardSuit(1); 
        if(suit == 2)
            tempDeck[i].setCardColor(1);
        if(suit == 0)
            tempDeck[i].setCardColor(0);
        if(suit == 3)
            tempDeck[i].setCardColor(0);

        ++rank;
        if(i == 13) ++suit;
        if(i == 26) ++suit;
        if(i == 39) ++suit;
        
        /*/ FOR TESTING
        tempDeck[i].setFaceUp();
        tempDeck[i].getCardSprite().setPosition(20+(20*i), 200);
        myDeck[i].setFaceUp();
        myDeck[i].getCardSprite().setPosition(20+(20*i), 400);
        */
   
    }
    
      
    // Create a random device
    std::random_device rd;
    // Initialize a generator
    std::mt19937 g(rd());

    //Suffle the array
    std::shuffle(std::begin(m_cardIndex), std::end(m_cardIndex), g);
    
    for(int i = 0; i < 52; i++){     
        myDeck[i] = tempDeck[m_cardIndex[i]];
        //myDeck[i] = tempDeck[i];
        std::cout<< m_cardIndex[i] <<", ";
 
        // for testing
        //myDeck[i].setFaceUp();
        //myDeck[i].getCardSprite().setPosition(20+(20*i), 400);
 
    }
    std::cout<<std::endl;


    // We put 28 cards on the tables
    PutCardOnTable();

    // We put the remaining cards on the Draw Pile Face Down
    for(int i = 28; i < 52; i ++){
        myDeck[i].setFaceDown();
        myDeck[i].setCardPosition(DRAW_PILE_POSX, DRAW_PILE_POSY);
    }
    // Top of Draw Pile is myDeck[51].setFaceUp();
   

}

Game::~Game()
{

}

void Game::ProcessInput(sf::RenderWindow &window, sf::Event event)
{   
    sf::Vector2f tempPosition(DISCARD_POSITION_X, DISCARD_POSITION_Y);

    while(window.pollEvent(event)){
        if(event.type == sf::Event::Closed)
            window.close();
 
       
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        
        //When the mouse is cliced on Draw Pile, it will move to Discard Pile with card facing up
        if(event.type == sf::Event::MouseButtonPressed){        
            sf::FloatRect rectBounds = myDeck[51].getCardSprite().getGlobalBounds();
           
            // checking whether the mouse position is within the bounds of the rectangle myDeck[__]. 
            if(rectBounds.contains(static_cast<sf::Vector2f>(mousePos))){
                //check if mouse click inside the card            
                if(myDeck[51].getCardSprite().getGlobalBounds().contains(mousePos)){
                    myDeck[51].setFaceUp();
                    myDeck[51].getCardSprite().setPosition(DISCARD_POSITION_X, DISCARD_POSITION_Y);
                }
            }
        }
       
        //When a mouse is click on the Discard Pile, you can drag the Card to put on the Table.
        if(event.type == sf::Event::MouseButtonPressed){
            sf::FloatRect rectBounds = myDeck[51].getCardSprite().getGlobalBounds();

            // checking whether the mouse position is within the bounds of the rectangle myDeck[__]. 
            if(rectBounds.contains(static_cast<sf::Vector2f>(mousePos))){
                // check if mouse click inside the card
                if(myDeck[51].getCardSprite().getGlobalBounds().contains(mousePos)){
                    m_isDragDiscardPile = !m_isDragDiscardPile;
                    // Calculate the offset from the top-left corner of the card
                    m_dragOffsetCard = mousePos - myDeck[51].getCardSprite().getPosition();
                }
            }
        }
        if(event.type == sf::Event::MouseButtonReleased){
            if(event.mouseButton.button == sf::Mouse::Left){
                m_isDragDiscardPile = false;           
                //TODO need to find the correct position for myDeck[51]
        //        myDeck[].getCardSprite().setPosition(TABLE_COL_POS_X + (6*TABLE_OFFSET_POS_X), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 6)); 
            }
        }
        //mapPixelToCoords(pixelPos) is convert to world coordinates
        if(m_isDragDiscardPile){
            myDeck[51].getCardSprite().setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)) - m_dragOffsetCard);
        }        


        // when mouse button is pressed drag the card at Table Col 1.
        if(event.type == sf::Event::MouseButtonPressed){
            sf::FloatRect rectBounds = myDeck[0].getCardSprite().getGlobalBounds();

            // checking whether the mouse position is within the bounds of the rectangle myDeck[0].
            if(rectBounds.contains(static_cast<sf::Vector2f>(mousePos))){
                // check if mouse click inside the card
                if(myDeck[0].getCardSprite().getGlobalBounds().contains(mousePos)){
                    m_isDragCard1 = true;
                    // Calculate the offset from the top-left corner of the card
                    m_dragOffsetCard = mousePos - myDeck[0].getCardSprite().getPosition();
                }
            }

        }
        if(event.type == sf::Event::MouseButtonReleased){
            if(event.mouseButton.button == sf::Mouse::Left){

                // if Build_Pile is empty, then Ace needs to be the first in the stack.
                if(myDeck[0].getCardSprite().getGlobalBounds().intersects(m_BuildPilesMap[0].getGlobalBounds()) &&
                myDeck[0].getCardRank() == ACE){
        
                    myDeck[0].getCardSprite().setPosition(BUILD_PILE_POS_X + (BUILD_PILE_OFFSET_X * 0), BUILD_PILE_POS_Y);
                    m_isDragCard1 = false;
                    myDeck[0].setFaceUp();
                }
                else{
                    m_isDragCard1 = false;
                    myDeck[0].getCardSprite().setPosition(TABLE_COL_POS_X , TABLE_COL_POS_Y);
                }
            }
        }
        //mapPixelToCoords(pixelPos) is convert to world coordinates
        if(m_isDragCard1){
            myDeck[0].getCardSprite().setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)) - m_dragOffsetCard);
        }        
         
        // when mouse button is pressed drag the card at Table Col 2.
        if(event.type == sf::Event::MouseButtonPressed){
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            sf::FloatRect rectBounds = myDeck[2].getCardSprite().getGlobalBounds();

            if(rectBounds.contains(static_cast<sf::Vector2f>(mousePos))){
                // check if mouse click inside the card
                if(myDeck[2].getCardSprite().getGlobalBounds().contains(mousePos)){
                    m_isDragCard2 = true;
                    // Calculate the offset from the top-left corner of the card
                    m_dragOffsetCard = mousePos - myDeck[2].getCardSprite().getPosition();
                }
            }
        }
        if(event.type == sf::Event::MouseButtonReleased){
            if(event.mouseButton.button == sf::Mouse::Left){
                m_isDragCard2 = false;         
                myDeck[2].getCardSprite().setPosition(TABLE_COL_POS_X + (TABLE_OFFSET_POS_X * 1), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 1));
            }
        }
        //mapPixelToCoords(pixelPos) is convert to world coordinates
        if(m_isDragCard2){
            myDeck[2].getCardSprite().setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)) - m_dragOffsetCard);
        }        


        // when mouse button is pressed drag the card at Table Col 3.
        if(event.type == sf::Event::MouseButtonPressed){
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            sf::FloatRect rectBounds = myDeck[5].getCardSprite().getGlobalBounds();

            if(rectBounds.contains(static_cast<sf::Vector2f>(mousePos))){
                // check if mouse click inside the card
                if(myDeck[5].getCardSprite().getGlobalBounds().contains(mousePos)){
                    m_isDragCard3 = true;
                    // Calculate the offset from the top-left corner of the card
                    m_dragOffsetCard = mousePos - myDeck[5].getCardSprite().getPosition();
                }
            }

        }
        if(event.type == sf::Event::MouseButtonReleased){
            if(event.mouseButton.button == sf::Mouse::Left){
                m_isDragCard3 = false;         
                myDeck[5].getCardSprite().setPosition(TABLE_COL_POS_X + (TABLE_OFFSET_POS_X * 2), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 2));
            }
        }
        //mapPixelToCoords(pixelPos) is convert to world coordinates
        if(m_isDragCard3){
            myDeck[5].getCardSprite().setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)) - m_dragOffsetCard);
        }        


        // when mouse button is pressed drag the card at Table Col 4.
        if(event.type == sf::Event::MouseButtonPressed){
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            sf::FloatRect rectBounds = myDeck[9].getCardSprite().getGlobalBounds();

            if(rectBounds.contains(static_cast<sf::Vector2f>(mousePos))){
                // check if mouse click inside the card
                if(myDeck[9].getCardSprite().getGlobalBounds().contains(mousePos)){
                    m_isDragCard4 = true;
                    // Calculate the offset from the top-left corner of the card
                    m_dragOffsetCard = mousePos - myDeck[9].getCardSprite().getPosition();
                }
            }

        }
        if(event.type == sf::Event::MouseButtonReleased){
            if(event.mouseButton.button == sf::Mouse::Left){
                m_isDragCard4 = false;         
                myDeck[9].getCardSprite().setPosition(TABLE_COL_POS_X + (3*TABLE_OFFSET_POS_X), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 3)); 
            }
        }
        //mapPixelToCoords(pixelPos) is convert to world coordinates
        if(m_isDragCard4){
            myDeck[9].getCardSprite().setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)) - m_dragOffsetCard);
        }        


        // when mouse button is pressed drag the card at Table Col 5.
        if(event.type == sf::Event::MouseButtonPressed){
            sf::FloatRect rectBounds = myDeck[14].getCardSprite().getGlobalBounds();
            // check if mouse click inside the card
            if(rectBounds.contains(static_cast<sf::Vector2f>(mousePos))){
                // check if mouse click inside the card
                if(myDeck[14].getCardSprite().getGlobalBounds().contains(mousePos)){
                    m_isDragCard5 = true;
                    // Calculate the offset from the top-left corner of the card
                    m_dragOffsetCard = mousePos - myDeck[14].getCardSprite().getPosition();
                }
            }

        }
        if(event.type == sf::Event::MouseButtonReleased){
            if(event.mouseButton.button == sf::Mouse::Left){
                m_isDragCard5 = false;         
                myDeck[14].getCardSprite().setPosition(TABLE_COL_POS_X + (4*TABLE_OFFSET_POS_X), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 4)); 
            }
        }
        //mapPixelToCoords(pixelPos) is convert to world coordinates
        if(m_isDragCard5){
            myDeck[14].getCardSprite().setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)) - m_dragOffsetCard);
        }        

        
        // when mouse button is pressed drag the card at Table Col 6.
        if(event.type == sf::Event::MouseButtonPressed){
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            sf::FloatRect rectBounds = myDeck[20].getCardSprite().getGlobalBounds();

            if(rectBounds.contains(static_cast<sf::Vector2f>(mousePos))){
                // check if mouse click inside the card
                if(myDeck[20].getCardSprite().getGlobalBounds().contains(mousePos)){
                    m_isDragCard6 = !m_isDragCard6;
                    // Calculate the offset from the top-left corner of the card
                    m_dragOffsetCard = mousePos - myDeck[20].getCardSprite().getPosition();
                }
            }

        }
        if(event.type == sf::Event::MouseButtonReleased){
            if(event.mouseButton.button == sf::Mouse::Left){
                m_isDragCard6 = false;         
                myDeck[20].getCardSprite().setPosition(TABLE_COL_POS_X + (5*TABLE_OFFSET_POS_X), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 5)); 
            }
        }
        //mapPixelToCoords(pixelPos) is convert to world coordinates
        if(m_isDragCard6){
            myDeck[20].getCardSprite().setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)) - m_dragOffsetCard);
        }        


        // when mouse button is pressed drag the card at Table Col 7.
        if(event.type == sf::Event::MouseButtonPressed){
            sf::FloatRect rectBounds = myDeck[27].getCardSprite().getGlobalBounds();

            if(rectBounds.contains(static_cast<sf::Vector2f>(mousePos))){
                // check if mouse click inside the card
                if(myDeck[27].getCardSprite().getGlobalBounds().contains(mousePos)){
                    m_isDragCard7 = !m_isDragCard7;
                    // Calculate the offset from the top-left corner of the card
                    m_dragOffsetCard = mousePos - myDeck[27].getCardSprite().getPosition();
                }
            }
        }
        if(event.type == sf::Event::MouseButtonReleased){
            if(event.mouseButton.button == sf::Mouse::Left){
                m_isDragCard7 = false;           
                myDeck[27].getCardSprite().setPosition(TABLE_COL_POS_X + (6*TABLE_OFFSET_POS_X), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 6)); 
            }
        }
        //mapPixelToCoords(pixelPos) is convert to world coordinates
        if(m_isDragCard7){
            myDeck[27].getCardSprite().setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)) - m_dragOffsetCard);
        }        



        /*/finding and open all the card.
        for(int i = 0; i < 52; i++){
            if(event.type == sf::Event::MouseButtonPressed){
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                sf::FloatRect rectBounds = myDeck[i].getCardSprite().getGlobalBounds();

                if(rectBounds.contains(static_cast<sf::Vector2f>(mousePos))){
                    // check if mouse click inside the card
                    if(myDeck[i].getCardSprite().getGlobalBounds().contains(mousePos)){
                        m_isDragCard6 = true;
                        myDeck[i].setFaceUp();
                        // Calculate the offset from the top-left corner of the card
                        m_dragOffsetCard = mousePos - myDeck[i].getCardSprite().getPosition();
                    }
                }

            }
            if(event.type == sf::Event::MouseButtonReleased){
                if(event.mouseButton.button == sf::Mouse::Left){
                    m_isDragCard6 = false;         
                }
            }
            //mapPixelToCoords(pixelPos) is convert to world coordinates
            if(m_isDragCard6){
                myDeck[i].getCardSprite().setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)) - m_dragOffsetCard);
            }        
        }*/





        






    }
}

void Game::CheckTableCol()
{
    std::string strSuit, strColor;
    
    if(myDeck[0].getCardColor() == 0)
        strColor = "Black";
    if(myDeck[0].getCardColor() == 1)
        strColor = "Red";

    switch (myDeck[0].getCardSuit()){
        case CLUBS:
            strSuit = "Clubs";
            break;
        case DIAMONDS:
            strSuit = "Diamond";
            break;
        case HEARTS:
            strSuit = "Hearts";
            break;
        case SPADES:
            strSuit = "Spades";
            break;
    }

    std::cout<<"Card rank: " << myDeck[0].getCardRank() << std::endl;
    std::cout<<"Card suit: " << strSuit << std::endl;
    std::cout<<"Card color: "<< strColor <<  std::endl;    

    

}

void Game::CheckBuildPile()
{
    /*
    m_BuildPilesMap[0].setPosition(BUILD_PILE_POS_X + (BUILD_PILE_OFFSET_X * 0), BUILD_PILE_POS_Y); 
    m_BuildPilesMap[1].setPosition(BUILD_PILE_POS_X + (BUILD_PILE_OFFSET_X * 1), BUILD_PILE_POS_Y); 
    m_BuildPilesMap[2].setPosition(BUILD_PILE_POS_X + (BUILD_PILE_OFFSET_X * 2), BUILD_PILE_POS_Y); 
    m_BuildPilesMap[3].setPosition(BUILD_PILE_POS_X + (BUILD_PILE_OFFSET_X * 3), BUILD_PILE_POS_Y); 
    */

    // if Build_Pile is empty, then Ace needs to be the first in the stack.
    if(myDeck[0].getCardSprite().getGlobalBounds().intersects(m_BuildPilesMap[0].getGlobalBounds()) &&
            myDeck[0].getCardRank() == ACE)
    {        
        myDeck[0].getCardSprite().setPosition(BUILD_PILE_POS_X + (BUILD_PILE_OFFSET_X * 0), BUILD_PILE_POS_Y);
        m_isDragCard1 = false;
        myDeck[0].setFaceUp();
    }
    
    for(int i = 0; i < 4; i++)
    {
        // if Build_Pile is empty, then Ace needs to be the first in the stack.
        if(myDeck[i].getCardSprite().getGlobalBounds().intersects(m_BuildPilesMap[i].getGlobalBounds()))
           // myDeck[i].getCardSuit() == CLUBS)
        {        
            myDeck[i].getCardSprite().setPosition(BUILD_PILE_POS_X + (BUILD_PILE_OFFSET_X * i), BUILD_PILE_POS_Y);
            m_isDragCard1 = false;
            myDeck[i].setFaceUp();
        }
        
        // Testing to put Card on myDeck[51] to the Build Pile 1, 2, 3, or 4
        if(myDeck[51].getCardSprite().getGlobalBounds().intersects(m_BuildPilesMap[i].getGlobalBounds()))
        {        
            myDeck[51].getCardSprite().setPosition(BUILD_PILE_POS_X + (BUILD_PILE_OFFSET_X * i), BUILD_PILE_POS_Y);
            m_isDragCard1 = false;
            myDeck[51].setFaceUp();
        }
    }

}
void Game::Update()
{
    CheckBuildPile();




}

void Game::PutCardOnTable()
{
    // Table Col one, one card, flip open. 
    myDeck[0].setFaceUp();
    myDeck[0].getCardSprite().setPosition(TABLE_COL_POS_X , TABLE_COL_POS_Y);

    // Table Col two, two cards, one closed, one flip open.
    myDeck[1].setFaceDown();
    myDeck[2].setFaceUp();

    myDeck[1].getCardSprite().setPosition(TABLE_COL_POS_X + (TABLE_OFFSET_POS_X * 1), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 0));
    myDeck[2].getCardSprite().setPosition(TABLE_COL_POS_X + (TABLE_OFFSET_POS_X * 1), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 1));

    // Table Col three, three cards, all closed, one at the bottom flip open.
    myDeck[3].setFaceDown(); 
    myDeck[4].setFaceDown(); 
    myDeck[5].setFaceUp();
    
    myDeck[3].getCardSprite().setPosition(TABLE_COL_POS_X + (TABLE_OFFSET_POS_X * 2), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 0));
    myDeck[4].getCardSprite().setPosition(TABLE_COL_POS_X + (TABLE_OFFSET_POS_X * 2), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 1));
    myDeck[5].getCardSprite().setPosition(TABLE_COL_POS_X + (TABLE_OFFSET_POS_X * 2), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 2));

    //Table Col 4, 4 cards, all closed, one at the bottom flip open. 
    myDeck[6].setFaceDown(); 
    myDeck[7].setFaceDown(); 
    myDeck[8].setFaceDown();
    myDeck[9].setFaceUp();

    myDeck[6].getCardSprite().setPosition(TABLE_COL_POS_X + (3*TABLE_OFFSET_POS_X), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 0));
    myDeck[7].getCardSprite().setPosition(TABLE_COL_POS_X + (3*TABLE_OFFSET_POS_X), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 1)); 
    myDeck[8].getCardSprite().setPosition(TABLE_COL_POS_X + (3*TABLE_OFFSET_POS_X), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 2)); 
    myDeck[9].getCardSprite().setPosition(TABLE_COL_POS_X + (3*TABLE_OFFSET_POS_X), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 3)); 

    //Table Col 5, 5 cards, all closed, one at the bottom flip open. 
    myDeck[10].setFaceDown(); 
    myDeck[11].setFaceDown(); 
    myDeck[12].setFaceDown();
    myDeck[13].setFaceDown();
    myDeck[14].setFaceUp();

    myDeck[10].getCardSprite().setPosition(TABLE_COL_POS_X + (4*TABLE_OFFSET_POS_X), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 0));
    myDeck[11].getCardSprite().setPosition(TABLE_COL_POS_X + (4*TABLE_OFFSET_POS_X), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 1)); 
    myDeck[12].getCardSprite().setPosition(TABLE_COL_POS_X + (4*TABLE_OFFSET_POS_X), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 2));
    myDeck[13].getCardSprite().setPosition(TABLE_COL_POS_X + (4*TABLE_OFFSET_POS_X), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 3)); 
    myDeck[14].getCardSprite().setPosition(TABLE_COL_POS_X + (4*TABLE_OFFSET_POS_X), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 4)); 

    //Table Col 6, 6 cards, all closed, one at the bottom flip open. 
    myDeck[15].setFaceDown(); 
    myDeck[16].setFaceDown(); 
    myDeck[17].setFaceDown();
    myDeck[18].setFaceDown();
    myDeck[19].setFaceDown(); 
    myDeck[20].setFaceUp();

    myDeck[15].getCardSprite().setPosition(TABLE_COL_POS_X + (5*TABLE_OFFSET_POS_X), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 0));
    myDeck[16].getCardSprite().setPosition(TABLE_COL_POS_X + (5*TABLE_OFFSET_POS_X), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 1)); 
    myDeck[17].getCardSprite().setPosition(TABLE_COL_POS_X + (5*TABLE_OFFSET_POS_X), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 2));
    myDeck[18].getCardSprite().setPosition(TABLE_COL_POS_X + (5*TABLE_OFFSET_POS_X), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 3)); 
    myDeck[19].getCardSprite().setPosition(TABLE_COL_POS_X + (5*TABLE_OFFSET_POS_X), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 4)); 
    myDeck[20].getCardSprite().setPosition(TABLE_COL_POS_X + (5*TABLE_OFFSET_POS_X), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 5)); 

    //Table Col 7, 7 cards, all closed, one at the bottom flip open. 
    myDeck[21].setFaceDown(); 
    myDeck[22].setFaceDown(); 
    myDeck[23].setFaceDown();
    myDeck[24].setFaceDown();
    myDeck[25].setFaceDown(); 
    myDeck[26].setFaceDown();
    myDeck[27].setFaceUp();

    myDeck[21].getCardSprite().setPosition(TABLE_COL_POS_X + (6*TABLE_OFFSET_POS_X), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 0));
    myDeck[22].getCardSprite().setPosition(TABLE_COL_POS_X + (6*TABLE_OFFSET_POS_X), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 1)); 
    myDeck[23].getCardSprite().setPosition(TABLE_COL_POS_X + (6*TABLE_OFFSET_POS_X), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 2));
    myDeck[24].getCardSprite().setPosition(TABLE_COL_POS_X + (6*TABLE_OFFSET_POS_X), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 3)); 
    myDeck[25].getCardSprite().setPosition(TABLE_COL_POS_X + (6*TABLE_OFFSET_POS_X), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 4)); 
    myDeck[26].getCardSprite().setPosition(TABLE_COL_POS_X + (6*TABLE_OFFSET_POS_X), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 5)); 
    myDeck[27].getCardSprite().setPosition(TABLE_COL_POS_X + (6*TABLE_OFFSET_POS_X), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 6)); 


 

}

void Game::Render(sf::RenderWindow &window)
{
    for(int i = 0; i < 4; i++){
        window.draw(m_BuildPilesMap[i]);
    }
    // Discard Pile Next to the Draw Pile
    window.draw(m_BuildPilesMap[4]); 

    // Draw Pile
    for(int i = 28; i < 52; i++){
        window.draw(myDeck[i].getCardSprite());
    }

    // Draw Card on Table
    for(int i = 0; i < 28; i++){
        window.draw(myDeck[i].getCardSprite());
    }
  

 
    /*/Test for in ordered card
    for(int i = 0; i < 52; i++){
        window.draw(tempDeck[i].getCardSprite());
        window.draw(myDeck[i].getCardSprite());
    }*/
    
   

}
