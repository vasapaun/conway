libSFML is needed for this project.

libSFML linux installation:
    
    Using apt-get:
    sudo apt-get install libsfml-dev

Project compilation:
    
    If libSFML is installed in the correct path:
    g++ main.cpp -o prog -lsfml-graphics -lsfml-window -lsfml-system

    If libSFML is not installed in the correct path:
    g++ main.cpp -o prog -L<sfml-install-path>/lib -lsfml-graphics -lsfml-window -lsfml-system

In case of problems, refer to https://www.sfml-dev.org/tutorials/2.6/
