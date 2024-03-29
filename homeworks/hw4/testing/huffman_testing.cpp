#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <vector>
#include <utility>
#include <string>
#include <fstream>
#include "gtest/gtest.h"
#include "../lib/huffman_lib.h"

const size_t BLOCK_SIZE = 65536;

bool is_file_empty(std::ifstream &pFile) {
    return pFile.peek() == std::ifstream::traits_type::eof();
}

void encode(std::string const &file_in, std::string const &file_out) {
    std::ifstream fin(file_in, std::ios::in | std::ios::binary);
    std::ofstream fout(file_out, std::ios::out | std::ios::binary);
    if (!(fin && fout)) {
        throw std::runtime_error("Error while opening input/output files");
    }
    if (is_file_empty(fin)) {
        fin.close();
        fout.close();
        return;
    }
    // Encoding mode
    symbol_counter counter;
    auto *block = (uint8_t *) malloc(BLOCK_SIZE * sizeof(uint8_t));
    while (fin) {
        memset(block, 0, BLOCK_SIZE);
        fin.read((char *) block, BLOCK_SIZE);
        counter.add_frequency(static_cast<size_t>(fin.gcount()), block);
    }
    fin.clear();
    fin.seekg(0);
    power_bitset<uint64_t> bits;
    std::vector<uint8_t> symbols;
    huffman_enc encoder(counter);
    encoder.encode_tree_data(bits, symbols);
    auto alph_size = static_cast<uint16_t>(symbols.size());
    // Started encoding (Huffman Tree goes first)
    // First byte - size of alphabet
    fout.write((char *) &alph_size, sizeof(uint16_t));
    // Second part - sequence of power_bitset<T>, where T = uint64_t, contains tree structure info.
    auto structure_size = static_cast<uint32_t>(bits.size());
    // This part is split : size first
    fout.write((char *) &structure_size, sizeof(uint32_t));
    // structure second
    size_t remain = bits.size() % (sizeof(uint64_t) * 8);
    if (remain == 0) {
        fout.write((char *) bits.bs_data().data(), bits.size() / 8);
    } else {
        fout.write((char *) bits.bs_data().data(),
                   (bits.size() / (sizeof(uint64_t) * 8) + 1) * sizeof(uint64_t));
    }
    // Third part - the alphabet (in bytes)
    fout.write((char *) symbols.data(), symbols.size());
    // and the encoded text
    while (fin) {
        fin.read((char *) block, BLOCK_SIZE);
        bits = encoder.encode_block((size_t) fin.gcount(), block);
        structure_size = static_cast<uint32_t>(bits.size());
        fout.write((char *) &structure_size, sizeof(uint32_t));
        remain = bits.size() % (sizeof(uint64_t) * 8);
        if (remain == 0) {
            fout.write((char *) bits.bs_data().data(), bits.size() / 8);
        } else {
            fout.write((char *) bits.bs_data().data(),
                       (bits.size() / (sizeof(uint64_t) * 8) + 1) * sizeof(uint64_t));
        }
    }
    free(block);
}

void decode(std::string const &file_in, std::string const &file_out) {
    std::ifstream fin(file_in, std::ios::in | std::ios::binary);
    std::ofstream fout(file_out, std::ios::out | std::ios::binary);
    if (is_file_empty(fin)) {
        return;
    }
    uint16_t alph_size;
    uint32_t tree_size;
    if (fin.read((char *) &alph_size, sizeof(uint16_t)).gcount() == 0 ||
        fin.read((char *) &tree_size, sizeof(uint32_t)).gcount() == 0) {
        throw std::runtime_error("File is damaged - probably missing info for decoding.");
    }
    uint32_t data_size = (tree_size / (sizeof(uint64_t) * 8) + ((tree_size % (sizeof(uint64_t) * 8)) != 0)) *
                         sizeof(uint64_t) + alph_size;
    std::vector<uint8_t> tree_data(data_size);
    if (alph_size > 256) {
        throw std::runtime_error("Incorrect alphabet data - file is damaged.");
    }
    if (fin.read((char *) tree_data.data(), data_size).gcount() != data_size) {
        throw std::runtime_error("File is damaged - probably missing info for decoding.");
    }
    huffman_dec decoder(tree_data, tree_size, alph_size);
    // Decoding
    while (fin) {
        uint32_t read_size;
        fin.read((char *) &read_size, sizeof(uint32_t));
        if (fin.gcount() == 0) {
            break;
        }
        uint32_t newsz = (read_size / (sizeof(uint64_t) * 8) + ((read_size % (sizeof(uint64_t) * 8)) != 0));
        std::vector<uint64_t> text_block(newsz);
        fin.read((char *) text_block.data(), text_block.size() * sizeof(uint64_t));
        if (fin.gcount() != newsz * sizeof(uint64_t)) {
            throw std::runtime_error("File is damaged - probably missing info for decoding.");
        }
        power_bitset<uint64_t> bs(text_block);
        size_t type_sz = (8 * sizeof(uint64_t));
        if (read_size % type_sz > 0) {
            bs.drop_last(type_sz - (read_size % type_sz));
        }
        std::vector<uint8_t> decoded = decoder.decode_block(bs);
        fout.write((char *) decoded.data(), decoded.size());
    }
}


