#include <iostream>
#include <cstdlib>
#include <string>
#include <SFML/Graphics.hpp>
#include <random>
#include <utility>

// global constants
// --------------------------------------------------------
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = WINDOW_WIDTH;
const sf::Color WINDOW_COLOR = sf::Color::White;
const float FRAME_RATE = (1.0/30.0) * 4000.0;

const int SQUARE_THICKNESS = WINDOW_WIDTH / 8;
const sf::Color COVER_COLOR_A = sf::Color::Green;
const sf::Color COVER_COLOR_B = sf::Color::Blue;
const sf::Color CLICKED_COVER_COLOR = sf::Color::Transparent;
const sf::Color UNDER_COLOR_A = sf::Color::White;
const sf::Color UNDER_COLOR_B = sf::Color{183,183,183,255};

const sf::Color BOMB_COLOR = sf::Color::Black;
const int BOMB_RADIUS = SQUARE_THICKNESS * 0.7 / 2;

const sf::Color FLAG_COLOR = sf::Color::Red;
const int FLAG_RADIUS = SQUARE_THICKNESS * 0.7 / 2;

const sf::Color NUMBER_COLOR = sf::Color::Black;
const int NUMBER_SIZE = 50;


// function definitions
// --------------------------------------------------------
void objectsSetup(sf::RectangleShape coverSquareArrayA[32],
                  sf::RectangleShape coverSquareArrayB[32],
                  sf::RectangleShape underSquareArrayA[32],
                  sf::RectangleShape underSquareArrayB[32],
                  sf::CircleShape bomb[8], sf::CircleShape flag[8],
                  sf::Text numbers[64], sf::Font &font);
void update(sf::RenderWindow &gameWindow,
            sf::RectangleShape coverSquareArrayA[32],
            sf::RectangleShape coverSquareArrayB[32],
            sf::CircleShape bomb[8], sf::CircleShape flag[8],
            bool &gameOver, sf::Text numbers[64]);
void draw(sf::RenderWindow &gameWindow, sf::RectangleShape coverSquareArrayA[32],
          sf::RectangleShape coverSquareArrayB[32],
          sf::RectangleShape underSquareArrayA[32],
          sf::RectangleShape underSquareArrayB[32],
          sf::CircleShape bomb[8], sf::CircleShape flag[8],
          sf::Text numbers[64], sf::Font &font);
void ifNumIs0(sf::RectangleShape coverSquareArrayA[32],
              sf::RectangleShape coverSquareArrayB[32],
              sf::Text numbers[64], int squarePositionX, int
              squarePositionY, int numPositionX,
              int numPositionY);

// --------------------------------------------------------
int main() {

    // defining the window
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH,
                                          WINDOW_HEIGHT),
                                          "MineSweeper");
    window.clear(WINDOW_COLOR);


    // initializing the objects in the game
    sf::RectangleShape coverSquareArrayA[32];
    sf::RectangleShape coverSquareArrayB[32];
    sf::RectangleShape underSquareArrayA[32];
    sf::RectangleShape underSquareArrayB[32];
    sf::CircleShape bombArray[8];
    sf::CircleShape flagArray[8];
    sf::Text numbersArray[64];

    // loading font
    sf::Font font;
    font.loadFromFile("Roboto-Medium.ttf");

    // setting up the objects in the game
    objectsSetup(coverSquareArrayA, coverSquareArrayB,
                 underSquareArrayA, underSquareArrayB, bombArray,
                 flagArray, numbersArray, font);

    // timing variables for the main game loop
    sf::Clock clock;
    sf::Time startTime = clock.getElapsedTime();
    sf::Time stopTime = startTime;
    float delta = 0.0;


    // variables to control the games start and finish
    bool started = false;
    bool gameOver = false;
    while (!gameOver){

        // calculate frame time
        stopTime = clock.getElapsedTime();
        delta += (stopTime.asMilliseconds() - startTime.asMilliseconds());
        startTime = stopTime;

        // event processing
        sf::Event event;
        while(window.pollEvent(event)){
            // closing the window
            if (event.type == sf::Event::Closed)
                gameOver = true;
        } // end while(gameWindow.pollEvent(event))


        // if it has been at least a full frame time
        if (delta >= FRAME_RATE) {

            // update the game properties
            update(window, coverSquareArrayA, coverSquareArrayB,
                   bombArray, flagArray, gameOver, numbersArray);

            // subtract the frame-rate from the current frame-time
            // for each full frame covered by this update
            while (delta >= FRAME_RATE)
                delta -= FRAME_RATE;
        }

        // draw the window and game elements
        draw(window, coverSquareArrayA, coverSquareArrayB,
             underSquareArrayA, underSquareArrayB, bombArray,
             flagArray, numbersArray, font);

    } // end while(!gameover)

    // game has ended, close the graphics window
    window.close();

    return 0;
} // end main


