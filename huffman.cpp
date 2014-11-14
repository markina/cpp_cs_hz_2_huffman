#include <fstream>
#include <stdexcept>
#include "huffman.hpp"
#include <iostream>
#include <cstring>

Node::Node()
{
    cnt = 0;
}


void print_usage()
{
    std::cout << USAGE << std::endl;
}

Huffman::Huffman()
{

}

Node::~Node()
{

}

Huffman::~Huffman()
{

}

void Huffman::put_files(char *in, char *out)
{
    in_file = in;
    out_file = out;
}

void Huffman::run()
{
    if(is_decompression) {
        decompression();
    }
    else {
        compression();
    }

}

void Huffman::compression()
{
    
}

void Huffman::decompression()
{

}
