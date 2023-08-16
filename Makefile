.PHONY: all
all: main

main: main.o unicode.o unicode_database.o

unicode_database.o: unicode.h unicode_database.inc

unicode_database.inc: generate_unicode_database.py ucd.all.grouped.xml
	python3 generate_unicode_database.py

ucd.all.grouped.xml: ucd.all.grouped.zip
	unzip -uo ucd.all.grouped.zip

ucd.all.grouped.zip:
	wget --timestamping https://www.unicode.org/Public/UCD/latest/ucdxml/ucd.all.grouped.zip

.PHONY: clean
clean:
	rm -f ucd.all.grouped.xml unicode_database.inc *.o main

.PHONY: clean!
clean!: clean
	rm -f ucd.all.grouped.zip