// function definitions
// --------------------------------------------------------
/**
 * initializing the game objects
 * @param coverSquareArrayA - first set of cover squares
 * @param coverSquareArrayB - second set of cover squares
 * @param underSquareArrayA - first set of under squares
 * @param underSquareArrayB - second set of under squares
 * @param bomb
 * @param flag
 */
void objectsSetup(sf::RectangleShape coverSquareArrayA[32],
                  sf::RectangleShape coverSquareArrayB[32],
                  sf::RectangleShape underSquareArrayA[32],
                  sf::RectangleShape underSquareArrayB[32],
                  sf::CircleShape bomb[8], sf::CircleShape flag[8],
                  sf::Text numbers[64], sf::Font &font){


    // first set of squares in the cover array (array A) ------------
    for(int i = 0; i < 32; i++){
        coverSquareArrayA[i].setSize(sf::Vector2f(SQUARE_THICKNESS,
                                                  SQUARE_THICKNESS));

        // calculating positions
        int xPosition;
        int yPosition;

        yPosition = i / 4;

        if(yPosition % 2 == 0){
            xPosition = (i % 4) * 2;
        }
        else{
            xPosition = (i % 4) * 2 + 1;
        }

        xPosition *= SQUARE_THICKNESS;
        yPosition *= SQUARE_THICKNESS;
        coverSquareArrayA[i].setPosition(xPosition, yPosition);

        coverSquareArrayA[i].setFillColor(COVER_COLOR_A);
    }

    // second set of squares in the cover array (array B) -----------
    for(int i = 0; i < 32; i++){
        coverSquareArrayB[i].setSize(sf::Vector2f(SQUARE_THICKNESS,
                                                  SQUARE_THICKNESS));

        // calculating positions
        int xPosition;
        int yPosition;

        yPosition = i / 4;

        if(yPosition % 2 == 0){
            xPosition = (i % 4) * 2 + 1;
        }
        else{
            xPosition = (i % 4) * 2;
        }

        xPosition *= SQUARE_THICKNESS;
        yPosition *= SQUARE_THICKNESS;
        coverSquareArrayB[i].setPosition(xPosition, yPosition);

        coverSquareArrayB[i].setFillColor(COVER_COLOR_B);
    }

    // first set of squares in the under array (array A) ------------
    for(int i = 0; i < 32; i++){
        underSquareArrayA[i].setSize(sf::Vector2f(SQUARE_THICKNESS,
                                                  SQUARE_THICKNESS));

        // calculating positions
        int xPosition;
        int yPosition;

        yPosition = i / 4;

        if(yPosition % 2 == 0){
            xPosition = (i % 4) * 2;
        }
        else{
            xPosition = (i % 4) * 2 + 1;
        }

        xPosition *= SQUARE_THICKNESS;
        yPosition *= SQUARE_THICKNESS;
        underSquareArrayA[i].setPosition(xPosition, yPosition);

        underSquareArrayA[i].setFillColor(UNDER_COLOR_A);
    }

    // second set of squares in the under array (array B) -----------
    for(int i = 0; i < 32; i++){
        underSquareArrayB[i].setSize(sf::Vector2f(SQUARE_THICKNESS,
                                                  SQUARE_THICKNESS));

        // calculating positions
        int xPosition;
        int yPosition;

        yPosition = i / 4;

        if(yPosition % 2 == 0){
            xPosition = (i % 4) * 2 + 1;
        }
        else{
            xPosition = (i % 4) * 2;
        }

        xPosition *= SQUARE_THICKNESS;
        yPosition *= SQUARE_THICKNESS;
        underSquareArrayB[i].setPosition(xPosition, yPosition);

        underSquareArrayB[i].setFillColor(UNDER_COLOR_B);
    }

    // initial properties of bombs ----------------------------------
    // random number generator engine
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution (0, 7);

    // array that will hold bomb positions to prevent duplicates
    std::pair<int,int> prevBombPositions[8];

    for(int i = 0; i < 8; i++){

        bomb[i].setRadius(BOMB_RADIUS);
        bomb[i].setFillColor(BOMB_COLOR);

        // calculating the position
        int bombXPosition = (distribution(gen) + .15) *
                            SQUARE_THICKNESS;
        int bombYPosition = (distribution(gen) + .15) *
                            SQUARE_THICKNESS;

        if(bombXPosition == 114){
            bombXPosition = 115;
        }
        if(bombYPosition == 114){
            bombYPosition = 115;
        }

        // if not the first bomb, check if the square is already taken
        if(i != 0){

            // while the position is taken, re-try positions
            bool positionTaken = true;
            while(positionTaken){

                int numOfTakenPos = 0;

                // looping through array of previous positions
                for(int j = 0; j < i; j++){

                    // if the positions match, add to numOfTakenPos
                    if(bombXPosition == prevBombPositions[j].first
                    && bombYPosition == prevBombPositions[j].second){

                        numOfTakenPos++;
                    }
                }

                // if position isn't taken, set positionTake false
                // to exit while loop
                if(numOfTakenPos == 0){
                    positionTaken = false;
                }
                    // if there are any positions already taken, set
                    // positionTaken to true and find new positions
                else{
                    positionTaken = true;
                    bombXPosition = (distribution(gen) + .15) * SQUARE_THICKNESS;
                    bombYPosition = (distribution(gen) + .15) * SQUARE_THICKNESS;
                    if(bombXPosition == 114){
                        bombXPosition = 115;
                    }
                    if(bombYPosition == 114){
                        bombYPosition = 115;
                    }
                }
            } // end while position is taken
        } // end checking positions

        // now that the position are different (or it's the first
        // bomb), assign it
        bomb[i].setPosition(bombXPosition, bombYPosition);

        // store position in array to check future bomb placement
        prevBombPositions[i] = std::make_pair(bombXPosition, bombYPosition);

    } // end initializing bombs

    // initial properties of flags----------------------------------
    for(int i = 0; i < 8; i++){

        flag[i].setRadius(FLAG_RADIUS);
        flag[i].setPointCount(3);
        flag[i].setFillColor(FLAG_COLOR);

        // setting flags to be outside of window
        flag[i].setPosition(WINDOW_WIDTH * 2, WINDOW_WIDTH * 2);
    }

    // initializing the numbers--------------------------------------
    for(int i = 0; i < 64; i++){

        // each square will have a number
        int numXPosition = ((i % 8) + 0.15) * SQUARE_THICKNESS;
        int numYPosition = ((i / 8) + 0.15) * SQUARE_THICKNESS;
        numbers[i].setPosition(numXPosition, numYPosition);

        if(numXPosition == 114){
            numXPosition = 115;
        }
        if(numYPosition == 114){
            numYPosition = 115;
        }

        numbers[i].setFillColor(NUMBER_COLOR);
        numbers[i].setCharacterSize(NUMBER_SIZE);
        numbers[i].setFont(font);

        int numBombs = 0;
        // loop through bombs to see if they match
        for(int j = 0; j < 8; j++){

            // see how many bombs are around
            // if bomb is in row above
            if(numYPosition - SQUARE_THICKNESS == bomb[j].getPosition().y){

                // if bomb is top left
                if(numXPosition - SQUARE_THICKNESS == bomb[j].getPosition().x){
                    numBombs++;
                }
                // if bomb is directly above
                if(numXPosition == bomb[j].getPosition().x){
                    numBombs++;
                }
                // if bomb is top right
                if(numXPosition + SQUARE_THICKNESS == bomb[j].getPosition().x){
                    numBombs++;
                }
            }
            // if bomb is in row below
            if(numYPosition + SQUARE_THICKNESS == bomb[j].getPosition().y){

                // if bomb is bottom left
                if(numXPosition - SQUARE_THICKNESS == bomb[j].getPosition().x){
                    numBombs++;
                }
                // if bomb is directly below
                if(numXPosition == bomb[j].getPosition().x){
                    numBombs++;
                }
                // if bomb is bottom right
                if(numXPosition + SQUARE_THICKNESS == bomb[j].getPosition().x){
                    numBombs++;
                }
            }
            // if bomb is in same row
            if(numYPosition == bomb[j].getPosition().y){

                // if bomb is bottom left
                if(numXPosition - SQUARE_THICKNESS == bomb[j].getPosition().x){
                    numBombs++;
                }
                // if bomb is bottom right
                if(numXPosition + SQUARE_THICKNESS == bomb[j].getPosition().x){
                    numBombs++;
                }
            }
        } // end looping through bombs

        // set the number to how many bombs are near it
        numbers[i].setString(std::to_string(numBombs));
        if(numBombs == 0){
            numbers[i].setFillColor(sf::Color::Transparent);
        }

    } // end initializing numbers
} // end of objectsSetup


