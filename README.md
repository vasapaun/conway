# Conway's Game of Life 

## Videos
*Contains flashing images*

https://github.com/user-attachments/assets/24c96b9e-0afb-418b-be9d-2c6cf4e0bc2d

https://github.com/user-attachments/assets/ee9d745a-0640-4faa-ae58-974524848719

## IMPORTANT 
This project was built using SFML 2.6 

The newest SFML 3.0 is not backwards-compatible.

To compile this project, use SFML 2.6

Alternatively, you can download the executable from the releases page [soon].

## Description
Simulation of Conway's Game of Life written in C++, using libSFML

The main simulation is written in main.cpp, while an interesting variation (feature, not a bug) is written in variation.cpp

## Dependencies
- **libSFML** is needed for this project.

- **libSFML** linux installation:

    Using apt:
    `sudo apt install libsfml-dev`

## Project compilation:

- **Automatic**: navigate to *conway* directory and run `make conway`, `make variation` or `make all`

- **Manual**: If libSFML is not installed in the correct path

    ``` g++ main.cpp -o conway -L<sfml-install-path>/lib -lsfml-graphics -lsfml-window -lsfml-system ```
    ``` g++ variation.cpp -o variation -L<sfml-install-path>/lib -lsfml-graphics -lsfml-window -lsfml-system ```

In case of problems, refer to https://www.sfml-dev.org/tutorials/2.6/
