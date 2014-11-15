#include <fstream>
#include <stdexcept>
#include "huffman.hpp"
#include <iostream>
#include <cstring>
#include <algorithm>

Node::Node() { string = ""; cnt = 0; left_child = NULL; right_child = NULL; }
Node::Node(int cnt, std::string string)  : cnt(cnt), string(string) { left_child = NULL; right_child = NULL; }
Node::~Node() {}

Huffman::Huffman() { for(int i = 0; i < MAX_NUM_BY_CHAR; ++i) {cnt[i] = 0; } }
Huffman::~Huffman() {
    //delete [] buffer;
}

void Huffman::compression(char * in, char * out)
{
    in_file = in;
    out_file = out;

    read_buffer();
    create_vector_cnt_by_char();
    std::sort(leaves.begin(), leaves.end(), compare);
    print_vector_cnt();

    get_tree();

    for(int i = 0;  i < MAX_NUM_BY_CHAR; i++) {
        code_by_char[i] = "";
    }

    count_code_by_char(root, "");
    print_code_by_char();
    send_code_by_char();
    send_message();
}



void Huffman::decompression(char * in, char * out)
{

    in_file = in;
    out_file = out;

    read_buffer();
    int begin_code=  extract_vector_cnt_by_char();

    //char * code = buffer + begin_code;
    std::string code = "011";

    std::string result = "";

    for (size_t i = 0; i < code.length(); ) {
        char c = get_letter(i, root, code);
        result += c;
        i += (int)code_by_char[c].length();
    }

    std::cout << result << "!!!!!!!!!!!!!!!!!!!\n";

}

void Huffman::read_buffer() {
    std::ifstream in("/home/rita/studies/cppcs/cpp_cs_hz_2_huffman/res/in.txt", std::ifstream::binary);
    if(in)
    {
        in.seekg(0, in.end);
        length_buffer = (int) in.tellg();
        in.seekg(0, in.beg);

        buffer = new char[length_buffer];

        //std::cout << "Reading " << length_buffer << " characters... ";

        in.read(buffer, length_buffer);

        if (in)
        {
            //std::cout << "all characters read successfully.\n";
        }
        else
        {
            //std::cout << "error: only " << in.gcount() << " could be read"; //todo
        }
        in.close();
    }
}


void Huffman::create_vector_cnt_by_char()
{

    for(int i = 0; i < length_buffer; ++i) {
        int num_by_char = buffer[i];
        cnt[num_by_char]++;
    }

    for(int i = 0; i < MAX_NUM_BY_CHAR; i++) {
        if(cnt[i] != 0) {
            char t = (char) i;
            std::string s (1, t);
            Node node(cnt[i], s);
            leaves.push_back(node);
        }
    }

    print_vector_cnt();

}

void Huffman::print_vector_cnt()
{
    for(int i = 0; i < leaves.size(); i++) {
        //std::cout << leaves[i].string << "!!!" << leaves[i].cnt << std::endl;
    }
}


Node::Node(Node *pNode)
{
    cnt = pNode->cnt;
    left_child = pNode->left_child;
    right_child = pNode->right_child;
    string = pNode->string;
}

void Huffman::get_tree()
{
    if(leaves.size() == 0) {
        return;
    }

    size_t iterrator = 0;
    while(iterrator < leaves.size() - 1) {
        Node first = new Node (leaves[iterrator]);
        Node second = new Node (leaves[iterrator + 1]);
        Node *newNode = new Node(first.cnt + second.cnt, first.string + second.string);
        newNode->left_child = new Node(first);
        newNode->right_child = new Node(second);
        leaves[iterrator + 1] = new Node (newNode);
        iterrator++;
        for(size_t j = iterrator; j < leaves.size() - 1; j++) {
            if(leaves[j].cnt > leaves[j+1].cnt) {
                std::swap(leaves[j], leaves[j+1]);
            } else {
                break;
            }
        }
        sort(leaves.begin() + iterrator, leaves.end(), compare); //todo good sort
    }

    root = new Node(leaves[iterrator]);
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
    //std::cout << "\n";
    for(int i = 0; i < MAX_NUM_BY_CHAR; i++) {
        if(code_by_char[i].length() != 0) {
            std::cout << (char)(i) << "->" << code_by_char[i] << std::endl;
        }
    }
}

void Huffman::send_code_by_char()
{
   // ... todo ??
}

Node Huffman::get_tree_from_file()
{
    //todo
    return root;
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

    return '?';

}

bool compare(Node a, Node b) { return a.cnt < b.cnt; }
void print_usage() {std::cout << USAGE << std::endl; }

void Huffman::send_message()
{

}

int Huffman::extract_vector_cnt_by_char()
{
    //todo
    return 0;
}
