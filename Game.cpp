#include "Game.hpp"
#include "Card.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Window.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <algorithm>
#include <random>
#include <vector>


Game::Game():myDeck(52), tempDeck(52), m_isDragCard1(false)
{
    CardInit();
    m_isDiscardPileEmpty = true;
}

Game::~Game()
{

}

void Game::CardInit()
{
      if(!m_pilesTexture.loadFromFile("./assets/FinalRectangle.png")){
          std::cerr<<"Error Loading FinalRectangle.png!\n";
          return;
      }
      m_pilesSprite.setTexture(m_pilesTexture);                                                                                         

      BuildPile0.resize(14);
      BuildPile1.resize(14);
      BuildPile2.resize(14);
      BuildPile3.resize(14);
      
      BuildPile0_Index = 1;
      BuildPile1_Index = 1;
      BuildPile2_Index = 1;
      BuildPile3_Index = 1;  

      for(int i = 0; i < 5; i++){
          m_BuildPilesMap[i] = m_pilesSprite;
      }
      m_BuildPilesMap[0].setPosition(BUILD_PILE_POS_X + (BUILD_PILE_OFFSET_X * 0), BUILD_PILE_POS_Y); 
      m_BuildPilesMap[1].setPosition(BUILD_PILE_POS_X + (BUILD_PILE_OFFSET_X * 1), BUILD_PILE_POS_Y); 
      m_BuildPilesMap[2].setPosition(BUILD_PILE_POS_X + (BUILD_PILE_OFFSET_X * 2), BUILD_PILE_POS_Y); 
      m_BuildPilesMap[3].setPosition(BUILD_PILE_POS_X + (BUILD_PILE_OFFSET_X * 3), BUILD_PILE_POS_Y); 
  
      // Discard Draw Pile
      m_BuildPilesMap[4].setPosition(DISCARD_POSITION_X, DISCARD_POSITION_Y);
  
      
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
          //myDeck[i].setCardFrontSprite(sf::IntRect((i%13)*CARD_WIDTH, (i/13)*CARD_HEIGHT, CARD_WIDTH, CARD_HEIGHT));
                 
          tempDeck[i].setCardRank(rank);
          tempDeck[i].setCardSuit(suit);   
          
          //myDeck[i].setCardRank(rank);
          //myDeck[i].setCardSuit(suit);
  
          // set color of the suit base on the Suit - enum Suit {CLUBS, DIAMONDS, HEARTS, SPADES};   
          if(suit == 1)
              tempDeck[i].setCardColor(1); 
              //myDeck[i].setCardColor(1);
          if(suit == 2)
              tempDeck[i].setCardColor(1);
              //myDeck[i].setCardColor(1);
          if(suit == 0)
              tempDeck[i].setCardColor(0);
              //myDeck[i].setCardColor(0);
          if(suit == 3)
              tempDeck[i].setCardColor(0);
              //myDeck[i].setCardColor(0);
  
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
  
      // copy randomize cards from tempDeck vector to myDeck vector using indices assignment    
      for(int i = 0; i < 52; i++){     
          myDeck[i] = tempDeck[m_cardIndex[i]];
          //myDeck[i] = tempDeck[i];
   
      }
      std::cout<<std::endl;
  
  
      // We put 28 cards on the tables
      PutCardOnTable();
      
     //int temp = 0; 
      // We put the remaining cards on the Draw Pile Face Down
      for(int i = 28; i < 52; i ++){
          myDeck[i].setFaceDown(); // original facedown
          myDeck[i].setCardPosition(DRAW_PILE_POSX, DRAW_PILE_POSY);          
          //++temp;
      }
      // Top of Draw Pile is myDeck[51].setFaceUp(); // For Testing


    if(!m_resetTexture.loadFromFile("./assets/reset.png")){ 
        std::cerr<<"Error loading spritesheet!\n";
        return;
    }

    m_resetButton.setTexture(m_resetTexture);
    m_resetButton.setPosition(1000.0, 800.0);
}


