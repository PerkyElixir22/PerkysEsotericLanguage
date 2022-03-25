CC=clang++
CARGS=--std=c++20 -Wall -Werror
SOURCE=./source
BUILD=./build
OUTFILE=main

compile:
	make clean
	-mkdir $(BUILD)
	$(CC) $(CARGS) $(SOURCE)/*.cpp -o $(BUILD)/$(OUTFILE)

clean:
	-rmdir $(BUILD) -rf