TEST(correctness, empty) {
    std::string test_file = "tests/empty";
    encode(test_file + ".in", test_file + ".enc");
    decode(test_file + ".enc", test_file + ".res");
    std::ifstream first(test_file + ".in");
    std::stringstream first_file, second_file;
    first_file << first.rdbuf();
    std::ifstream second(test_file + ".res");
    second_file << second.rdbuf();
    EXPECT_EQ(first_file.str(), second_file.str());
}

TEST(correctness, one_letter) {
    std::string test_file = "tests/one_letter";
    encode(test_file + ".in", test_file + ".enc");
    decode(test_file + ".enc", test_file + ".res");
    std::ifstream first(test_file + ".in");
    std::stringstream first_file, second_file;
    first_file << first.rdbuf();
    std::ifstream second(test_file + ".res");
    second_file << second.rdbuf();
    EXPECT_EQ(first_file.str(), second_file.str());
}

TEST(correctness, random_ascii) {
    std::string test_file = "tests/random_ascii";
    encode(test_file + ".in", test_file + ".enc");
    decode(test_file + ".enc", test_file + ".res");
    std::ifstream first(test_file + ".in");
    std::stringstream first_file, second_file;
    first_file << first.rdbuf();
    std::ifstream second(test_file + ".res");
    second_file << second.rdbuf();
    EXPECT_EQ(first_file.str(), second_file.str());
}

TEST(correctness, random_unicode) {
    std::string test_file = "tests/random_unicode";
    encode(test_file + ".in", test_file + ".enc");
    decode(test_file + ".enc", test_file + ".res");
    std::ifstream first(test_file + ".in");
    std::stringstream first_file, second_file;
    first_file << first.rdbuf();
    std::ifstream second(test_file + ".res");
    second_file << second.rdbuf();
    EXPECT_EQ(first_file.str(), second_file.str());
}

TEST(correctness, double_compression_ascii) {
    std::string test_file = "tests/random_ascii";
    encode(test_file + ".in", test_file + ".enc");
    encode(test_file + ".enc", test_file + ".enc2");
    decode(test_file + ".enc2", test_file + ".enc");
    decode(test_file + ".enc", test_file + ".res");
    std::ifstream first(test_file + ".in");
    std::stringstream first_file, second_file;
    first_file << first.rdbuf();
    std::ifstream second(test_file + ".res");
    second_file << second.rdbuf();
    EXPECT_EQ(first_file.str(), second_file.str());
}

TEST(correctness, double_compression_unicode) {
    std::string test_file = "tests/random_unicode";
    encode(test_file + ".in", test_file + ".enc");
    encode(test_file + ".enc", test_file + ".enc2");
    decode(test_file + ".enc2", test_file + ".enc");
    decode(test_file + ".enc", test_file + ".res");
    std::ifstream first(test_file + ".in");
    std::stringstream first_file, second_file;
    first_file << first.rdbuf();
    std::ifstream second(test_file + ".res");
    second_file << second.rdbuf();
    EXPECT_EQ(first_file.str(), second_file.str());
}