void Game::ProcessInput(sf::RenderWindow &window, sf::Event event)
{   
    // sf::Vector2f tempPosition(DISCARD_POSITION_X, DISCARD_POSITION_Y);
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));    
    
    sf::FloatRect rectBoundsResetButton = m_resetButton.getGlobalBounds();


    while(window.pollEvent(event))
    {
        if(event.type == sf::Event::Closed)
            window.close();


        for(int i = 0; i < 52; i++){         
            myDeck[i].cardProcessInput(window,event);
        
        }

        // check whether the moues position is within the bounds of the rectangle ResetButton
        if(rectBoundsResetButton.contains(static_cast<sf::Vector2f>(mousePos))){
            if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                // check if mouse click inside the ResetButton
                if(m_resetButton.getGlobalBounds().contains(mousePos)){
                    ResetButton(); // re-Initialize game or resetting the game. 
                }
            }
        }

        // Draw Cards from Deck
        if(m_isDiscardPileEmpty)
        {   
            DiscardCard.resize(1);
            for(int j = 51; j >= 28; j--)
            {
                sf::FloatRect rectBounds = myDeck[j].getCardSprite().getGlobalBounds();  
                // check whether the moues position is within the bounds of the rectangle Card m_frontSprite   
                if(rectBounds.contains(static_cast<sf::Vector2f>(mousePos))){ 
                    
                    // If myDeck[j].getIsFaceUp == false, then we can flip the card open, so we can let the Discarded Card move freely 
                    if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left
                            && myDeck[j].getIsFaceUp() == false) 
                    {
                        // if mouse is clicked inside the Rectangle myDeck[j] and the m_isDiscardPileEmpty is empty true
                        // then flip the card and set m_isDiscardPileEmpty is not empty.  
                        if(myDeck[j].getCardSprite().getGlobalBounds().contains(mousePos) && m_isDiscardPileEmpty == true && 
                            myDeck[j].getIsOnBuildPile() == false)
                        { 
                            m_isDiscardPileEmpty = false;
                            myDeck[j].setFaceUp();
                            myDeck[j].getCardSprite().setPosition(DISCARD_POSITION_X, DISCARD_POSITION_Y);

                            DiscardCard.resize(2);
                            DiscardCard[0] = myDeck[j];
                            break; // exit the loop when there is one card on the Discarded Pile. 
                                   // we will set the m_isDiscardPileEmpty to true 
                                   // when the DiscardCard[0] is moved to different location 
                                   // DiscardCard[0] position will follow myDeck[j] position. 
                        }
                    }
                    
                    if(event.type == sf::Event::MouseButtonReleased){
                      if(event.mouseButton.button == sf::Mouse::Left){
                        //to prevent from clicking
                      }
                    }

                }
            
             }

        }        
             
        /*/When we click on the FaceDown Card, it will Flip the Card Open. 
        for(int i = 1; i < TABLE_NUM_CARDS; i++){
            sf::FloatRect rectBounds = myDeck[i].getCardSprite().getGlobalBounds();
            // check whether the moues position is within the bounds of the rectangle Card m_frontSprite/m_backSprite
            if(rectBounds.contains(static_cast<sf::Vector2f>(mousePos))){
                
                // If myDeck[j].getIsFaceUp() == false, then we can flip the card open.
                if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left 
                        && myDeck[i].getIsFaceUp() == false)
                {
                    if(myDeck[2].getCardSprite().getPosition().x != TABLE_COL_POS_X + (TABLE_OFFSET_POS_X * 1) 
                            && myDeck[2].getCardSprite().getPosition().y != TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 1))
                        myDeck[i].flipCard();
                    break;
                }

            }
    
        }*/

        // if myDeck[2] card moves then myDeck[1] card on the top of myDeck[2] card  can be flip open. 
        sf::FloatRect rectBounds1 = myDeck[1].getCardSprite().getGlobalBounds();
        // check whether the moues position is within the bounds of the rectangle Card m_frontSprite/m_backSprite
        if(rectBounds1.contains(static_cast<sf::Vector2f>(mousePos)))
        {
            // If myDeck[1].getIsFaceUp() == false, then we can flip the card open.
            if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left
                && myDeck[1].getIsFaceUp() == false)
            {
                // if myDeck[2] card move from its original position then you can flip the myDeck[1] card.
                if(myDeck[2].getCardSprite().getPosition().x != TABLE_COL_POS_X + (TABLE_OFFSET_POS_X * 1)
                    && myDeck[2].getCardSprite().getPosition().y != TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 1))
                {
                    myDeck[1].flipCard();
                }
            }

        }

    
        sf::FloatRect rectBounds4 = myDeck[4].getCardSprite().getGlobalBounds();
          // check whether the moues position is within the bounds of the rectangle Card m_frontSprite/m_backSprite
          if(rectBounds4.contains(static_cast<sf::Vector2f>(mousePos)))
          {
              // If myDeck[4].getIsFaceUp() == false, then we can flip the card open.
              if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left
                  && myDeck[4].getIsFaceUp() == false)
              {
                  // if myDeck[5] card move from its original position then you can flip the myDeck[4] card. 
                  if(myDeck[5].getCardSprite().getPosition().x != TABLE_COL_POS_X + (TABLE_OFFSET_POS_X * 2)
                      && myDeck[5].getCardSprite().getPosition().y != TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 2))
                  {
                      myDeck[4].flipCard();
                  }
              }
  
         }

          // if myDeck[4] card moves then myDeck[3] card on the top of myDeck[4] card  can be flip open. 
          sf::FloatRect rectBounds3 = myDeck[3].getCardSprite().getGlobalBounds();
          // check whether the moues position is within the bounds of the rectangle Card m_frontSprite/m_backSprite
          if(rectBounds3.contains(static_cast<sf::Vector2f>(mousePos)))
          {
              // If myDeck[3].getIsFaceUp() == false, then we can flip the card open.
              if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left
                  && myDeck[3].getIsFaceUp() == false)
              {
                  // if myDeck[4] card move from its original position then you can flip the myDeck[3] card.
                  if(myDeck[4].getCardSprite().getPosition().x != TABLE_COL_POS_X + (TABLE_OFFSET_POS_X * 2)
                      && myDeck[4].getCardSprite().getPosition().y != TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 1))
                  {
                      myDeck[3].flipCard();
                  }
              }
  
          }
        
        /*/Flip Folded Card on Table 3 (3 is the index of the Card On Table) 
        int j = 3;
        for(int i = 9; i <= 6; i--)
        {
            
            sf::FloatRect rectBoundsPreviousCard = myDeck[i-1].getCardSprite().getGlobalBounds();
            
            if(rectBoundsPreviousCard.contains(static_cast<sf::Vector2f>(mousePos)))
            {
            
                if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left
                        && myDeck[i-1].getIsFaceUp() == false)
                {
                    
                    if(myDeck[i].getCardSprite().getPosition().x != TABLE_COL_POS_X + (TABLE_OFFSET_POS_X * 3)
                            && myDeck[i].getCardSprite().getPosition().y != TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * j))
                    {
                        myDeck[i-1].setFaceUp();
                        --j;
                    }

                }

            }

        }*/

        // if myDeck[9] card moves then myDeck[8] card on the top of myDeck[9] card  can be flip open. 
        sf::FloatRect rectBounds8 = myDeck[8].getCardSprite().getGlobalBounds();
        // check whether the moues position is within the bounds of the rectangle Card m_frontSprite/m_backSprite
        if(rectBounds8.contains(static_cast<sf::Vector2f>(mousePos)))
        {
            // If myDeck[8].getIsFaceUp() == false, then we can flip the card open.
            if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left
                && myDeck[8].getIsFaceUp() == false)
            {
                // if myDeck[9] card move from its original position then you can flip the myDeck[3] card.
                if(myDeck[9].getCardSprite().getPosition().x != TABLE_COL_POS_X + (TABLE_OFFSET_POS_X * 3)
                    && myDeck[9].getCardSprite().getPosition().y != TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 3))
                {
                    myDeck[8].flipCard();
                }
            }
        }
           



    }
}

