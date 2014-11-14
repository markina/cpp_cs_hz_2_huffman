#include <vector>

#pragma once

const std::string USAGE = "USAGE: ./huffman [-c|-u] [-f file_name] [-o output]";

class Node {
public:
    Node(Node *pNode);

    std::string string;
    int cnt;
    Node * left_child;
    Node * right_child;

    Node();

    Node(int cnt, std::string string);

    ~Node();
};

class Huffman {
    static int const MAX_NUM_BY_CHAR = 256;

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

    int cnt[MAX_NUM_BY_CHAR]{0};

    void read_vector_cnt_by_char();

    std::vector<Node> vector;

    void print_vector_cnt();


};

void print_usage();