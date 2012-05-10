MC = metil_comp -Isrc -Iext/Celo/src -Iext/PrepArg/src --comp-dir compilations -g3 -Wall
INSTALL_DIR = `pwd`

all: cmp

# compilation of the soda server
cmp: install_dir ext/Celo ext/PrepArg src/Soda/Com/HttpRequest_Supervision_parser.h src/Soda/Com/SodaRequest_Public_parser.h
	${MC} -o soda -ne src/Soda/soda.cpp

#
test_%:
	${MC} tests/$@.cpp

%.h: %.sipe ext/Sipe
	make -C ext/Sipe
	ext/Sipe/sipe -o $@ $<

ext/Sipe:
	mkdir -p ext; cd ext; test -e Sipe || ( test -e ../../Sipe && ln -s `pwd`/../../Sipe . ) || git clone git@github.com:hleclerc/Sipe.git

ext/Celo:
	mkdir -p ext; cd ext; test -e Celo || ( test -e ../../Celo && ln -s `pwd`/../../Celo . ) || git clone git@github.com:hleclerc/Celo.git

ext/PrepArg:
	mkdir -p ext; cd ext; test -e PrepArg || ( test -e ../../PrepArg && ln -s `pwd`/../../PrepArg . ) || git clone git@github.com:hleclerc/PrepArg.git

install_dir:
	export D="#define INSTALL_DIR \"${INSTALL_DIR}\""; grep "$$D" src/Soda/InstallDir.h || echo "$$D" > src/Soda/InstallDir.h

.PHONY: cmp exe install_dir