void update(sf::RenderWindow &gameWindow,
            sf::RectangleShape coverSquareArrayA[32],
            sf::RectangleShape coverSquareArrayB[32],
            sf::CircleShape bomb[8], sf::CircleShape flag[8], bool
            &gameOver, sf::Text numbers[64]){

    // evaluating user input
    // Check if a mouse button is pressed
    // if left button - not a flag, selecting square
    // if right button - flag
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

        // get position
        int mousePositionX = sf::Mouse::getPosition(gameWindow).x;
        int mousePositionY = sf::Mouse::getPosition(gameWindow).y;


        // check if there's a bomb where clicked
        int bombPositionX = ((mousePositionX / SQUARE_THICKNESS) +
                0.15) * SQUARE_THICKNESS;
        int bombPositionY = ((mousePositionY / SQUARE_THICKNESS) +
                0.15) * SQUARE_THICKNESS;

        for(int i = 0; i < 8; i++){
            if (bombPositionX == bomb[i].getPosition().x &&
            bombPositionY == bomb[i].getPosition().y){

                gameOver = true;
            }
        }


        // deal with squares
        int squarePositionX = mousePositionX / SQUARE_THICKNESS *
                SQUARE_THICKNESS;
        int squarePositionY = mousePositionY / SQUARE_THICKNESS *
                SQUARE_THICKNESS;

        // if the squares have the same position, make it clear
        for (int i = 0; i < 32; i++) {
            if (squarePositionX == coverSquareArrayA[i]
                    .getPosition().x && squarePositionY ==
                                        coverSquareArrayA[i]
                                        .getPosition().y) {

                coverSquareArrayA[i].setFillColor(CLICKED_COVER_COLOR);
            } else if (squarePositionX == coverSquareArrayB[i]
                    .getPosition().x && squarePositionY ==
                                        coverSquareArrayB[i].getPosition().y) {

                coverSquareArrayB[i].setFillColor(CLICKED_COVER_COLOR);
            }
        }

        // check if the number where clicked
        int numPositionX = ((mousePositionX / SQUARE_THICKNESS) +
                            0.15) * SQUARE_THICKNESS;
        int numPositionY = ((mousePositionY / SQUARE_THICKNESS) +
                            0.15) * SQUARE_THICKNESS;

        // check if number is zero and clear surrounding tiles if
        // it is
        // if the tile for the number is clicked
        for(int n = 0; n < 64; n++){

            if (numPositionX == numbers[n].getPosition().x
                && numPositionY == numbers[n].getPosition().y) {

                ifNumIs0(coverSquareArrayA, coverSquareArrayB, numbers,
                         squarePositionX, squarePositionY, numPositionX, numPositionY);
            }
        }


    } else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {

        // get position
        int mousePositionX = sf::Mouse::getPosition(gameWindow).x;
        int mousePositionY = sf::Mouse::getPosition(gameWindow).y;

        // calculate flag position
        int flagPositionX = ((mousePositionX / SQUARE_THICKNESS) +
                             0.15) * SQUARE_THICKNESS;
        int flagPositionY = ((mousePositionY / SQUARE_THICKNESS) +
                             0.15) * SQUARE_THICKNESS;

        for(int i = 0; i < 8; i++){
            // if a flag is available, allot it
            if(flag[i].getPosition().x == WINDOW_WIDTH * 2){

                flag[i].setPosition(flagPositionX, flagPositionY);
                i = 8;
            }
            // if the flag has been allotted, remove it
            else if (flag[i].getPosition().x == flagPositionX &&
            flag[i].getPosition().y == flagPositionY){

                flag[i].setPosition(WINDOW_WIDTH * 2, WINDOW_WIDTH * 2);
                i = 8;
            }
        }
    } // if right button
} // end update


