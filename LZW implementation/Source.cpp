#include <iostream>
#include <queue>
#include <fstream>
#include <sstream>
#include "LZW.h"
#include <filesystem>

namespace fs = std::filesystem;

struct CompressionResult {
	unsigned int sizeBefore;
	unsigned int sizeAfter;
	float compressionRatio;
};

enum class ProgramTypes {
	CompressFixedLZWFile, CompressChangeableLZWFile, DecompressFixedLZWFile, DecompressChangeableLZWFile,
};


void decompressFile(std::string input, std::string output, ProgramTypes programType) {
	std::ifstream inputDecompression(input, std::ios::in | std::ios::binary);
	std::ofstream outputDecompession(output, std::ios::out | std::ios::binary);
	LZW lzw;
	if (programType == ProgramTypes::DecompressFixedLZWFile) lzw.decompressFixedBytes(inputDecompression, outputDecompession);
	if (programType == ProgramTypes::DecompressChangeableLZWFile) lzw.decompressBestBytes(inputDecompression, outputDecompession);
}

CompressionResult compressFile(std::string input, std::string output, ProgramTypes programType) {
	std::ifstream inputCom(input, std::ios::in | std::ios::binary);
	std::ofstream outputCom(output, std::ios::out | std::ios::binary);

	LZW lzw;
	if (programType == ProgramTypes::CompressFixedLZWFile) lzw.compressFixedBytes(inputCom, outputCom);
	if (programType == ProgramTypes::CompressChangeableLZWFile) lzw.compressBestBytes(inputCom, outputCom);
	inputCom.close();
	outputCom.close();
	CompressionResult res;
	res.sizeBefore = std::filesystem::file_size(input);
	res.sizeAfter = std::filesystem::file_size(output);
	res.compressionRatio = (float)res.sizeAfter / res.sizeBefore;
	return res;
}

void compressFilesFromDirectory(std::string dirName) {

	std::string fixedDir = "fixedLZWCompression";
	if (fs::exists(fixedDir)) !fs::remove_all(fixedDir);
	if (!fs::create_directory(fixedDir)) throw "error occured while creating dir";

	std::string changeableDir = "changeableLZWCompression";
	if (fs::exists(changeableDir)) fs::remove_all(changeableDir);
	if (!fs::create_directory(changeableDir)) throw "error occured while creating dir";

	std::cout << "Compressing files from directory, please wait :)" << std::endl;

	std::ofstream csvResult("result.csv");
	csvResult << "File Name, File Size Before, File Size After LZW Fixed, Compression Ratio LZW Fixed, File Size After LZW Changeable, Compression Ratio LZW Changeable" << std::endl;

	for (auto& p : fs::directory_iterator(dirName)) {
		auto filepath = p.path();
		auto filename = fs::path(filepath).filename().u8string();
		auto lzwFixedResult = compressFile(filepath.u8string(), fixedDir + "//" + filename, ProgramTypes::CompressFixedLZWFile);
		auto lzwChangeableResult = compressFile(filepath.u8string(), changeableDir + "//" + filename, ProgramTypes::CompressChangeableLZWFile);
		csvResult << filename << ',' << lzwFixedResult.sizeBefore << ',' << lzwFixedResult.sizeAfter << ',' <<
			std::setprecision(2) << std::fixed << lzwFixedResult.compressionRatio * 100 << '%' << ',' << lzwChangeableResult.sizeAfter << ',' << std::setprecision(2) << std::fixed <<
			lzwChangeableResult.compressionRatio * 100 << '%' <<std::endl;
	}

	std::cout << "Compression finished!" << std::endl;
}

void decompressFilesFromDirectory(std::string dirInput, std::string dirOutput, ProgramTypes type) {

	if (fs::exists(dirOutput)) !fs::remove_all(dirOutput);
	if (!fs::create_directory(dirOutput)) throw "error occured while creating dir";

	std::cout << "Decompressing files from directory, please wait :)" << std::endl;

	for (auto& p : fs::directory_iterator(dirInput)) {
		auto filepath = p.path();
		auto filename = fs::path(filepath).filename().u8string();
		decompressFile(filepath.u8string(), dirOutput + "//" + filename, type);
	}

	std::cout << "Decompression finished!" << std::endl;
}




int main(int argv, const char** argc) {
	if (argv < 2) {
		std::cerr << "Too few arguments passed!" << std::endl;
		exit(1);
	}

	std::string mode = argc[1];
	CompressionResult res;
	if (mode == "-comprff") res = compressFile(argc[2], argc[3], ProgramTypes::CompressFixedLZWFile);
	if (mode == "-comprfc") res = compressFile(argc[2], argc[3], ProgramTypes::CompressChangeableLZWFile);
	if (mode == "-decomprff") decompressFile(argc[2], argc[3], ProgramTypes::DecompressFixedLZWFile);
	if (mode == "-decomprfc") decompressFile(argc[2], argc[3], ProgramTypes::DecompressChangeableLZWFile);
	if (mode == "-comprdirall") compressFilesFromDirectory(argc[2]);
	if (mode == "-decomprdirf") decompressFilesFromDirectory(argc[2], argc[3], ProgramTypes::DecompressFixedLZWFile);
	if (mode == "-decomprdirc") decompressFilesFromDirectory(argc[2], argc[3], ProgramTypes::DecompressChangeableLZWFile);
	if (mode == "-comprdirallcantrbry") compressFilesFromDirectory("../resources/cantrbry");
}