void Game::CheckDiscardPile()
{   
    // The DiscardCard[0] aways has one element becaues we cannot have more than one draw card. 
    
    // tempIndex is the last index on the Draw Deck Card which is 51. 
    int tempIndex = myDeck.size()-1; 

    // There are 24 cards in Draw Deck because 52 total cards subtracted from 28 cards on the table. 
    int Num_Deck_Draw = 24;

    // There are 28 cards on the table. 
    int Card_On_Table = 28;


    if(!m_isDiscardPileEmpty){
    
        // std::cout<<myDeck[51].getCardPositionX()<<std::endl;
        for(int i = tempIndex; i >= Num_Deck_Draw; i--)
        {   
            for(int j = 0; j < TABLE_NUM_CARDS; j++)
            {
                //DiscardCard is never render, it is a variable to keep track of the Discarded Pile
                if(DiscardCard.size() != 0){

                    //if(DiscardCard[0].getCardPositionX() != 150 && DiscardCard[0].getCardPositionY() != 20)
                    //m_BuildPilesMap[4] is a Discard Pile. We check if the Draw Card intersect with the Discard Pile Rectangle
                    if(m_BuildPilesMap[4].getGlobalBounds().intersects(myDeck[i].getCardSprite().getGlobalBounds()))
                    {
                        m_isDiscardPileEmpty = false;
                    }
                    
                    if(myDeck[i].getIsPickUp() && myDeck[i].getCardSprite().getGlobalBounds().intersects(myDeck[j].getCardSprite().getGlobalBounds()) 
                            && myDeck[j].getIsFaceUp()) 
                    {
                        std::cout<<"DiscardCard moved"<<std::endl;    
                        DiscardCard.clear();
                        m_isDiscardPileEmpty = true;
                    }
                }
            }
        }
    }

   // Checking Cards CANNOT Stuck Together, Card will show on the bottom if stuck with another card. 
   for(int i = 0; i <= myDeck.size(); i++)
   {
        if(myDeck[i].getIsPickUp()){
            // Checking PickUp card intersect with cards on table  forward  
            for(int j = i + 1; j < myDeck.size(); j++){
                // check if PickUp Card intersect with Cards on Table with the FaceUP (not Back of the Card)
                if(myDeck[i].getCardSprite().getGlobalBounds().intersects(myDeck[j].getCardSprite().getGlobalBounds())
                    && myDeck[j].getIsFaceUp() && myDeck[j].getIsOnBuildPile() == false)
                    // if myDeck[j]. is not on the BuildPile then we can make sure the cards did not stuck together otherwise it will render 
                    // the card twice
                {
                    //std::cout<<"checking card intersect CANNOT HAVE CARDS STUCK TOGETHER"<<std::endl; 
                    myDeck[i].setCardPosition(myDeck[j].getCardPositionX(), myDeck[j].getCardPositionY() + TABLE_OFFSET_POS_Y);
                    myDeck[i].setIsParent(true);
                    myDeck[j].setIsChild(true);
                }

            }

        } 
        
        /*if(myDeck[i].getIsPickUp()){ 
            // Checking PickUp card intersect with cards on table backward     
            for(int j = Card_On_Table; j > 0 ; j--){
                // check if PickUp Card intersect with Cards on Table with the FaceUP (not Back of the Card)        
                if(myDeck[i].getCardSprite().getGlobalBounds().intersects(myDeck[j].getCardSprite().getGlobalBounds())
                         && myDeck[j].getIsFaceUp()) 
                {
                    std::cout<<"checking card intersect BACKWARD"<<std::endl;
                }
            }

        }*/      

   }   
            
}

