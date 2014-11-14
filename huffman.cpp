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


}



void Huffman::decompression()
{

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
