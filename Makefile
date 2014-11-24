compile:
	g++ -std=c++11 -Wall -g -o huffman main.cpp huffman.cpp

run_compression:
	./huffman -c -f my_in_file.txt -o my_out_file.txt

run_decompression:
	./huffman -u -o result.txt -f my_out_file.txt

valgrind_c:
	valgrind ./huffman -c -f my_in_file.txt -o my_out_file.txt

valgrind_u:
	valgrind ./huffman -u -f my_out_file.txt -o my_in_file.txt

valgrind_c_photo:
	valgrind ./huffman -c -f in_photo.jpg -o zip_photo.zzz

valgrind_u_photo:
	valgrind ./huffman -u -f zip_photo.zzz -o out_photo.jpg

run_c_photo:
	./huffman -c -f in_photo.jpg -o zip_photo.zzz

run_u_photo:
	./huffman -u -f zip_photo.zzz -o out_photo.jpg

run_c_empty:
	./huffman -c -f empty.in -o empty.zzz

run_u_empty:
	./huffman -u -f empty.zzz -o empty.out

valgrind_c_empty:
	valgrind ./huffman -c -f empty.in -o empty.zzz

valgrind_u_empty:
	valgrind ./huffman -u -f empty.zzz -o empty.out

not_exit:
	./huffman -c -f a.in -o a.out

valgrind_not_exit:
	valgrind ./huffman -c -f a.in -o a.out

bad_usage:
	./huffman -t -f empty.in -o empty.zzz

valgrind_bad_usage:
	valgrind ./huffman -t -f empty.in -o empty.zzz