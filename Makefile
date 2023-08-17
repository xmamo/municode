override CFLAGS := -pedantic-errors -Wall -Wextra $(CFLAGS)

.PHONY: all
all: main

main: main.o unicode.o unicode_cp_properties.o

unicode_cp_properties.o: unicode.h unicode_cp_properties.inc

unicode_cp_properties.inc: generate_unicode_cp_properties.py ucd.all.grouped.xml
	python3 generate_unicode_cp_properties.py

ucd.all.grouped.xml: ucd.all.grouped.zip
	unzip -uo ucd.all.grouped.zip

ucd.all.grouped.zip:
	wget --timestamping https://www.unicode.org/Public/UCD/latest/ucdxml/ucd.all.grouped.zip

.PHONY: clean
clean:
	rm -f ucd.all.grouped.xml unicode_cp_properties.inc *.o main

.PHONY: clean!
clean!: clean
	rm -f ucd.all.grouped.zip
