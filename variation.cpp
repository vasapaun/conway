#include <iostream>
#include <string>
#include <cstdlib>
#include <chrono>
#include <unistd.h>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Window.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/WindowStyle.hpp>

#define NUM_OF_STARTING_CELLS   10000
#define SIZE_OF_CELL            4
#define WHITE                   255, 255, 255
#define BLACK                   0  , 0  , 0

void getShapesFromMatrix(std::vector< std::vector<bool> >& matrix, std::vector< sf::RectangleShape >& shapes){
    sf::RectangleShape rectTemplate(sf::Vector2f(SIZE_OF_CELL, SIZE_OF_CELL));
    rectTemplate.setFillColor(sf::Color(WHITE));
    
    shapes.clear();

    for(int i = 0; i < matrix.size(); i++){
        for(int j = 0; j < matrix[0].size(); j++){
            if(matrix[i][j]){
                rectTemplate.setPosition(j * SIZE_OF_CELL, i * SIZE_OF_CELL);
                shapes.push_back(rectTemplate);
            }
        }
    }

    return;
}

void fillMatrix(std::vector< std::vector<bool> >& matrix){
    srand(time(0));
    
    int matrixHeight = matrix.size();
    int matrixWidth  = matrix[0].size();

    matrix[matrixHeight / 2][matrixWidth / 2 - 1] = true;
    matrix[matrixHeight / 2][matrixWidth / 2]     = true;
    matrix[matrixHeight / 2][matrixWidth / 2 + 1] = true;

    return;
}
 
void drawRectangles(const std::vector< sf::RectangleShape >& rectangles, sf::RenderWindow* window){
    for(auto rectangle : rectangles)
        window->draw(rectangle);
    
    return;
}

bool lives(char count){
    if(count < 2 || count > 3)  return false;
    return true;
}

bool updateSingleCell(int i, int j, std::vector< std::vector<bool> >& matrix){
    char count = 0; // matrixHeight = matrix.size(), matrixWidth = matrix[0].size();

    count = matrix[i - 1][j - 1] + matrix[i - 1][j] + matrix[i - 1][j + 1] +
                    matrix[i][j - 1] +     matrix[i][j + 1] +
                    matrix[i + 1][j - 1] + matrix[i + 1][j] + matrix[i + 1][j + 1];

    return lives(count);
}

void printAllLiveCells(std::vector< std::vector<bool> >& matrix){
    for(int i = 1; i < matrix.size() - 1; i++)
        for(int j = 1; j < matrix[0].size() - 1; j++)
            if(matrix[i][j])    std::cout << i << " " << j << " " << updateSingleCell(i, j, matrix)  << std::endl;
}

void updateCellMatrix(std::vector< std::vector<bool>>& currMatrix){
    // Special cases are edge cells, so we update the 4 corner cells first, then the 4 edges, and then we update
    // the inner matrix regularly with a double for loop. We can't overwrite the matrix before calculating the next
    // state of all the cells, based on the current matrix
    
    int matrixHeight = currMatrix.size(), matrixWidth = currMatrix[0].size();

    // We allocate space for a new matrix with the same dimensions as the current matrix
    std::vector< std::vector<bool> > newMatrix(matrixHeight, std::vector<bool>(matrixWidth, false));

    char count;

    for(int i = 1; i < matrixHeight - 1; i++){
        for(int j = 1; j < matrixWidth - 1; j++){
            newMatrix[i][j] = updateSingleCell(i, j,currMatrix); 
        }
    }

    //Copy the new matrix into "current" matrix
    for(int i = 1; i < matrixHeight - 1; i++){
        for(int j = 1; j < matrixWidth - 1; j++){
            currMatrix[i][j] = newMatrix[i][j];
        }
    }

    return;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(600, 600), "Conway's Game of Life", sf::Style::Fullscreen);

    // Load font
    sf::Font font;
    if(!font.loadFromFile("NotoSansCJK-Regular.ttc")) std::cout << "Failed to load font from file" << std::endl;
    else    std::cout << "Successfully loaded font from file" << std::endl;

    // Get height and width of screen 
    int screenHeight, screenWidth;
    screenHeight = sf::VideoMode::getDesktopMode().height;
    screenWidth  = sf::VideoMode::getDesktopMode().width;

    // Create cell matrix
    size_t matrixWidth = screenWidth / SIZE_OF_CELL, matrixHeight = screenHeight / SIZE_OF_CELL;
    std::vector< std::vector<bool> > mainMatrix(matrixHeight + 2, std::vector<bool>(matrixWidth + 2, false));

    // Fill matrix at the beginning
    fillMatrix(mainMatrix);

    bool gamePaused = false;

    // Main game loop
    while (window.isOpen())
    {
        // Handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch(event.type){
                // Handle window close
                case sf::Event::Closed:
                    window.close();
                    break;

                //Handle key presses
                case sf::Event::KeyPressed:
                    switch(event.key.code){

                        // Quit game
                        case sf::Keyboard::Escape:
                            window.close();
                            break;

                        // Pause game
                        case sf::Keyboard::Space:
                            gamePaused = (gamePaused == true)? false : true;
                            break;

                        // Dont handle any other key presses
                        default:
                            break;
                    }
                    break;
                
                default:
                    break;
            }
        }

        // Draw new frame
        window.clear();

        sf::Text pauseText;
        pauseText.setFont(font);
        pauseText.setCharacterSize(24);
        pauseText.setPosition(screenWidth/2 - 50, screenHeight/2 - 25);
        if(gamePaused)  pauseText.setString("Paused");
        else            pauseText.setString("");
        pauseText.setFillColor(sf::Color(WHITE));

        if(!gamePaused){
            std::vector< sf::RectangleShape > liveCellShapes;
            getShapesFromMatrix(mainMatrix, liveCellShapes);
            drawRectangles(liveCellShapes, &window);
            updateCellMatrix(mainMatrix);
        }

        window.draw(pauseText);

        window.display();
    }

    return 0;
}
