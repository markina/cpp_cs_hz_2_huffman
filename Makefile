compile:
	g++ -Wall -g -o huffman main.cpp huffman.cpp

run:
	./huffman -c -f myfile.txt -o result.bin

valgrind_invalid_operation:
	valgrind ./huffman -c -f myfile.txt -o result.bin
