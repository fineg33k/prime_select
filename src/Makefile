PREFIX = /usr/local

all: prime_switch.c
	gcc -o prime_switch prime_switch.c -Wall `pkg-config --cflags --libs gtk+-3.0` -export-dynamic
clean:
	$(RM) myprog

.PHONY: install
install: prime_switch
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp $< $(DESTDIR)$(PREFIX)/bin/prime_switch
	mkdir -p /etc/prime_switch
	cp prime_switch.glade /etc/prime_switch/
