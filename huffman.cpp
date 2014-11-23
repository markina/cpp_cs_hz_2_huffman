#include <fstream>
#include <stdexcept>
#include "huffman.hpp"
#include <iostream>
#include <cstring>
#include <algorithm>


Node::Node(int cnt, char ch)  : cnt(cnt), ch(ch) { left_child = nullptr; right_child = nullptr; }
Node::Node() { ch = '0'; cnt = 0; left_child = nullptr; right_child = nullptr; }
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
    get_leaves();



    get_tree(leaves);

    get_code_by_char();

    put_code_by_char();

    put_massage();

    read_writer.send();
}


void Decompression::decompression() {
    read_writer.read_in_string();

    int first_index_code = get_code_by_char();

    get_tree();

    put_decode_massage(first_index_code);

    read_writer.send();
}

void ReadWriter::read_in_string() {
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
    }
    for(int i = 0; i < length_in_buffer; i++)
    {
        in_string.push_back(in_buffer[i]);
    }
}

void print_vector_cnt(std::vector<Node> leaves)
{
    for(int i = 0; i < leaves.size(); i++) {
        std::cout << "Count: " << leaves[i].ch << " = " << leaves[i].cnt << std::endl;
    }
}


int Compression::get_size_msg()
{
    int s = 0;
    for(int i = 0; i < leaves.size(); i++) {
        int num_char = (int)leaves[i].ch > 0 ? (int)leaves[i].ch : (-(int)leaves[i].ch) + 128;
        s += leaves[i].cnt * code_by_char[num_char].length();
    }
    return s;
}

void Compression::get_leaves()
{
    int cnt[ReadWriter::MAX_NUM_BY_CHAR];

    for(int i = 0; i <= ReadWriter::MAX_NUM_BY_CHAR; ++i) {
        cnt[i] = 0;
    }

    for(int i = 0; i < read_writer.in_string.size(); ++i) {
        int num_by_char = read_writer.in_string[i];
        cnt[num_by_char]++;
    }

    for(int i = 0; i <= ReadWriter::MAX_NUM_BY_CHAR; i++) {
        if(cnt[i] != 0) {
            Node node(cnt[i], (char) i);
            leaves.push_back(node);
        }
    }
    std::sort(leaves.begin(), leaves.end(), Node::compare);
    print_vector_cnt(leaves);
}




void Compression::get_tree(std::vector<Node> leaves)
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

    }

    root = new Node(leaves[iterrator]);
}

void print_code_by_char(std::string code_by_char[])
{
    std::cout << "\n";
    for(int i = 0; i <= ReadWriter::MAX_NUM_BY_CHAR; i++) {
        if(code_by_char[i].length() != 0) {
            std::cout << "Code: " << (char)(i) << "->" << code_by_char[i] << std::endl;
        }
    }
}

void Compression::get_code_by_char() {
    for(int i = 0;  i <= ReadWriter::MAX_NUM_BY_CHAR; i++) {
        code_by_char[i] = "";
    }

    rec_code_by_char(root, "");

    print_code_by_char(code_by_char);
}

void Compression::rec_code_by_char(Node node, std::string cur_string)
{
    if(node.left_child == nullptr && node.right_child == nullptr) {
        char c = node.ch;
        code_by_char[c > 0 ? c : (-c) + 128]  = cur_string;
        return;
    }

    rec_code_by_char(node.left_child, cur_string + "0");
    rec_code_by_char(node.right_child, cur_string + "1");
}


