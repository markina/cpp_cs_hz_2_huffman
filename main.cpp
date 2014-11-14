#include <iostream>
#include <string.h>
#include "huffman.hpp"

int main(int argc, char ** argv)
{
    Huffman archiver;

    if(argc < 6) {
        print_usage();
        return 1;
    }

    if(std::string(argv[1]) == "-u") {
        archiver.is_decompression = true;
    } else if (std::string(argv[1]) == "-c") {
        archiver.is_decompression = false;
    } else {
        print_usage();
        return 1;
    }

    if((std::string(argv[2]) == "-o") && std::string(argv[4]) == "-f") {
        archiver.put_files(argv[5], argv[3]);
    } else if ((std::string(argv[2]) == "-f") && (std::string(argv[4]) == "-o")) {
        archiver.put_files(argv[3], argv[5]);
    } else {
        print_usage();
        return 1;
    }

    archiver.run();





    return 0;
}