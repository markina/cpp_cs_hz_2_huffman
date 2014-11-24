#include <fstream>
#include <stdexcept>
#include "huffman.hpp"
#include <iostream>
#include <cstring>
#include <algorithm>

//void print_vector_cnt(std::vector<Node> leaves)
//{
//    for(int i = 0; i < leaves.size(); i++) {
//        std::cout << "Count: " << leaves[i].ch << " = " << leaves[i].cnt << std::endl;
//    }
//}
//
//void print_byte(std::vector<bool> byte) {
//    std::cout << "bute: ";
//    for(int i = 0; i < byte.size(); i++) {
//        std::cout << byte[i];
//    }
//    std::cout << std::endl;
//}
//
//void Huffman::print_code_by_char(std::string code_by_char[])
//{
//    std::cout << "\n";
//    for(int i = 0; i < 256; i++) {
//        if(code_by_char[i].length() != 0) {
//            std::cout << "Code: " << cast_int_to_char(i) << "->" << code_by_char[i] << std::endl;
//        }
//    }
//}
//
//void print_out_string(std::vector<char> out_string)
//{
//    std::cout << std::endl;
//    for(int i = 0; i < out_string.size(); i++) {
//        std::cout << out_string[i];
//    }
//    std::cout << std::endl;
//}
//
//void print_in_string(std::vector<char> in_string)
//{
//    std::cout << std::endl;
//    for(int i = 0; i < in_string.size(); i++) {
//    std::cout << in_string[i];
//    }
//    std::cout << std::endl;
//}
//
//void print_decode_massage(std::vector<char> vector)
//{
//    std::cout << "result::::::";
//    for(int i = 0; i < vector.size(); i++) {
//        std::cout << vector[i];
//    }
//
//}


int Compression::compression() {
    int ret = reader.read_in_string();
    if(ret != 0) {
        root = new Node();
        return 1;
    }

    std::cout << reader.in_string.size() << std::endl;

    get_leaves();

    get_tree(leaves);

    get_code_by_char();

    put_code_by_char();

    size_t size_addition = writer.out_string.size();

    put_massage();

    std::cout << writer.out_string.size() << std::endl;

    std::cout << size_addition << std::endl;

    writer.send();

    return 0;

}

void Compression::get_leaves()
{
    int cnt[MAX_NUM_BY_CHAR];

    for(size_t i = 0; i < MAX_NUM_BY_CHAR; ++i) {
        cnt[i] = 0;
    }

    for(size_t i = 0; i < reader.in_string.size(); ++i) {
        int num_by_char = cast_char_to_int(reader.in_string[i]);
        cnt[num_by_char]++;
    }

    for(size_t i = 0; i < MAX_NUM_BY_CHAR; i++) {
        if(cnt[i] != 0) {
            Node node(cnt[i], cast_int_to_char(i));
            leaves.push_back(node);
        }
    }
    std::sort(leaves.begin(), leaves.end(), Node::compare);
    //print_vector_cnt(leaves);
}


void Compression::get_tree(std::vector<Node> leaves)
{
    if(leaves.size() == 0) {
        root = new Node();
        return;
    }

    if(leaves.size() == 1) {
        root = new Node();
        root->left_child = new Node(leaves[0]);
        return;
    }

    size_t i_leaves = 0;
    while(i_leaves < leaves.size() - 1) {
        Node * first = new Node (leaves[i_leaves]);
        Node * second = new Node (leaves[i_leaves + 1]);
        Node *nNode = new Node(first->cnt + second->cnt, first->ch);
        nNode->left_child = new Node(first);
        nNode->right_child = new Node(second);
        leaves[i_leaves + 1] = Node (nNode);
        i_leaves++;
        delete first;
        delete second;
        //delete nNode->left_child;
        //delete nNode->right_child;
        delete nNode;
        for(size_t j = i_leaves; j < leaves.size() - 1; j++) {
            if(leaves[j].cnt > leaves[j+1].cnt) {
                std::swap(leaves[j], leaves[j+1]);
            } else {
                break;
            }
        }

    }
    root = new Node(leaves[i_leaves]);
}



void Compression::get_code_by_char() {
    clear_code_by_char();


    rec_code_by_char(root, "");

    //print_code_by_char(code_by_char);
}

void Compression::rec_code_by_char(Node node, std::string cur_string)
{
    if(node.left_child == nullptr && node.right_child == nullptr) {
        code_by_char[cast_char_to_int(node.ch)] = cur_string;
        return;
    }

    if(node.left_child != nullptr)
    {
        rec_code_by_char(node.left_child, cur_string + "0");
    }
    if(node.right_child != nullptr)
    {
        rec_code_by_char(node.right_child, cur_string + "1");
    }
}


int Compression::get_size_msg()
{
    int s = 0;
    for(size_t i = 0; i < leaves.size(); i++) {
        s += leaves[i].cnt * code_by_char[cast_char_to_int((int)leaves[i].ch)].length();
    }
    return s;
}

