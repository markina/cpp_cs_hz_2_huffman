#include <iostream>
#include "huffman.hpp"

int main(int argc, char ** argv)
{
    if(argc < 6) {
        Huffman::print_usage();
        return 1;
    }

    bool is_decompression;
    if(std::string(argv[1]) == "-u") {
        is_decompression = true;
    } else if (std::string(argv[1]) == "-c") {
        is_decompression = false;
    } else {
        Huffman::print_usage();
        return 1;
    }

    char * in;
    char * out;
    if((std::string(argv[2]) == "-o") && std::string(argv[4]) == "-f") {
        in = argv[5];
        out = argv[3];
    } else if ((std::string(argv[2]) == "-f") && (std::string(argv[4]) == "-o")) {
        in = argv[3];
        out = argv[5];
    } else {
        Huffman::print_usage();
        return 1;
    }

    try
    {
        if(is_decompression) {
            Decompression archiver(in, out);
            archiver.decompression();
        }
        else {
            Compression archiver(in, out);
            archiver.compression();
        }
    } catch (ExceptionFileNotFound exception) {
        std::cout << exception.what();
        return 1;
    } catch (ExceptionSameCharactersNotRead exception) {
        std::cout << exception.what();
        return 1;
    }



    return 0;
}