#pragma once
#include<string>
#include<unordered_map>
#include<vector>
#include<fstream>
#include"Streams.h"

const unsigned int EOF_CODE = 256;

class LZW {
private:
	std::unordered_map<std::string, unsigned int> initializeCodeTable();
	std::unordered_map<unsigned int, std::string> initializeReverseCodeTable();
	unsigned int compress(InputStream& is, OutputStream& out, bool shouldWrite = true);
	void decompress(InputStream& is, OutputStream& out);
public: 
	void compressFixedBytes(std::istream& is, std::ostream& out);
	void compressBestBytes(std::istream& is, std::ostream& out);
	void decompressFixedBytes(std::istream& is, std::ostream& out);
	void decompressBestBytes(std::istream& is, std::ostream& out);
};