void Game::CheckCardsOnTable()
{
    /*/ Test: Check the correct rank and suit of a card
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

    if(myDeck[2].getCardColor() == 0)
        strColor = "Black";
    if(myDeck[2].getCardColor() == 1)
        strColor = "Red";

    switch (myDeck[2].getCardSuit()){
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

    std::cout<<"Card rank: " << myDeck[2].getCardRank() << std::endl;
    std::cout<<"Card suit: " << strSuit << std::endl;
    std::cout<<"Card color: "<< strColor <<  std::endl;    


    if(myDeck[5].getCardColor() == 0)
        strColor = "Black";
    if(myDeck[5].getCardColor() == 1)
        strColor = "Red";

    switch (myDeck[5].getCardSuit()){
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

    std::cout<<"Card rank: " << myDeck[5].getCardRank() << std::endl;
    std::cout<<"Card suit: " << strSuit << std::endl;
    std::cout<<"Card color: "<< strColor <<  std::endl;    

    if(myDeck[9].getCardColor() == 0)
        strColor = "Black";
    if(myDeck[9].getCardColor() == 1)
        strColor = "Red";

    switch (myDeck[9].getCardSuit()){
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

    std::cout<<"Card rank: " << myDeck[9].getCardRank() << std::endl;
    std::cout<<"Card suit: " << strSuit << std::endl;
    std::cout<<"Card color: "<< strColor <<  std::endl;    

    if(myDeck[14].getCardColor() == 0)
        strColor = "Black";
    if(myDeck[14].getCardColor() == 1)
        strColor = "Red";

    switch (myDeck[14].getCardSuit()){
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

    std::cout<<"Card rank: " << myDeck[14].getCardRank() << std::endl;
    std::cout<<"Card suit: " << strSuit << std::endl;
    std::cout<<"Card color: "<< strColor <<  std::endl;    

    if(myDeck[20].getCardColor() == 0)
        strColor = "Black";
    if(myDeck[20].getCardColor() == 1)
        strColor = "Red";

    switch (myDeck[20].getCardSuit()){
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

    std::cout<<"Card rank: " << myDeck[20].getCardRank() << std::endl;
    std::cout<<"Card suit: " << strSuit << std::endl;
    std::cout<<"Card color: "<< strColor <<  std::endl;    

    if(myDeck[27].getCardColor() == 0)
        strColor = "Black";
    if(myDeck[27].getCardColor() == 1)
        strColor = "Red";

    switch (myDeck[27].getCardSuit()){
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

    std::cout<<"Card rank: " << myDeck[27].getCardRank() << std::endl;
    std::cout<<"Card suit: " << strSuit << std::endl;
    std::cout<<"Card color: "<< strColor <<  std::endl;    

    // Put card below the lower rank card
    if(myDeck[0].getCardSprite().getGlobalBounds().intersects(myDeck[2].getCardSprite().getGlobalBounds()))
    {
        std::cout <<"card 0 intersect with card 2"<<std::endl;
        if(myDeck[0].getCardRank() < myDeck[2].getCardRank()){
            float new_X, new_Y;
            new_X = myDeck[2].getCardSprite().getPosition().x;
            new_Y = myDeck[2].getCardSprite().getPosition().y;
            myDeck[0].setCardPosition(new_X, new_Y + TABLE_OFFSET_POS_Y); 
        }

    }

    // Put card below the lower rank card
    if(myDeck[0].getCardSprite().getGlobalBounds().intersects(myDeck[5].getCardSprite().getGlobalBounds()))
    {
        if(myDeck[0].getCardRank() < myDeck[5].getCardRank()){
            float new_X, new_Y;
            new_X = myDeck[5].getCardSprite().getPosition().x;
            new_Y = myDeck[5].getCardSprite().getPosition().y;
            myDeck[0].getCardSprite().setPosition(new_X, new_Y + TABLE_OFFSET_POS_Y);
        }

    }

    // Put card below the lower rank card
    if(myDeck[0].getCardSprite().getGlobalBounds().intersects(myDeck[9].getCardSprite().getGlobalBounds()))
    {
        if(myDeck[0].getCardRank() < myDeck[5].getCardRank()){
            float new_X, new_Y;
            new_X = myDeck[9].getCardSprite().getPosition().x;
            new_Y = myDeck[9].getCardSprite().getPosition().y;
            myDeck[0].getCardSprite().setPosition(new_X, new_Y + TABLE_OFFSET_POS_Y);
        }

    }
*/

    // check and update card base on its parent 
    /*   
    for (int j = 0; j < TABLE_NUM_COL; j++){
        for(int i = 0; i < j + 1; i++){
            
            if(myDeck[i].getCardSprite().getGlobalBounds().intersects(myDeck[i+1].getCardSprite().getGlobalBounds()) && myDeck[i].getIsFaceUp()){
                if(myDeck[i].getCardRank() < myDeck[i+1].getCardRank()){
                    std::cout<<"Check if myDeck["<< i <<"] < "<<"myDeck[" << i+1 <<"]"<<std::endl;                
                    //myDeck[i+1] is parent to myDeck[i]
                    myDeck[i+1].setIsParent(true);
                    myDeck[i].setIsChild(true);
                    float xpos, ypos;
                    xpos = myDeck[i+1].getCardSprite().getPosition().x;
                    ypos = myDeck[i+1].getCardSprite().getPosition().y; 
                    myDeck[i].getCardSprite().setPosition(xpos, ypos + TABLE_OFFSET_POS_Y);

                }
            }
         }
    }*/ 

    /*
    for(int i = 0; i < 52; i++){
        if(myDeck[i].getIsGetPickUp()){
            if(myDeck[i].getCardSprite().getGlobalBounds().intersects(myDeck[i+1].getCardSprite().getGlobalBounds()) && myDeck[i].getIsFaceUp()){

                if(myDeck[i].getCardRank() < myDeck[i+1].getCardRank()){
                    //std::cout<<"Check if myDeck["<< i <<"] < "<<"myDeck[" << i+1 <<"]"<<std::endl; 
                     //myDeck[i+1] is parent to myDeck[i]
                     myDeck[i+1].setIsParent(true);
                     myDeck[i].setIsChild(true);
                     float xpos, ypos;
                     xpos = myDeck[i+1].getCardSprite().getPosition().x;
                     ypos = myDeck[i+1].getCardSprite().getPosition().y;
                     myDeck[i].getCardSprite().setPosition(xpos, ypos + TABLE_OFFSET_POS_Y); 

                }
            
            }

         }

     }*/
            
        

   




}

