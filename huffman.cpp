#include <fstream>
#include <stdexcept>
#include "huffman.hpp"
#include <iostream>
#include <cstring>
#include <algorithm>

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

bool comp (Node a, Node b) {
    return a.cnt < b.cnt;
}

void Huffman::compression()
{
    read_vector_cnt_by_char();
    std::sort(vector.begin(), vector.end(), comp);
    std::cout<< "\n";
    print_vector_cnt();

    Node root = get_tree();

    for(int i = 0;  i < MAX_NUM_BY_CHAR; i++) {
        code_by_char[i] = "";
    }

    count_code_by_char(root, "");
    print_code_by_char();
    coding_code_by_char_for_send();
}



void Huffman::decompression()
{

////////
    read_vector_cnt_by_char();  ///////
    std::sort(vector.begin(), vector.end(), comp); ////////
    std::cout<< "\n"; ////////////
    print_vector_cnt(); ///////////////

    Node root = get_tree(); /////////////

    for(int i = 0;  i < MAX_NUM_BY_CHAR; i++) { ///
        code_by_char[i] = "";
    }

    count_code_by_char(root, "");//
    print_code_by_char(); //
    coding_code_by_char_for_send(); ///
    //////////
    //compression();

    std::string code = "11110001101111000";

    std::string result = "";

    for (int i = 0; i < code.length(); ) {
        char c = get_letter(i, root, code);
        result += c;
        i += code_by_char[c].length();
    }

    std::cout << result << "!!!!!!!!!!!!!!!!!!!\n";

}

Node::Node(int cnt, std::string string)  : cnt(cnt), string(string)
{
 // (:
}

void Huffman::read_vector_cnt_by_char()
{
    //debug in_file /////////////////////////////
    std::string in_string = "coffecoff";/////////
    /////////////////////////////////////////////

    for(int i = 0; i < in_string.length(); ++i) {
        int num_by_char = in_string[i];
        cnt[num_by_char]++;
    }

    for(int i = 0; i < MAX_NUM_BY_CHAR; i++) {
        if(cnt[i] != 0) {
            char t = (char) i;
            std::string s (1, t);
            Node node(cnt[i], s);
            vector.push_back(node);
        }
    }

    print_vector_cnt();

}

void Huffman::print_vector_cnt()
{
    for(int i = 0; i < vector.size(); i++) {
        std::cout << vector[i].string << "!!!" << vector[i].cnt << std::endl;
    }
}


Node::Node(Node *pNode)
{
    cnt = pNode->cnt;
    left_child = pNode->left_child;
    right_child = pNode->right_child;
    string = pNode->string;
}

Node Huffman::get_tree()
{

    int begin_vector = 0;
    while(begin_vector < vector.size() - 1) {
        Node first = vector[begin_vector];
        Node second = vector[begin_vector + 1];
        Node *newNode = new Node(first.cnt + second.cnt, first.string + second.string);
        newNode->left_child = new Node(first);
        newNode->right_child = new Node(second);
        vector[begin_vector + 1] = new Node (newNode);
        begin_vector++;
        sort(vector.begin() + begin_vector, vector.end(), comp);
    }

    return new Node(vector[begin_vector]);
}

void Huffman::count_code_by_char(Node node, std::string cur_string)
{


    if(node.left_child == NULL) {
        char c = node.string[0];
        code_by_char[(int)c] = cur_string;
        return;
    }

    count_code_by_char(node.left_child, cur_string + "0");
    count_code_by_char(node.right_child, cur_string + "1");
}

void Huffman::print_code_by_char()
{
    std::cout << "\n";
    for(int i = 0; i < MAX_NUM_BY_CHAR; i++) {
        if(code_by_char[i].length() != 0) {
            std::cout << (char)(i) << "!!!" << code_by_char[i] << std::endl;
        }
    }
}

void Huffman::coding_code_by_char_for_send()
{
   // ... todo ??
}

Node Huffman::get_tree_from_file()
{
    /// todo change
    read_vector_cnt_by_char();
    std::sort(vector.begin(), vector.end(), comp);
    std::cout<< "\n";
    print_vector_cnt();

    Node root = get_tree();
    return root;

    ///
}

char Huffman::get_letter(int position, Node node, std::string code)
{
    if(node.left_child == NULL) {
        return node.string[0];
    }
    if(code[position] == '1') {
        return get_letter(position+1, node.right_child, code);
    }
    if(code[position] == '0') {
        return get_letter(position+1, node.left_child, code);
    }
}
