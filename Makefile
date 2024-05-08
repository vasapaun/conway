conway:
	g++ main.cpp -o conway -lsfml-graphics -lsfml-window -lsfml-system

install: conway
	mkdir -p /usr/local/bin/
	cp conway /usr/local/bin/
	chmod 755 /usr/local/bin/conway
	echo "conway installed!"
clean:
	rm -f *.o
uninstall:
	rm -f /usr/local/bin/conway

.PHONY: install clean uninstall
