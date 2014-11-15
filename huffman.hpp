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

    std::string code_by_char[MAX_NUM_BY_CHAR];

    void read_vector_cnt_by_char();

    std::vector<Node> vector;

    void print_vector_cnt();


    Node get_tree();

    void count_code_by_char(Node, std::string);

    void print_code_by_char();

    void coding_code_by_char_for_send();

    Node get_tree_from_file();

    char get_letter(int i, Node node, std::string code);
};

void print_usage();