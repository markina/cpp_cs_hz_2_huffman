#pragma once

const std::string USAGE = "USAGE: ./huffman [-c|-u] [-f file_name] [-o output]";

class Node {
public:
    std::string string;
    int cnt;
    Node * left_child;
    Node * right_child;

    Node();

    ~Node();
};

class Huffman {
public:
    bool is_decompression;
    char * in_file;
    char * out_file;

    void put_files(char * in, char * out);

    void run();

    Huffman();

    ~Huffman();

private:
    void compression();
    void decompression();
};

void print_usage();