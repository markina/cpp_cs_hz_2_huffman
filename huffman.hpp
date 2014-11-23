#include <vector>

#pragma once

const std::string USAGE = "USAGE: ./huffman [-c|-u] [-f file_name] [-o output]";

struct Node {
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
//
//struct Coder {
//    void send_code_by_char();
//    Node get_tree_from_file();
//
//    char get_letter(int i, Node node, std::string code);
//
//
//
//    int get_message();
//
//    int extract_vector_cnt_by_char();
//
//    std::string get_string_by_char(char i);
//
//    void get_out_buffer(std::string []);
//
//    Node extract_tree(ReadWriter writer);
//
//    void write_out_buffer();
//};

struct ReadWriter
{
    ReadWriter();

    ReadWriter(char *in, char *out);

    static int const MAX_NUM_BY_CHAR = 256;

    char * in_file;
    char * out_file;
    std::vector<char> in_string;
    std::vector<char> out_string;

    //std::vector<char> vector_for_buffer;
    void read_in_string();


    void send();
};

struct Huffman {
    Huffman(char *in, char *out);

    static void print_usage();

    std::vector<Node> leaves;

    Huffman() ;
    ~Huffman();

protected:
    ReadWriter read_writer;

    std::string code_by_char[ReadWriter::MAX_NUM_BY_CHAR];
    Node * root;

    //int cnt[ReadWriter::MAX_NUM_BY_CHAR];
    //void get_leaves();
    //std::vector<Node> leaves;
    //void print_vector_cnt(); //
    //void print_code_by_char(); //
    //void rec_code_by_char(Node node, std::string string);


};

struct Compression:public Huffman {
    Compression(char *in, char *out);

    void compression();

private:
    void get_leaves();

    void get_tree(std::vector<Node> leaves);
    void get_code_by_char();

    void rec_code_by_char(Node node, std::string cur_string);

    void get_out_buffer();

    void put_code_by_char();

    void put_massage();

    void put_string_by_number(int n);

    int get_size_msg();
};

struct Decompression:public Huffman {
    Decompression(char *in, char *out);

    void decompression();
    void get_tree();
    int get_code_by_char();

    void add_new_leave(int id_char, int l, std::string string, Node * node);

    void put_decode_massage(int begin);

    void get_vector_byte_by_char(std::vector<bool> & in_byte, int left_byte, char c);

    char get_letter(int position, std::vector<bool> & in_byte, Node * node);
};
