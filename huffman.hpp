#pragma once

#include <vector>

const std::string USAGE = "USAGE: ./huffman [-c|-u] [-f file_name] [-o output]";

struct Node
{
    Node();
    ~Node();
    Node(Node *pNode);
    Node(int cnt, char ch) ;

    int cnt;
    char ch ;
    Node * left_child;
    Node * right_child;

    static bool compare(const Node a, const Node b) ;
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

    void send() const ;

    Writer();
    Writer(char *out);
};

struct Huffman
{
    static size_t const MAX_NUM_BY_CHAR = 256;
    static size_t const OFFSET = 128;

    static void print_usage();

    Huffman(char *in, char *out);
    Huffman() ;
    ~Huffman();

protected:
    Reader reader;
    Writer writer;

    std::string code_by_char[MAX_NUM_BY_CHAR];
    Node * root;
    std::vector<Node> leaves;

    void clear_code_by_char();

    int cast_char_to_int(const char ch) const;
    char cast_int_to_char(const int n) const;
    int cast_char_to_real_int(const char ch) const;
    char cast_real_int_to_char(int n);

private:
    void delete_tree();
    void rec_delete_tree(Node * pNode);
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
    void put_decode_massage(size_t begin);
    void get_vector_byte_by_char(std::vector<bool> & in_byte, int left_byte, char c);
    size_t get_letter(size_t position, std::vector<bool> & in_byte, Node * node);
    void get_vector_byte_by_char(std::vector<bool> &in_byte, char c);
};

struct ExceptionFileNotFound :public std::exception{
    virtual const char *what() const throw();
};

struct ExceptionSameCharactersNotRead :public std::exception{
    virtual const char *what() const throw();
};
