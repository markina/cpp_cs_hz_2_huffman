#include <vector>

#pragma once

const std::string USAGE = "USAGE: ./huffman [-c|-u] [-f file_name] [-o output]";

struct Node {

    Node();
    ~Node();

    Node(Node *pNode);
    Node(int cnt, std::string string);

    std::string string;
    int cnt;
    Node * left_child;
    Node * right_child;
};

struct Huffman {
    static int const MAX_NUM_BY_CHAR = 256;

public:
    void compression(char * in, char * out);
    void decompression(char * in, char * out);

    Huffman() ;
    ~Huffman();

private:
    char * in_file;
    char * out_file;
    char * buffer;
    char * result_buffer;
    int length_buffer;
    int length_result_buffer;

    int cnt[MAX_NUM_BY_CHAR];

    void create_vector_cnt_by_char();

    std::vector<Node> leaves;
    void get_tree();
    Node root;


    void count_code_by_char(Node, std::string);

    std::string code_by_char[MAX_NUM_BY_CHAR];

    void send_code_by_char();
    Node get_tree_from_file();

    char get_letter(int i, Node node, std::string code);

    void print_vector_cnt();
    void print_code_by_char();

    void send_message();

    void read_buffer();

    int extract_vector_cnt_by_char();
};

void print_usage();
bool compare(Node a, Node b);