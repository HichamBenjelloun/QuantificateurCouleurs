# Makefile
# Benjelloun Hicham 01 Jan 2013
# v4.0

# Identification
AUTHOR=benjelloun

# Commandes
CC=gcc -O3 -g -Wall -pedantic
CFLAG=-limage -lm
CPFLAG=
RM=rm -rf

# Repertoires
REF=./
HEADERS=$(REF)include/
SRC=$(REF)src/
OBJ=$(REF)obj/
LIB=$(REF)lib/
BIN=$(REF)bin/
DOC=$(REF)doc/

INCLUDE=-I$(HEADERS)

# Modules
MOD_IMAGE=$(SRC)image/
MOD_HISTO=$(SRC)histo/
MOD_QUANTIFICATION=$(SRC)quantification/
APP=$(SRC)app/

LIBS=-L$(LIB)

# Construction de l'executable
all : $(OBJ)histogram.o $(OBJ)quantification.o $(OBJ)reduire.o
	$(CC) $(LIBS) -o $(BIN)reduire $^ $(CFLAG)

# Compilation des librairies
libimage : 
	$(CC) $(INCLUDE) -o $(OBJ)image.o $(MOD_IMAGE)image.c -c
	ar -rv $(LIB)libimage.a $(OBJ)image.o

$(OBJ)histogram.o: $(MOD_HISTO)histogram.c
	$(CC) $(INCLUDE) -o $(OBJ)histogram.o $(MOD_HISTO)histogram.c -c

$(OBJ)quantification.o: $(MOD_QUANTIFICATION)quantification.c
	$(CC) $(INCLUDE) -o $(OBJ)quantification.o $(MOD_QUANTIFICATION)quantification.c -c

$(OBJ)reduire.o: $(APP)reduire.c
	$(CC) $(INCLUDE) -o $(OBJ)reduire.o $(APP)reduire.c -c

$(OBJ)ppmhist.o: $(APP)ppmhist.c
	$(CC) $(INCLUDE) -o $(OBJ)ppmhist.o $(APP)ppmhist.c -c

# Edition des liens

ppmhist: $(OBJ)ppmhist.o
	$(CC) $(OBJ)ppmhist.o $(OBJ)histogram.o -o $(BIN)$@ $(LIBS) -limage 


# Documentation
documentation :
	doxygen config

# Nettoyer les fichiers temporaires
clean:
	$(RM) gmon.out *.tar *~ $(SRC)*~ $(APP)*~ $(MOD_IMAGE)*~ $(MOD_HISTO)*~ $(MOD_QUANTIFICATION)*~ $(HEADERS)*~ $(OBJ)* $(BIN)*

# Archive
archive :
	tar -cvf $(AUTHOR)_projet.tar *

# Tarball
tarball : clean archive