void draw(sf::RenderWindow &gameWindow, sf::RectangleShape coverSquareArrayA[32],
          sf::RectangleShape coverSquareArrayB[32],
          sf::RectangleShape underSquareArrayA[32],
          sf::RectangleShape underSquareArrayB[32],
          sf::CircleShape bomb[8], sf::CircleShape flag[8],
          sf::Text numbers[64], sf::Font &font){


    // clearing window and setting it to the window's color
    gameWindow.clear(WINDOW_COLOR);


    // drawing under squares first so that they'll be at the bottom
    for(int i = 0; i < 32; i++){
        gameWindow.draw(underSquareArrayA[i]);
    }
    for(int i = 0; i < 32; i++){
        gameWindow.draw(underSquareArrayB[i]);
    }

    // drawing numbers
    for(int i = 0; i < 64; i++){
        gameWindow.draw(numbers[i]);
    }

    // drawing bombs
    for(int i = 0; i < 8; i++){
        gameWindow.draw(bomb[i]);
    }

    // drawing cover squares
    for(int i = 0; i < 32; i++){
        gameWindow.draw(coverSquareArrayA[i]);
    }
    for(int i = 0; i < 32; i++){
        gameWindow.draw(coverSquareArrayB[i]);
    }

    // drawing flags
    for(int i = 0; i < 8; i++){
        gameWindow.draw(flag[i]);
    }

    // display game window
    gameWindow.display();

} // end draw


