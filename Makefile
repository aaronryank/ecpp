OBJ := src/token.o src/tokenizer.o src/string.o src/main.o src/pp-setup.o src/preprocessor.o src/rules.o src/pretty.o src/uncomment.o
OBJ_ := src/token.o src/tokenizer.o src/string.o src/preprocessor.o src/pp-setup.o src/rules.o
CC := gcc
CFLAGS := -g -Wall -O3

ECPP := out/ecpp
UNCOMMENT := out/uncomment

all: $(OBJ)
	$(CC) $(OBJ) $(CFLAGS) -o $(ECPP)

uncomment:
	$(CC) -DUNCOMMENT_MAIN src/uncomment.c $(CFLAGS) -o $(UNCOMMENT)

clean:
	rm $(OBJ)

pretty: $(OBJ_)
	$(CC) $(CFLAGS) $(OBJ_) -DPRETTY_EXEC src/pretty.c -o out/pretty