void Compression::put_string_by_number(int n){
    std::vector<char> t;
    while(n > 0) {
        t.push_back(char((int)'0' + n % 10));
        n /= 10;
    }

    for(int i = t.size() - 1; i>= 0; i--) {
        writer.out_string.push_back(t[i]);
    }
}



void Compression::put_code_by_char()
{
    int n = leaves.size();
    put_string_by_number(n);
    writer.out_string.push_back(' ');

    for(size_t i = 0; i < MAX_NUM_BY_CHAR; i++) {
        if(code_by_char[i].length() != 0) {
            writer.out_string.push_back(cast_int_to_char(i));
            for(size_t j = 0; j < code_by_char[i].length(); j++) {
                if (code_by_char[i][j] == '1')
                {
                    writer.out_string.push_back('1');
                } else
                {
                    writer.out_string.push_back('0');
                }
            }
            writer.out_string.push_back(' ');
        }
    }
    //print_out_string(writer.out_string);
}

void Compression::put_massage()
{
    int size_massage = get_size_msg();
    int excess_zero = 8 - (size_massage % 8);
    writer.out_string.push_back('0' + excess_zero);

    std::vector<bool> out_byte;
    for(int i =0; i < excess_zero; i++) {
        out_byte.push_back(0);
    }

    for(size_t i = 0; i < reader.in_string.size(); ++i) {
        for(size_t j = 0; j < code_by_char[cast_char_to_int(reader.in_string[i])].length(); j++) {
            if(code_by_char[cast_char_to_int(reader.in_string[i])][j] == '0') {
                out_byte.push_back(0);
            } else {
                out_byte.push_back(1);
            }
        }
    }

    for(size_t i = 0; i < out_byte.size(); i += 8) {
        writer.out_string.push_back(get_char_from_8_byte(i, out_byte));
    }

    //print_byte(out_byte);

    //print_out_string(writer.out_string);
}


size_t Decompression::get_letter(size_t position, std::vector<bool> & in_byte, Node * node)
{
    if(node->left_child == nullptr && node->right_child == nullptr) {
        writer.out_string.push_back(node->ch);
        return position;
    }
    if(in_byte[position]) {
        return get_letter(position + 1, in_byte, node->right_child);
    }
    if(!in_byte[position]) {
        return get_letter(position + 1, in_byte, node->left_child);
    }

    return 0;
}


int Decompression::decompression() {
    int ret = reader.read_in_string();
    if(ret != 0) {
        root = new Node();
        return 1;
    }

    std::cout << reader.in_string.size() << std::endl;

    size_t first_index_massage = get_code_by_char();

    get_tree();

    put_decode_massage(first_index_massage);

    std::cout << writer.out_string.size() << std::endl;

    std::cout << first_index_massage << std::endl;

    writer.send();
    return 0;
}

void Decompression::get_tree()
{
    root = new Node();
    for(size_t i = 0; i < MAX_NUM_BY_CHAR; i++) {
        if(code_by_char[i] != "")
        {
            add_new_leave(i, 0, code_by_char[i], root);
        }
    }
}

int Decompression::get_code_by_char()
{
    //print_in_string(reader.in_string);

    int n = 0;
    size_t i = 0;
    for(; i < reader.in_string.size(); ++i) {
        if(reader.in_string[i] == ' ') {
            i++;
            break;
        }
        n *= 10;
        n += reader.in_string[i] - '0';
    }

    //std::cout << "n = " << n << "\n"; //

    clear_code_by_char();

    for(int x = 0; x < n; ++x) {
        int id_char = cast_char_to_int(reader.in_string[i]);
        i++;
        for(; i < reader.in_string.size(); ++i) {
            if(reader.in_string[i] == ' ') {
                i++;
                break;
            }
            code_by_char[id_char].push_back(reader.in_string[i]);
        }
    }

    //print_code_by_char(code_by_char);
    return i;
}




void Decompression::add_new_leave(int id_char, int l, std::string string, Node * node)
{
    for (size_t i = l; i < string.length(); i++)
    {
        if (string[i] == '0')
        {
            if (node->left_child == nullptr)
            {
                if (i == string.length() - 1)
                {
                    node->left_child = new Node(0, cast_int_to_char(id_char));
                    return;
                }
                else
                {
                    node->left_child = new Node(0, '?');
                    add_new_leave(id_char, l + 1, string, node->left_child);
                    return;
                }
            }
            else {
                add_new_leave(id_char, l + 1, string, node->left_child);
                return;
            }


        }
        if (string[i] == '1')
        {
            if (node->right_child == nullptr)
            {
                if (i == string.length() - 1)
                {
                    node->right_child = new Node(0, cast_int_to_char(id_char));
                    return;
                }
                else
                {
                    node->right_child = new Node(0, '?');
                    add_new_leave(id_char, l + 1, string, node->right_child);
                    return;
                }
            }
            else {

                add_new_leave(id_char, l + 1, string, node->right_child);
                return;
            }
        }

    }
}