TEST(correctness, decode_no_info) {
    std::string test_file = "tests/random_ascii";
    EXPECT_ANY_THROW(decode(test_file + ".in", test_file + ".res"));
}

TEST(correctness, incorrect_alphabet) {
    std::string test_file = "tests/damaged_alphabet";
    EXPECT_ANY_THROW(decode(test_file + ".in", test_file + ".res"));
}

TEST(correctness, whitespaces) {
    std::string test_file = "tests/whitespaces";
    encode(test_file + ".in", test_file + ".enc");
    encode(test_file + ".enc", test_file + ".enc2");
    decode(test_file + ".enc2", test_file + ".enc");
    decode(test_file + ".enc", test_file + ".res");
    std::ifstream first(test_file + ".in");
    std::stringstream first_file, second_file;
    first_file << first.rdbuf();
    std::ifstream second(test_file + ".res");
    second_file << second.rdbuf();
    EXPECT_EQ(first_file.str(), second_file.str());
}

TEST(correctness, damaged_info_structure) {
    std::string test_file = "tests/damaged_structure";
    EXPECT_ANY_THROW(decode(test_file + ".in", test_file + "res"));
}

TEST(correctness, range255) {
    std::string test_file = "tests/range";
    std::ofstream fout("tests/range.in", std::ios::out | std::ios::binary);
    for (uint8_t i = 0; i < 255; i++) {
        fout.write((char *) &i, sizeof(uint8_t));
    }
    fout.close();
    encode(test_file + ".in", test_file + ".enc");
    decode(test_file + ".enc", test_file + ".res");
    std::ifstream first(test_file + ".in");
    std::stringstream first_file, second_file;
    first_file << first.rdbuf();
    std::ifstream second(test_file + ".res");
    second_file << second.rdbuf();
    EXPECT_EQ(first_file.str(), second_file.str());
}

TEST(correctness, random_medium){
    std::string test_file = "tests/range";
    std::ofstream fout("tests/range.in", std::ios::out | std::ios::binary);
    const size_t blocks = 1 << 19;
    for (size_t i = 0; i < blocks; i++) {
        uint32_t nxt = rand();
        fout.write((char *) &nxt, sizeof(uint32_t));
    }
    fout.close();
    encode(test_file + ".in", test_file + ".enc");
    decode(test_file + ".enc", test_file + ".res");
    std::ifstream first(test_file + ".in");
    std::stringstream first_file, second_file;
    first_file << first.rdbuf();
    std::ifstream second(test_file + ".res");
    second_file << second.rdbuf();
    EXPECT_EQ(first_file.str(), second_file.str());
}



TEST(correctness, random_medium_double_encode){
    std::string test_file = "tests/range";
    std::ofstream fout("tests/range.in", std::ios::out | std::ios::binary);
    const size_t blocks = 1 << 19;
    for (size_t i = 0; i < blocks; i++) {
        uint32_t nxt = rand();
        fout.write((char *) &nxt, sizeof(uint32_t));
    }
    fout.close();
    encode(test_file + ".in", test_file + ".enc");
    encode(test_file + ".enc", test_file + ".enc2");
    decode(test_file + ".enc2", test_file + ".enc");
    decode(test_file + ".enc", test_file + ".res");
    std::ifstream first(test_file + ".in");
    std::stringstream first_file, second_file;
    first_file << first.rdbuf();
    std::ifstream second(test_file + ".res");
    second_file << second.rdbuf();
    EXPECT_EQ(first_file.str(), second_file.str());
}


TEST(correctness, random_large_256mb){
    srand(time(NULL));
    std::string test_file = "tests/range";
    std::ofstream fout("tests/range.in", std::ios::out | std::ios::binary);
    const size_t blocks = 1 << 25;
    for (size_t i = 0; i < blocks; i++) {
        uint32_t nxt = rand();
        fout.write((char *) &nxt, sizeof(uint32_t));
    }
    fout.close();
    encode(test_file + ".in", test_file + ".enc");
    decode(test_file + ".enc", test_file + ".res");
    std::ifstream first(test_file + ".in");
    std::stringstream first_file, second_file;
    first_file << first.rdbuf();
    std::ifstream second(test_file + ".res");
    second_file << second.rdbuf();
    EXPECT_EQ(first_file.str(), second_file.str());
}