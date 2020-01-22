#include "LZW.h"

std::unordered_map<std::string, unsigned int> LZW::initializeCodeTable() {
	std::unordered_map<std::string, unsigned int> codeTable;
	for (int i = 0; i < 256; i++)
	{
		std::string temp = "";
		temp += (char)i;
		codeTable[temp] = i;
	}
	return codeTable;
}

void LZW::compressFixedBytes(std::istream& is, std::ostream& out) {
	FixedOutputStream outCompress(out);
	FixedInputStream inComp(is);
	this->compress(inComp , outCompress);
}

void LZW::compressBestBytes(std::istream& is, std::ostream& out) {
	FixedOutputStream def(std::cout);
	BestInputStream inComp(is);
	auto maxCode = this->compress(inComp, def, false);
	int bitsNeeded = 0;
	while (maxCode > 0) {
		bitsNeeded++;
		maxCode /= 2;
	}
	BestOutputStream outCompress(out, bitsNeeded);
	outCompress.writeBytesNeeded();
	inComp.goToBeginning();
	this->compress(inComp, outCompress);
}

void LZW::decompressBestBytes(std::istream& is, std::ostream& out) {
	BestInputStream inDec(is);
	unsigned int bitsNeeded = 0;
	inDec.readBytesNeeded(bitsNeeded);
	BestOutputStream outDecompress(out, bitsNeeded);
	this->decompress(inDec, outDecompress);
}


void LZW::decompressFixedBytes(std::istream& is, std::ostream& out) {
	FixedOutputStream outCompress(out);
	FixedInputStream inComp(is);
	this->decompress(inComp, outCompress);
}

std::unordered_map<unsigned int, std::string> LZW::initializeReverseCodeTable() {
	std::unordered_map<unsigned int, std::string> codeTable;
	for (int i = 0; i < 256; i++)
	{
		std::string temp = "";
		temp += (char)i;
		codeTable[i] = temp;
	}
	return codeTable;
}

unsigned int LZW::compress(InputStream& is, OutputStream& out, bool shouldWrite)
{
	auto codeTable = this->initializeCodeTable();
	std::string pref = "";
	unsigned int curCode = 257;
	std::vector<unsigned int> codes;
	char c;
	while(is >> c)
	{
		if (codeTable.find(pref + c) != codeTable.end()) pref = pref + c;
		else
		{
			if(shouldWrite) out << codeTable[pref];
			codeTable[pref + c] = curCode++;
			pref = c;
		}

	}
	if (codeTable.find(pref) == codeTable.end()) codeTable[pref] = curCode;
	auto code = codeTable[pref];
	if (shouldWrite) out << code;
	if (shouldWrite) out << EOF_CODE;
	return curCode;
}

void LZW::decompress(InputStream& is, OutputStream& out) {
	auto codeTable = this->initializeReverseCodeTable();
	unsigned int old = 0;
	is >> old;
	out << "" + codeTable[old];
	unsigned int curCode = 257;
	unsigned int temp = 0;
	while (is >> temp && temp != EOF_CODE)
	{
		if (codeTable.find(temp) != codeTable.end()) {
			auto tempStr = codeTable[temp];
			out << tempStr;
			codeTable[curCode++] = codeTable[old] + tempStr[0];
			old = temp;
		}
		else {
			auto tempStr = codeTable[old];
			codeTable[curCode++] = tempStr + tempStr[0];
			out << (tempStr + tempStr[0]);
		}
		temp = 0;
	}
}