void Decompression::put_decode_massage(size_t begin)
{
    char c = reader.in_string[begin];
    int bad_zero = c - '0';

    std::vector<bool> in_byte;

    char ch = reader.in_string[begin + 1];
    get_vector_byte_by_char(in_byte, bad_zero, ch);


    ///////////////stop
    for(size_t i = begin + 2; i < reader.in_string.size(); i++) {
        char c = reader.in_string[i];
        get_vector_byte_by_char(in_byte, c);
    }

    //print_byte(in_byte);

    size_t i = 0;
    while(i < in_byte.size())
    {
        i = get_letter(i, in_byte, root);

        //i += code_by_char[t].length();
    }



    //print_decode_massage(writer.out_string);

}

void Decompression::get_vector_byte_by_char(std::vector<bool> &in_byte, char c) {

    int num = cast_char_to_real_int(c);
    for(int i = 128; i > 0; i /= 2) {
        if(i == 128) {
            in_byte.push_back(num < 0);
            if(num < 0)
            {
                num += 128;
            }
        }
        else
        {
            in_byte.push_back((num / i) == 1);
            num %= i;
        }
    }
}

void Decompression::get_vector_byte_by_char(std::vector<bool> & in_byte, int left_byte, char c)
{
    int num = cast_char_to_real_int(c);

    for(int i = 128; i > 0; i /= 2) {
        if(left_byte > 0) {
            if(i == 128) {
                if(num < 0)
                {
                    num += 128;
                }
            }
            else {
                num %= i;
            }
            left_byte--;
        }
        else
        {
            if(i == 128) {
                in_byte.push_back(num < 0);
                if(num < 0)
                {
                    num += 128;
                }
            }
            else
            {
                in_byte.push_back((num / i) == 1);
                num %= i;
            }
        }
    }
}

Decompression::Decompression(char *in, char *out) : Huffman(in, out) {}
Compression::Compression(char *in, char *out) : Huffman(in, out) {}
Huffman::Huffman(char *in, char *out) : reader(in), writer(out) {}
Huffman::Huffman() {  }
Huffman::~Huffman() {
    delete_tree();

}
Writer::Writer() {}
Reader::Reader() {}
Reader::Reader(char *in) : in_file(in) {}
Writer::Writer(char *out) : out_file(out) {}
void Huffman::print_usage() {std::cout << USAGE << std::endl; }
Node::Node(int cnt, char ch)  : cnt(cnt), ch(ch) { left_child = nullptr; right_child = nullptr; }
Node::Node() : Node(0, '0') {}
Node::~Node() {}
Node::Node(Node *pNode)
{
    cnt = pNode->cnt;
    left_child = pNode->left_child;
    right_child = pNode->right_child;
    ch = pNode->ch;
}
bool Node::compare(Node a, Node b) { return a.cnt < b.cnt; }

int Reader::read_in_string() {
    char *in_buffer = nullptr;
    int length_in_buffer = 0;
    std::ifstream in(in_file, std::ifstream::binary);
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
        for (int i = 0; i < length_in_buffer; i++)
        {
            in_string.push_back(in_buffer[i]);
        }

        delete [] in_buffer;
        return 0;
    }
    std::cout << "No such file" << std::endl;
    return 1;
}

void Writer::send()
{
    std::ofstream outfile (out_file, std::ofstream::binary);

    size_t size =  out_string.size();
    char buffer[size];

    for(size_t j = 0; j < size; ++j) {
        buffer[j] = out_string[j];
    }

    outfile.write(buffer,size);

    //delete[] buffer;

    outfile.close();
}


int Huffman::cast_char_to_int(char ch)
{
    return ((int)ch) + OFFSET;
}

int Huffman::cast_char_to_real_int(char ch)
{
    return ((int)ch);
}


char Huffman::cast_int_to_char(int n)
{
    return (char)(n - OFFSET);
}

char Huffman::cast_real_int_to_char(int n)
{
    return (char)(n);
}

char Compression::get_char_from_8_byte(int begin, std::vector<bool> & byte)
{
    int id_char = 0;
    int end = begin + 8;
    for(int j = 128; j > 0 && begin < end; begin++, j /= 2) {
        if(j == 128) {
            id_char += byte[begin] * (-j);
        }
        else
        {
            id_char += byte[begin] * j;
        }
    }
    return cast_real_int_to_char(id_char);
}

void Huffman::clear_code_by_char()
{

    for(size_t j = 0; j < MAX_NUM_BY_CHAR; ++j) {
        code_by_char[j] = "";
    }

}

void Huffman::delete_tree()
{
    if(root)
    {
        rec_delete_tree(root);
    }
}

void Huffman::rec_delete_tree(Node *node)
{
    if(node->left_child == nullptr && node->right_child == nullptr) {
        delete node;
        return;
    }

    if(node->left_child != nullptr)
    {
        rec_delete_tree(node->left_child);
    }
    if(node->right_child != nullptr)
    {
        rec_delete_tree(node->right_child);
    }

    delete node;
}