void Compression::put_string_by_number(int n){
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

char Decompression::get_letter(int position, std::vector<bool> & in_byte, Node * node)
{
    if(node->left_child == nullptr && node->right_child == nullptr) {
        return node->ch;
    }
    if(in_byte[position]) {
        return get_letter(position+1, in_byte, node->right_child);
    }
    if(!in_byte[position]) {
        return get_letter(position+1, in_byte, node->left_child);
    }

    return '?';

}

Decompression::Decompression(char *in, char *out) : Huffman(in, out) {}
Compression::Compression(char *in, char *out) : Huffman(in, out) {}
Huffman::Huffman(char *in, char *out) : read_writer(in, out) {}
ReadWriter::ReadWriter(char *in, char *out) :in_file(in), out_file(out) {}


void Decompression::get_tree()
{
    root = new Node();
    for(int i = 0; i <= ReadWriter::MAX_NUM_BY_CHAR; i++) {
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

    for(int j = 0; j <= ReadWriter::MAX_NUM_BY_CHAR; ++j) {
        code_by_char[j] = "";
    }

    for(int x = 0; x < n; ++x) {
        int num_char = read_writer.in_string[i] > 0 ? read_writer.in_string[i] : (-read_writer.in_string[i]) + 128;;
        i++;
        for(; i < read_writer.in_string.size(); ++i) {
            if(read_writer.in_string[i] == ' ') {
                i++;
                break;
            }
            code_by_char[num_char].push_back(read_writer.in_string[i]);
        }
    }

    print_code_by_char(code_by_char);
    return i;
}
void Huffman::print_usage() {std::cout << USAGE << std::endl; }

ReadWriter::ReadWriter()
{

}

void Compression::put_code_by_char()
{

    int n = leaves.size();
    put_string_by_number(n);
    read_writer.out_string.push_back(' ');
    for(size_t i = 0; i <= ReadWriter::MAX_NUM_BY_CHAR; i++) {
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
    int size_massage = get_size_msg();
    int excess_zero = 8 - (size_massage % 8);

    read_writer.out_string.push_back('0' + excess_zero);

    std::vector<int> one_and_zero;
    for(int i =0; i < excess_zero; i++) {
        one_and_zero.push_back(0);
    }
    for(int i = 0; i < read_writer.in_string.size(); ++i) {
        int num_char = read_writer.in_string[i] > 0 ? read_writer.in_string[i] : (-read_writer.in_string[i]) + 128;
        for(int j = 0; j < code_by_char[num_char].length(); j++) {
            if(code_by_char[num_char][j] == '0') {
                one_and_zero.push_back(0);
            } else {
                one_and_zero.push_back(1);
            }
        }
    }

    for(int i = 0; i < one_and_zero.size(); ) {
        int id_char = 0;
        for(int j = 128; j > 0; j /= 2) {
            if(j == 128) {
                id_char += one_and_zero[i] * (-j);
            }
            else
            {
                id_char += one_and_zero[i] * j;
            }
            i++;
        }
        read_writer.out_string.push_back((char)id_char);
    }


    print_out_string(read_writer.out_string);
}

void ReadWriter::send()
{
    std::ofstream outfile (out_file, std::ofstream::binary);

    unsigned long size =  out_string.size();
    char buffer[size];

    for(int j = 0; j < size; ++j) {
        buffer[j] = out_string[j];
    }

    outfile.write(buffer,size);

    //delete[] buffer;

    outfile.close();
}

void Decompression::add_new_leave(int id_char, int l, std::string string, Node * node)
{
    for (int i = l; i < string.length(); i++)
    {
        if (string[i] == '0')
        {
            if (node->left_child == nullptr)
            {
                if (i == string.length() - 1)
                {
                    node->left_child = new Node(0, id_char);
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
                    node->right_child = new Node(0, id_char);
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

void print_decode_massage(std::vector<char> vector)
{
    std::cout << "result::::::";
    for(int i = 0; i < vector.size(); i++) {
        std::cout << vector[i];
    }

}

void Decompression::put_decode_massage(int begin)
{
    char c = read_writer.in_string[begin];
    int bad_zero = c - '0';

    std::vector<bool> in_byte;
    for(int i = begin + 1; i < read_writer.in_string.size(); i++) {
        char c = read_writer.in_string[i];
        get_vector_byte_by_char(in_byte, bad_zero, c);
        bad_zero = 0;
    }

    int i = 0;
    while(i < in_byte.size())
    {
        char t = get_letter(i, in_byte, root);
        read_writer.out_string.push_back(t);
        i += code_by_char[t > 0 ? t : (-t) + 128].length();
    }

    print_decode_massage(read_writer.out_string);

}


void Decompression::get_vector_byte_by_char(std::vector<bool> &in_byte, int left_byte, char c)
{
    int num = c;
    for(int i = 128; i > 0; i /= 2) {
        if(left_byte > 0) {
            left_byte--;
            num %= i;

        }
        else
        {
            if(i == 128) {
                in_byte.push_back(num < 0);
                num += 128;
            }
            else
            {
                in_byte.push_back((bool) (num / i));
                num %= i;
            }
        }
    }


}