void Game::CheckBuildPile()
{
    /*
    m_BuildPilesMap[0].setPosition(BUILD_PILE_POS_X + (BUILD_PILE_OFFSET_X * 0), BUILD_PILE_POS_Y); 
    m_BuildPilesMap[1].setPosition(BUILD_PILE_POS_X + (BUILD_PILE_OFFSET_X * 1), BUILD_PILE_POS_Y); 
    m_BuildPilesMap[2].setPosition(BUILD_PILE_POS_X + (BUILD_PILE_OFFSET_X * 2), BUILD_PILE_POS_Y); 
    m_BuildPilesMap[3].setPosition(BUILD_PILE_POS_X + (BUILD_PILE_OFFSET_X * 3), BUILD_PILE_POS_Y); 
    */
    
    for(int i = 0; i < m_BuildPilesMap.size(); i++)
    {
        for(int j = 0; j < myDeck.size(); j++){
            // if Build_Pile is empty, then Ace needs to be the first in the stack.
            if(myDeck[j].getCardSprite().getGlobalBounds().intersects(m_BuildPilesMap[i].getGlobalBounds()))
            {        
                switch(i)
                {
                    case 0:
                       if(myDeck[j].getCardRank() == BuildPile0_Index){ // check BuildPile in order 1(Ace) to 13(King)
                           BuildPile0[BuildPile0_Index] = myDeck[j];
                           myDeck[j].getCardSprite().setPosition(BUILD_PILE_POS_X + (BUILD_PILE_OFFSET_X * i), BUILD_PILE_POS_Y);
                           BuildPile0[BuildPile0_Index].getCardSprite().setPosition(myDeck[j].getCardSprite().getPosition());
                           m_isDragCard1 = false; 
                           myDeck[j].setFaceUp();
                           myDeck[j].setIsOnBuildPile(true);
                           ++BuildPile0_Index;
                       } break;
                    case 1:
                       if(myDeck[j].getCardRank() == BuildPile1_Index){
                           BuildPile1[BuildPile1_Index] = myDeck[j];
                           myDeck[j].getCardSprite().setPosition(BUILD_PILE_POS_X + (BUILD_PILE_OFFSET_X * i), BUILD_PILE_POS_Y);
                           BuildPile1[BuildPile1_Index].getCardSprite().setPosition(myDeck[j].getCardSprite().getPosition()); 
                           m_isDragCard1 = false;
                           myDeck[j].setFaceUp();
                           myDeck[j].setIsOnBuildPile(true);
                           ++BuildPile1_Index;
                       }break;                      
                    case 2:
                       if(myDeck[j].getCardRank() == BuildPile2_Index){
                           BuildPile2[BuildPile2_Index] = myDeck[j];
                           myDeck[j].getCardSprite().setPosition(BUILD_PILE_POS_X + (BUILD_PILE_OFFSET_X * i), BUILD_PILE_POS_Y);
                           BuildPile2[BuildPile2_Index].getCardSprite().setPosition(myDeck[j].getCardSprite().getPosition()); 
                           m_isDragCard1 = false;
                           myDeck[j].setFaceUp();
                           myDeck[j].setIsOnBuildPile(true);
                           ++BuildPile2_Index;
                       }break;                     
                    case 3:
                       if(myDeck[j].getCardRank() == BuildPile3_Index){
                           BuildPile3[BuildPile3_Index] = myDeck[j];
                           myDeck[j].getCardSprite().setPosition(BUILD_PILE_POS_X + (BUILD_PILE_OFFSET_X * i), BUILD_PILE_POS_Y);
                           BuildPile3[BuildPile3_Index].getCardSprite().setPosition(myDeck[j].getCardSprite().getPosition()); 
                           m_isDragCard1 = false;
                           myDeck[j].setFaceUp();
                           myDeck[j].setIsOnBuildPile(true);
                           ++BuildPile3_Index;
                       }break;

                }
            }
            

        }
    }



}
void Game::Update()
{
   
    CheckBuildPile();
    CheckCardsOnTable();
    CheckDiscardPile();

    /*/for testing open all the cards
    for(int i = 0; i < myDeck.size(); i++){
        myDeck[i].setFaceUp();
    }*/
}

