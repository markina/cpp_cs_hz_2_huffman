#include <fstream>
#include <stdexcept>
#include "huffman.hpp"
#include <iostream>
#include <cstring>
#include <algorithm>


Node::Node(int cnt, char ch)  : cnt(cnt), ch(ch) { left_child = NULL; right_child = NULL; }
Node::Node() { ch = '0'; cnt = 0; left_child = NULL; right_child = NULL; }
Node::~Node() {}
Node::Node(Node *pNode)
{
    cnt = pNode->cnt;
    left_child = pNode->left_child;
    right_child = pNode->right_child;
    ch = pNode->ch;
}
bool Node::compare(Node a, Node b) { return a.cnt < b.cnt; }

Huffman::Huffman() {  }
Huffman::~Huffman() {    /*delete [] buffer;*/ }

void Compression::compression() {
    read_writer.read_in_string();
    leaves = get_leaves();

    get_tree();

    get_code_by_char();

    put_code_by_char();
    put_massage();
    read_writer.send();
}


void Decompression::decompression() {
    read_writer.read_in_string();

    int first_index_code = get_code_by_char();

    get_tree();

    int k = 0;
//
//    read_writer.get_out_buffer();
//    read_writer.write_out_buffer();

}

void ReadWriter::read_in_string() {
    char *in_buffer = nullptr;
    int length_in_buffer = 0;
    std::ifstream in("/home/rita/studies/cppcs/cpp_cs_hz_2_huffman/res/new.txt", std::ifstream::binary);
    if(in)
    {
        in.seekg(0, in.end);
        length_in_buffer = (int) in.tellg();
        in.seekg(0, in.beg);

        in_buffer = new char[length_in_buffer];

        //std::cout << "Reading " << length_buffer << " characters... ";

        in.read(in_buffer, length_in_buffer);

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
    for(int i = 0; i < length_in_buffer; i++)
    {
        in_string.push_back(in_buffer[i]);
    }
}

void print_vector_cnt(std::vector<Node> leaves)
{
    for(int i = 0; i < leaves.size(); i++) {
        std::cout << leaves[i].ch << "!!!" << leaves[i].cnt << std::endl;
    }
}


std::vector<Node> Compression::get_leaves()
{
    int cnt[ReadWriter::MAX_NUM_BY_CHAR];
    std::vector<Node> leaves;

    for(int i = 0; i < ReadWriter::MAX_NUM_BY_CHAR; ++i) {
        cnt[i] = 0;
    }

    for(int i = 0; i < read_writer.in_string.size(); ++i) {
        int num_by_char = read_writer.in_string[i];
        cnt[num_by_char]++;
    }

    for(int i = 0; i < ReadWriter::MAX_NUM_BY_CHAR; i++) {
        if(cnt[i] != 0) {
            Node node(cnt[i], (char) i);
            leaves.push_back(node);
        }
    }
    std::sort(leaves.begin(), leaves.end(), Node::compare);
    print_vector_cnt(leaves);
    return leaves;
}




void Compression::get_tree()
{
    if(leaves.size() == 0) {
        return;
    }

    size_t iterrator = 0;
    while(iterrator < leaves.size() - 1) {
        Node first = new Node (leaves[iterrator]);
        Node second = new Node (leaves[iterrator + 1]);
        Node *newNode = new Node(first.cnt + second.cnt, first.ch);
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
        //sort(leaves.begin() + iterrator, leaves.end(), Node::compare); //todo good sort
    }

    root = new Node(leaves[iterrator]);
}

void print_code_by_char(std::string code_by_char[])
{
    std::cout << "\n";
    for(int i = 0; i < 256; i++) {
        if(code_by_char[i].length() != 0) {
            std::cout << (char)(i) << "->" << code_by_char[i] << std::endl;
        }
    }
}

void Compression::get_code_by_char() {
    for(int i = 0;  i < ReadWriter::MAX_NUM_BY_CHAR; i++) {
        code_by_char[i] = "";
    }

    rec_code_by_char(root, "");

    print_code_by_char(code_by_char);
}

void Compression::rec_code_by_char(Node node, std::string cur_string)
{
    if(node.left_child == NULL) {
        char c = node.ch;
        code_by_char[(int)c] = cur_string;
        return;
    }

    rec_code_by_char(node.left_child, cur_string + "0");
    rec_code_by_char(node.right_child, cur_string + "1");
}


void Compression::get_string_by_number(int n ){
    std::vector<char> t;

    while(n > 0) {
        t.push_back(char((int)'0' + n%10));
        n /= 10;
    }


    for(int i = t.size() - 1; i>= 0; i--) {
        read_writer.out_string.push_back(t[i]);
    }
}


void print_out_string(std::vector<char> vector)
{
    std::cout << std::endl;
    for(int i = 0; i < vector.size(); i++) {
        std::cout << vector[i];
    }
    std::cout << std::endl;
}

//
//Node Huffman::get_tree_from_file()
//{
//    //todo
//    return root;
//}
//
//char Huffman::get_letter(int position, Node node, std::string code)
//{
//    if(node.left_child == NULL) {
//        return node.string[0];
//    }
//    if(code[position] == '1') {
//        return get_letter(position+1, node.right_child, code);
//    }
//    if(code[position] == '0') {
//        return get_letter(position+1, node.left_child, code);
//    }
//
//    return '?';
//
//}
//
//
//
//
//int Huffman::get_message()
//{
//    int size_massege = 0;
//    for(int i = 0; i < length_buffer; i++)
//    {
//        std::string s = code_by_char[(buffer[i])];
//        size_massege += s.length();
//        for (int j =0; j < s.length(); j++)
//        {
//            vector_for_buffer.push_back(s[j]);
//        }
//    }
//}
//
//int Huffman::extract_vector_cnt_by_char()
//{
//    //todo
//    return 0;
//}
//
//void Compression::get_out_buffer()
//{
//    send_code_by_char();
//    int last_i_list_code = vector_for_buffer.size();
//    int size_message = get_message();
//    //result_buffer[vector_for_buffer.size() + ];
//    for(int i = 0; i < last_i_list_code; i++) {
//
//    }
//
//}
//
//Node Coder::extract_tree(ReadWriter writer)
//{
//    int begin_code=  extract_vector_cnt_by_char();
//
//    //char * code = buffer + begin_code;
//    std::string code = "011";
//
//    std::string result = "";
//
//    for (size_t i = 0; i < code.length(); ) {
//        char c = get_letter(i, root, code);
//        result += c;
//        i += (int)code_by_char[c].length();
//    }
//
//    std::cout << result << "!!!!!!!!!!!!!!!!!!!\n";
//}
//
//void ReadWriter::write_out_buffer()
//{
//
//}

Decompression::Decompression(char *in, char *out) : Huffman(in, out) {}
Compression::Compression(char *in, char *out) : Huffman(in, out) {}
Huffman::Huffman(char *in, char *out) : read_writer(in, out) {}
ReadWriter::ReadWriter(char *in, char *out) :in_file(in), out_file(out) {}

void Compression::get_out_buffer()
{

}

void Decompression::get_tree()
{
    for(int i = 0; i < ReadWriter::MAX_NUM_BY_CHAR; i++) {
        if(code_by_char[i] != "")
        {
            add_new_leave(i, 0, code_by_char[i], root);
        }
    }
}

int Decompression::get_code_by_char()
{
    int n = 0;
    int i = 0;
    for(; i < read_writer.in_string.size(); ++i) {
        if(read_writer.in_string[i] == ' ') {
            i++;
            break;
        }
        n *= 10;
        n += read_writer.in_string[i] - '0';
    }

    std::cout << "n = " << n << "\n"; //

    for(int j = 0; j < 256; ++j) {
        code_by_char[j] = "";
    }

    for(int x = 0; x < n; ++x) {
        int id_char = (int)read_writer.in_string[i];
        i++;
        for(; i < read_writer.in_string.size(); ++i) {
            if(read_writer.in_string[i] == ' ') {
                i++;
                break;
            }
            code_by_char[id_char].push_back(read_writer.in_string[i]);
        }
    }

    print_code_by_char(code_by_char);
}
void Huffman::print_usage() {std::cout << USAGE << std::endl; }

ReadWriter::ReadWriter()
{

}

void Compression::put_code_by_char()
{

    int n = leaves.size();
    get_string_by_number(n);
    read_writer.out_string.push_back(' ');
    for(size_t i = 0; i < ReadWriter::MAX_NUM_BY_CHAR; i++) {
        if(code_by_char[i].length() != 0) {
            read_writer.out_string.push_back(char(i));
            for(int j = 0; j < code_by_char[i].length(); j++) {
                if(code_by_char[i].length() != 0)
                {
                    if (code_by_char[i][j] == '1')
                    {
                        read_writer.out_string.push_back('1');
                    } else
                    {
                        read_writer.out_string.push_back('0');
                    }
                }
            }
            read_writer.out_string.push_back(' ');
        }
    }

    print_out_string(read_writer.out_string);

}

void Compression::put_massage()
{
    int size_massage = get_size_massege();
    int excess_zero = 8 - size_massage%8;
    std::vector<int> one_and_zero;
    for(int i =0; i < excess_zero; i++) {
        one_and_zero.push_back(0);
    }
    for(int i = 0; i < read_writer.in_string.size(); ++i) {
        for(int j = 0; j < code_by_char[read_writer.in_string[i]].length(); j++) {
            if(code_by_char[read_writer.in_string[i]][j] == '0') {
                one_and_zero.push_back(0);
            } else {
                one_and_zero.push_back(1);
            }
        }
    }

    for(int i = 0; i < one_and_zero.size(); ) {
        int id_char = 0;
        for(int j = 1; j < 256; j *= 2) {
            id_char += one_and_zero[i] * j;
            i++;
        }
        read_writer.out_string.push_back((char)id_char);
    }


    print_out_string(read_writer.out_string);
}

void ReadWriter::send()
{
    std::ofstream outfile ("/home/rita/studies/cppcs/cpp_cs_hz_2_huffman/res/in.txt", std::ofstream::binary);

    unsigned long size =  out_string.size();
    char buffer[size];

    for(int j = 0; j < size; ++j) {
        buffer[j] = out_string[j];
    }

    outfile.write(buffer,size);

    //delete[] buffer;

    outfile.close();
}


int Compression::get_size_massege()
{
    int s = 0;
    for(int i = 0; i < ReadWriter::MAX_NUM_BY_CHAR; i++) {
        s += leaves[i].cnt * code_by_char[(int)leaves[i].ch].size();
    }
    return s;
}

void Decompression::add_new_leave(int id_char, int l, std::string string, Node & node)
{
    for (int i = l; i < string.size(); i++)
    {
        if (string[i] == '0')
        {
            if (node.left_child != NULL)
            { //todo left_child
                if (i == string.size() - 1)
                {
                    node.left_child = new Node(0, id_char);
                    return;
                }
            }
            else
            {
                node.left_child = new Node(0, '?');
                add_new_leave(id_char, l + 1, string, (Node &) node.left_child);
            }


        }
        if (string[i] == '1')
        {
            if (node.right_child != NULL)
            {
                if (i == string.size() - 1)
                {
                    node.right_child = new Node(0, id_char);
                    return;
                }
            }
            else
            {
                node.right_child = new Node(0, '?');
                add_new_leave(id_char, l + 1, string, (Node &) node.right_child);
            }
        }

    }
}

