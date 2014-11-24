#pragma once

#include <vector>

const std::string USAGE = "USAGE: ./huffman [-c|-u] [-f file_name] [-o output]";

struct Node
{
    Node();
    ~Node();
    Node(Node *pNode);
    Node(int cnt, char ch);

    char ch;
    int cnt;
    Node * left_child;
    Node * right_child;

    static bool compare(Node a, Node b);
};

struct Reader
{
    char * in_file;
    std::vector<char> in_string;

    void read_in_string();

    Reader(char *in);
    Reader();
};

struct Writer
{
    char * out_file;
    std::vector<char> out_string;

    void send();

    Writer();
    Writer(char *out);
};

struct Huffman
{
    static int const MAX_NUM_BY_CHAR = 256;

    std::vector<Node> leaves;

    int cast_char_to_int(char ch);
    char cast_int_to_char(int n);
    int cast_char_to_real_int(char ch);


    static void print_usage();

    Huffman(char *in, char *out);
    Huffman() ;
    ~Huffman();

protected:
    Reader reader;
    Writer writer;

    std::string code_by_char[MAX_NUM_BY_CHAR];
    Node * root;

    int OFFSET = 128;

    void clear_code_by_char();
    //void print_code_by_char(std::string code_by_char[]);

    char cast_real_int_to_char(int n);
};

struct Compression:public Huffman {
    Compression(char *in, char *out);
    void compression();

private:
    void get_leaves();

    void get_tree(std::vector<Node> leaves);

    void get_code_by_char();

    void rec_code_by_char(Node node, std::string cur_string);

    void put_code_by_char();

    void put_massage();

    void put_string_by_number(int n);

    int get_size_msg();

    char get_char_from_8_byte(int begin, std::vector<bool> &byte);
};

struct Decompression:public Huffman {
    Decompression(char *in, char *out);
    void decompression();

private:

    void get_tree();

    int get_code_by_char();

    void add_new_leave(int id_char, int l, std::string string, Node * node);

    void put_decode_massage(int begin);

    void get_vector_byte_by_char(std::vector<bool> & in_byte, int left_byte, char c);

    int get_letter(int position, std::vector<bool> & in_byte, Node * node);

    void get_vector_byte_by_char(std::vector<bool> &in_byte, char c);
};