void Game::PutCardOnTable()
{
    /*/ Table Col one, one card, flip open. 
    myDeck[0].setFaceUp();
    myDeck[0].getCardSprite().setPosition(TABLE_COL_POS_X + (TABLE_OFFSET_POS_X * 0), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 0));

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

    myDeck[6].getCardSprite().setPosition(TABLE_COL_POS_X + (TABLE_OFFSET_POS_X * 3), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 0));
    myDeck[7].getCardSprite().setPosition(TABLE_COL_POS_X + (TABLE_OFFSET_POS_X * 3), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 1)); 
    myDeck[8].getCardSprite().setPosition(TABLE_COL_POS_X + (TABLE_OFFSET_POS_X * 3), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 2)); 
    myDeck[9].getCardSprite().setPosition(TABLE_COL_POS_X + (TABLE_OFFSET_POS_X * 3), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 3)); 

    //Table Col 5, 5 cards, all closed, one at the bottom flip open. 
    myDeck[10].setFaceDown(); 
    myDeck[11].setFaceDown(); 
    myDeck[12].setFaceDown();
    myDeck[13].setFaceDown();
    myDeck[14].setFaceUp();

    myDeck[10].getCardSprite().setPosition(TABLE_COL_POS_X + (TABLE_OFFSET_POS_X * 4), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 0));
    myDeck[11].getCardSprite().setPosition(TABLE_COL_POS_X + (TABLE_OFFSET_POS_X * 4), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 1)); 
    myDeck[12].getCardSprite().setPosition(TABLE_COL_POS_X + (TABLE_OFFSET_POS_X * 4), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 2));
    myDeck[13].getCardSprite().setPosition(TABLE_COL_POS_X + (TABLE_OFFSET_POS_X * 4), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 3)); 
    myDeck[14].getCardSprite().setPosition(TABLE_COL_POS_X + (TABLE_OFFSET_POS_X * 4), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 4)); 

    //Table Col 6, 6 cards, all closed, one at the bottom flip open. 
    myDeck[15].setFaceDown(); 
    myDeck[16].setFaceDown(); 
    myDeck[17].setFaceDown();
    myDeck[18].setFaceDown();
    myDeck[19].setFaceDown(); 
    myDeck[20].setFaceUp();

    myDeck[15].getCardSprite().setPosition(TABLE_COL_POS_X + (TABLE_OFFSET_POS_X * 5), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 0));
    myDeck[16].getCardSprite().setPosition(TABLE_COL_POS_X + (TABLE_OFFSET_POS_X * 5), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 1)); 
    myDeck[17].getCardSprite().setPosition(TABLE_COL_POS_X + (TABLE_OFFSET_POS_X * 5), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 2));
    myDeck[18].getCardSprite().setPosition(TABLE_COL_POS_X + (TABLE_OFFSET_POS_X * 5), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 3)); 
    myDeck[19].getCardSprite().setPosition(TABLE_COL_POS_X + (TABLE_OFFSET_POS_X * 5), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 4)); 
    myDeck[20].getCardSprite().setPosition(TABLE_COL_POS_X + (TABLE_OFFSET_POS_X * 5), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 5)); 

    //Table Col 7, 7 cards, all closed, one at the bottom flip open. 
    myDeck[21].setFaceDown(); 
    myDeck[22].setFaceDown(); 
    myDeck[23].setFaceDown();
    myDeck[24].setFaceDown();
    myDeck[25].setFaceDown(); 
    myDeck[26].setFaceDown();
    myDeck[27].setFaceUp();

    myDeck[21].getCardSprite().setPosition(TABLE_COL_POS_X + (TABLE_OFFSET_POS_X * 6), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 0));
    myDeck[22].getCardSprite().setPosition(TABLE_COL_POS_X + (TABLE_OFFSET_POS_X * 6), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 1)); 
    myDeck[23].getCardSprite().setPosition(TABLE_COL_POS_X + (TABLE_OFFSET_POS_X * 6), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 2));
    myDeck[24].getCardSprite().setPosition(TABLE_COL_POS_X + (TABLE_OFFSET_POS_X * 6), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 3)); 
    myDeck[25].getCardSprite().setPosition(TABLE_COL_POS_X + (TABLE_OFFSET_POS_X * 6), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 4)); 
    myDeck[26].getCardSprite().setPosition(TABLE_COL_POS_X + (TABLE_OFFSET_POS_X * 6), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 5)); 
    myDeck[27].getCardSprite().setPosition(TABLE_COL_POS_X + (TABLE_OFFSET_POS_X * 6), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * 6)); 
    */

    
    int myDeck_index = 0;

    for (int j = 0; j < TABLE_NUM_COL; j++){
       for(int i = 0; i < j + 1 ; i++){
         myDeck[myDeck_index].setCardPosition(TABLE_COL_POS_X + (TABLE_OFFSET_POS_X * j), TABLE_COL_POS_Y + (TABLE_OFFSET_POS_Y * i)); 
         myDeck[myDeck_index].setFaceDown(); // original facedown 
         ++myDeck_index;
       }
       myDeck[myDeck_index - 1].flipCard();      
    } 
     
    CardsOnTable.resize(myDeck_index);

    // assigning myDeck cards to CardsOnTables to track
    for(int i = 0; i < myDeck_index; i++)
    {
    //    CardsOnTable[i] = myDeck[i];
    }
}

