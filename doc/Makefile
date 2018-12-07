
APP_NAME=bsm-shell
BASE_DIR=$(CURDIR)

##
## To use this, in yourfile.c do something like this:
## printf("this program was compiled from SVN revision %s\n",SVN_REV);
##
SVNDEF =        -D'SVN_REV="$(shell svnversion -n .)"'


all: doc

doc: ./dd/html/index.html

./dd/html/index.html: dd.dox
	doxygen dd.dox

./dd/$(APP_NAME).pdf: dd.dox ./dd/html/index.html
	@cd ./dd/latex; make && mv refman.pdf ../$(APP_NAME).pdf

clean :
	@rm -fr ./dd

.PHONY : clean doc

#-include $(DEPS)
