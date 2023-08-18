override CFLAGS := -pedantic-errors -Wall -Wextra $(CFLAGS)

.PHONY: all
all: main

main: main.o mu_utf8.o mu_properties.o

mu_utf8.o: mu_utf8.h

mu_properties.o: mu_properties.h mu_properties.inc

mu_properties.inc: generate_mu_properties.py ucd.all.grouped.xml
	python3 generate_mu_properties.py

ucd.all.grouped.xml: ucd.all.grouped.zip
	unzip -uo ucd.all.grouped.zip

ucd.all.grouped.zip:
	wget --timestamping https://www.unicode.org/Public/UCD/latest/ucdxml/ucd.all.grouped.zip

.PHONY: clean
clean: mostlyclean
	rm -f ucd.all.grouped.zip

.PHONY: mostlyclean
mostlyclean:
	rm -f ucd.all.grouped.xml mu_properties.inc *.o main
