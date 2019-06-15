#include <fstream>
#include <iostream>
#include <cstring>
#include "lib/huffman_lib.h"

bool is_file_empty(std::ifstream &pFile) {
    return pFile.peek() == std::ifstream::traits_type::eof();
}

int main(int argc, char *argv[]) {
    const size_t BLOCK_SIZE = 65536;
    try {
        if (argc != 4) {
            throw std::runtime_error("Usage: <mode> <input_file> <output_file>");
        }
        std::ifstream fin(argv[2], std::ios::in | std::ios::binary);
        std::ofstream fout(argv[3], std::ios::out | std::ios::binary);
        if (!(fin && fout)) {
            throw std::runtime_error("Error while opening input/output files");
        }
        if (is_file_empty(fin)) {
            fin.close();
            fout.close();
            return 0;
        }
        if (strcmp(argv[1], "-e") == 0) {
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
        } else if (strcmp(argv[1], "-d") == 0) {
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
        } else {
            fin.close();
            fout.close();
            throw std::runtime_error(R"(Unknown mode. Please, enter "-e" to encode or "-d" to decode.)");
        }
    } catch (std::runtime_error const &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}