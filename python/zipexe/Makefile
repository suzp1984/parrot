all: zipexe.exe

clean: 
	rm -f zipexe.exe

PREFIX=/usr/local
BINDIR=$(PREFIX)/bin
MANDIR=$(PREFIX)/man
PYTHON=/usr/bin/env python

.PHONY: all clean zipexe.exe

zipexe.exe: zipexe/*.py
	zip --quiet zipexe zipexe/*.py
	zip --quiet --junk-paths zipexe zipexe/__main__.py
	echo '#!$(PYTHON)' > zipexe.exe
	cat zipexe.zip >> zipexe.exe
	rm zipexe.zip
	chmod a+x zipexe.exe