void ifNumIs0(sf::RectangleShape coverSquareArrayA[32],
              sf::RectangleShape coverSquareArrayB[32],
              sf::Text numbers[64], int squarePositionX,
              int squarePositionY, int numPositionX,
              int numPositionY){

    for(int j = 0; j < 64; j++) {

        // if the number value is zero, clear the squares
        // around it
        if (numbers[j].getString() == "0") {

            // if the squares have the same position, make it clear
            for (int i = 0; i < 32; i++) {

                // square in row above
                if (squarePositionY - SQUARE_THICKNESS ==
                    coverSquareArrayA[i].getPosition().y) {

                    // top left square
                    if (squarePositionX - SQUARE_THICKNESS ==
                        coverSquareArrayA[i].getPosition().x){

                        coverSquareArrayA[i].setFillColor(CLICKED_COVER_COLOR);
                    }
                    // directly above square
                    if (squarePositionX ==
                        coverSquareArrayA[i].getPosition().x){

                        coverSquareArrayA[i].setFillColor(CLICKED_COVER_COLOR);
                    }
                    // top right square
                    if (squarePositionX + SQUARE_THICKNESS ==
                        coverSquareArrayA[i].getPosition().x){

                        coverSquareArrayA[i].setFillColor(CLICKED_COVER_COLOR);
                    }
                }
                if (squarePositionY - SQUARE_THICKNESS ==
                    coverSquareArrayB[i].getPosition().y) {

                    // top left square
                    if (squarePositionX - SQUARE_THICKNESS ==
                        coverSquareArrayB[i].getPosition().x){

                        coverSquareArrayB[i].setFillColor(CLICKED_COVER_COLOR);
                    }
                    // directly above square
                    if (squarePositionX ==
                        coverSquareArrayB[i].getPosition().x){

                        coverSquareArrayB[i].setFillColor(CLICKED_COVER_COLOR);
                    }
                    // top right square
                    if (squarePositionX + SQUARE_THICKNESS ==
                        coverSquareArrayB[i].getPosition().x){

                        coverSquareArrayB[i].setFillColor(CLICKED_COVER_COLOR);
                    }
                } // end row above

                // square in row below
                if (squarePositionY + SQUARE_THICKNESS ==
                    coverSquareArrayA[i].getPosition().y) {

                    // bottom left square
                    if (squarePositionX - SQUARE_THICKNESS ==
                        coverSquareArrayA[i].getPosition().x){

                        coverSquareArrayA[i].setFillColor(CLICKED_COVER_COLOR);
                    }
                    // directly below square
                    if (squarePositionX ==
                        coverSquareArrayA[i].getPosition().x){

                        coverSquareArrayA[i].setFillColor(CLICKED_COVER_COLOR);
                    }
                    // bottom right square
                    if (squarePositionX + SQUARE_THICKNESS ==
                        coverSquareArrayA[i].getPosition().x){

                        coverSquareArrayA[i].setFillColor(CLICKED_COVER_COLOR);
                    }
                }
                if (squarePositionY + SQUARE_THICKNESS ==
                    coverSquareArrayB[i].getPosition().y) {

                    // bottom left square
                    if (squarePositionX - SQUARE_THICKNESS ==
                        coverSquareArrayB[i].getPosition().x){

                        coverSquareArrayB[i].setFillColor(CLICKED_COVER_COLOR);
                    }
                    // directly below square
                    if (squarePositionX ==
                        coverSquareArrayB[i].getPosition().x){

                        coverSquareArrayB[i].setFillColor(CLICKED_COVER_COLOR);
                    }
                    // bottom right square
                    if (squarePositionX + SQUARE_THICKNESS ==
                        coverSquareArrayB[i].getPosition().x){

                        coverSquareArrayB[i].setFillColor(CLICKED_COVER_COLOR);
                    }
                } // end row below

                // square in same row
                if (squarePositionY ==
                    coverSquareArrayA[i].getPosition().y) {

                    // directly left square
                    if (squarePositionX - SQUARE_THICKNESS ==
                        coverSquareArrayA[i].getPosition().x){

                        coverSquareArrayA[i].setFillColor(CLICKED_COVER_COLOR);
                    }
                    // directly right square
                    if (squarePositionX + SQUARE_THICKNESS ==
                        coverSquareArrayA[i].getPosition().x){

                        coverSquareArrayA[i].setFillColor(CLICKED_COVER_COLOR);
                    }
                }
                if (squarePositionY ==
                    coverSquareArrayB[i].getPosition().y) {

                    // directly left square
                    if (squarePositionX - SQUARE_THICKNESS ==
                        coverSquareArrayB[i].getPosition().x){

                        coverSquareArrayB[i].setFillColor(CLICKED_COVER_COLOR);
                    }
                    // directly right square
                    if (squarePositionX + SQUARE_THICKNESS ==
                        coverSquareArrayB[i].getPosition().x){

                        coverSquareArrayB[i].setFillColor(CLICKED_COVER_COLOR);
                    }
                } // end same row

            } // end looping through squares


            // if the surrounding nums are also zero, clear
            // their surrounding blocks

            // if row above
            if(numPositionY - SQUARE_THICKNESS == numbers[j]
                    .getPosition().y){

                // if bomb is top left
                if(numPositionX - SQUARE_THICKNESS == numbers[j].getPosition().x
                   && numbers[j].getString() == "0"){

                    ifNumIs0(coverSquareArrayA, coverSquareArrayB, numbers,
                             squarePositionX, squarePositionY, numPositionX, numPositionY);
                }
                // if bomb is directly above
                if(numPositionX == numbers[j].getPosition().x
                   && numbers[j].getString() == "0"){

                    ifNumIs0(coverSquareArrayA, coverSquareArrayB, numbers,
                             squarePositionX, squarePositionY, numPositionX, numPositionY);
                }
                // if bomb is top right
                if(numPositionX + SQUARE_THICKNESS == numbers[j].getPosition().x
                   && numbers[j].getString() == "0"){

                    ifNumIs0(coverSquareArrayA, coverSquareArrayB, numbers,
                             squarePositionX, squarePositionY, numPositionX, numPositionY);
                }
            }
            // if bomb is in row below
            if(numPositionY + SQUARE_THICKNESS == numbers[j].getPosition().y){

                // if bomb is bottom left
                if(numPositionX - SQUARE_THICKNESS == numbers[j].getPosition().x
                   && numbers[j].getString() == "0"){

                    ifNumIs0(coverSquareArrayA, coverSquareArrayB, numbers,
                             squarePositionX, squarePositionY, numPositionX, numPositionY);
                }
                // if bomb is directly below
                if(numPositionX == numbers[j].getPosition().x
                   && numbers[j].getString() == "0"){

                    ifNumIs0(coverSquareArrayA, coverSquareArrayB, numbers,
                             squarePositionX, squarePositionY, numPositionX, numPositionY);
                }
                // if bomb is bottom right
                if(numPositionX + SQUARE_THICKNESS ==
                   numbers[j].getPosition().x
                   && numbers[j].getString() == "0"){

                    ifNumIs0(coverSquareArrayA, coverSquareArrayB, numbers,
                             squarePositionX, squarePositionY, numPositionX, numPositionY);
                }
            }
            // if bomb is in same row
            if(numPositionY == numbers[j].getPosition().y){

                // if bomb is directly left
                if(numPositionX - SQUARE_THICKNESS == numbers[j].getPosition().x
                   && numbers[j].getString() == "0"){

                    ifNumIs0(coverSquareArrayA, coverSquareArrayB, numbers,
                             squarePositionX, squarePositionY, numPositionX, numPositionY);
                }
                // if bomb is directly right
                if(numPositionX + SQUARE_THICKNESS ==
                   numbers[j].getPosition().x
                   && numbers[j].getString() == "0"){

                    ifNumIs0(coverSquareArrayA, coverSquareArrayB, numbers,
                             squarePositionX, squarePositionY, numPositionX, numPositionY);
                }
            }

        } // end if "0"
    }
} // end ifNumIs0