OBJ    := src/token.o src/tokenizer.o src/string.o src/main.o src/pp-parse.o src/preprocessor.o src/rules.o src/pretty.o src/uncomment.o
OBJ_   := src/token.o src/tokenizer.o src/string.o src/pp-parse.o src/preprocessor.o src/rules.o src/uncomment.o
CC     := gcc
CFLAGS := -coverage -g -Wall -O3 -ansi

.PHONY: all uncomment clean pretty outdir

OUTDIR    := out/
ECPP      := out/ecpp
UNCOMMENT := out/uncomment

all: outdir $(OBJ)
	$(CC) $(OBJ) $(CFLAGS) -o $(ECPP)

outdir:
	mkdir -p $(OUTDIR)

uncomment:
	$(CC) -DUNCOMMENT_MAIN src/token.c src/uncomment.c $(CFLAGS) -o $(UNCOMMENT)

clean:
	rm src/*.o

pretty: $(OBJ_)
	$(CC) $(CFLAGS) $(OBJ_) -DPRETTY_EXEC src/pretty.c -o out/pretty
