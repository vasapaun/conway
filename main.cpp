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
#define TIME_PAUSE              0
#define WHITE                   255, 255, 255
#define BLACK                   0  , 0  , 0

void getShapesFromMatrix(std::vector< std::vector<bool> >& matrix, std::vector< sf::RectangleShape >& shapes, char cellSize){
    sf::RectangleShape rectTemplate(sf::Vector2f(cellSize, cellSize));
    rectTemplate.setFillColor(sf::Color(WHITE));
    
    shapes.clear();

    for(int i = 0; i < matrix.size(); i++){
        for(int j = 0; j < matrix[0].size(); j++){
            if(matrix[i][j]){
                rectTemplate.setPosition(j * cellSize, i * cellSize);
                shapes.push_back(rectTemplate);
            }
        }
    }

    return;
}

void randomFillMatrix(std::vector< std::vector<bool> >& matrix, int cellNumber){
    srand(time(0));
    
    int matrixHeight = matrix.size();
    int matrixWidth  = matrix[0].size();
    int rnd, h, w;
    
    for(int i = 0; i < cellNumber; i++){
        rnd = rand(), w = rand() % (matrixWidth - 2) + 1, h = rand() % (matrixHeight - 2) + 1;
        matrix[h][w] = true;
    }

    return;
}
 
void drawRectangles(const std::vector< sf::RectangleShape >& rectangles, sf::RenderWindow* window){
    for(auto rectangle : rectangles)
        window->draw(rectangle);
    
    return;
}

bool lives(char count, bool alive){
    if(alive && (count == 2 || count ==3))  return true;
    if(!alive && count == 3)    return true;
    return false;
}

bool updateSingleCell(int i, int j, std::vector< std::vector<bool> >& matrix){
    char count = 0; // matrixHeight = matrix.size(), matrixWidth = matrix[0].size();

    count = matrix[i - 1][j - 1] + matrix[i - 1][j] + matrix[i - 1][j + 1] +
                    matrix[i][j - 1] +     matrix[i][j + 1] +
                    matrix[i + 1][j - 1] + matrix[i + 1][j] + matrix[i + 1][j + 1];

    return lives(count, matrix[i][j]);
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
    char cellSize = SIZE_OF_CELL;
    int cellNumber = NUM_OF_STARTING_CELLS, timeInterval = TIME_PAUSE;
    std::string cellSizeInput, cellNumberInput, timeIntervalInput;
    bool choseDefault = false;
    //Input all variables like cell size, simulation speed etc.
    std::cout <<    "Welcome to Conway's Game of Life.\n You will now be prompted to input parameters that dictate how the simulation will take place.\n If you would like to leave a parameter default, input 'd'.\n If you would like to leave all the parameters default, input 'D'.\n You can exit the simulation at any point by clicking Escape.\n You can pause the simulation at any point by clicking Space.\n\n";

    if(!choseDefault){
        std::cout << "Set the size of the cells [in pixels]: \n";
        std::cin >> cellSizeInput;
        if(cellSizeInput == "D") choseDefault = true;
        else if(cellSizeInput != "d") cellSize = std::stoi(cellSizeInput);
    }
    if(!choseDefault){
        std::cout << "Set the number of cells to be randomly generated at the start of the simulation: \n";
        std::cin >> cellNumberInput;
        if(cellNumberInput == "D") choseDefault = true;
        else if(cellNumberInput != "d") cellNumber = std::stoi(cellNumberInput);
    }
    if(!choseDefault){
        std::cout << "Set the time interval between frames [in milliseconds]: \n";
        std::cin >> timeIntervalInput;
        if(timeIntervalInput == "D") choseDefault = true;
        else if(timeIntervalInput != "d") timeInterval = std::stoi(timeIntervalInput) * 1000; // usleep takes time interval in microseconds
    }

    sf::RenderWindow window(sf::VideoMode(600, 600), "Conway's Game of Life", sf::Style::Fullscreen);

    // Get height and width of screen 
    int screenHeight, screenWidth;
    screenHeight = sf::VideoMode::getDesktopMode().height;
    screenWidth  = sf::VideoMode::getDesktopMode().width;

    // Create cell matrix
    size_t matrixWidth = screenWidth / cellSize, matrixHeight = screenHeight / cellSize;
    std::vector< std::vector<bool> > mainMatrix(matrixHeight + 2, std::vector<bool>(matrixWidth + 2, false));

    // Fill matrix at the beginning
    randomFillMatrix(mainMatrix, cellNumber);

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

        if(!gamePaused){
            std::vector< sf::RectangleShape > liveCellShapes;
            getShapesFromMatrix(mainMatrix, liveCellShapes, cellSize);
            drawRectangles(liveCellShapes, &window);
            updateCellMatrix(mainMatrix);
        }

        window.display();

        usleep(timeInterval);
    }

    return 0;
}
