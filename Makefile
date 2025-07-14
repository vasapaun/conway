conway:
	g++ main.cpp -o conway -lsfml-graphics -lsfml-window -lsfml-system

variation:
	g++ variation.cpp -o variation -lsfml-graphics -lsfml-window -lsfml-system

all: conway variation

clean:
	rm -f *.o variation conway

.PHONY: conway variation clean
