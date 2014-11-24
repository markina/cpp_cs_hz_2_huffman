compile:
	g++ -std=c++11 -Wall -g -o huffman main.cpp huffman.cpp

run_compression:
	./huffman -c -f my_in_file.txt -o my_out_file.txt

run_decompression:
	./huffman -u -o my_in_file.txt -f my_out_file.txt

valgrind_c:
	valgrind ./huffman -c -f my_in_file.txt -o my_out_file.txt

valgrind_u:
	valgrind ./huffman -u -f my_out_file.txt -o my_in_file.txt
