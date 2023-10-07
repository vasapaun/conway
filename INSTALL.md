## Dependencies
- **libSFML** is needed for this project.

- **libSFML** linux installation:

    Using apt:
    `sudo apt install libsfml-dev`

## Project compilation:

- **Automatic**: navigate to *conway*  directory and run `make install` (you might need to prefix it with `sudo`)

- **Manual**: If libSFML is not installed in the correct path

    ``` g++ main.cpp -o conway -L<sfml-install-path>/lib -lsfml-graphics -lsfml-window -lsfml-system ```

In case of problems, refer to https://www.sfml-dev.org/tutorials/2.6/