void Game::ResetButton()
{
    BuildPile0.clear();
    BuildPile1.clear();
    BuildPile2.clear();  
    BuildPile3.clear();

    CardInit();    
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

    // Rendering Card on the BuildPile in the correct order to display the last card (Descending Order)  
    if(BuildPile0_Index >= 1){
        for(int i = 0; i <  BuildPile0_Index; i++)
        {
            window.draw(BuildPile0[i].getCardSprite());
        }
    }

    if(BuildPile1_Index >= 1){
        for(int i = 0; i < BuildPile1_Index; i++)                                                                          
        {
            window.draw(BuildPile1[i].getCardSprite());
        }
    }
 
    if(BuildPile2_Index >= 1){
        for(int i = 0; i < BuildPile2_Index; i++)
        {
            window.draw(BuildPile2[i].getCardSprite());
        }
    }

    if(BuildPile3_Index >= 1){
        for(int i = 0; i < BuildPile3_Index; i++)
        {
            window.draw(BuildPile3[i].getCardSprite());
        }
    }

    window.draw(m_resetButton);

    /*/Test for in ordered card
    for(int i = 0; i < 52; i++){
        window.draw(tempDeck[i].getCardSprite());
        window.draw(myDeck[i].getCardSprite());
    }*/
    
   

}

void Game::setIsDiscardPileEmpty(bool isDiscardPileEmpty)
{
    m_isDiscardPileEmpty = isDiscardPileEmpty;
}

bool Game::getIsDiscardPileEmpty()
{
    return m_isDiscardPileEmpty;
}
