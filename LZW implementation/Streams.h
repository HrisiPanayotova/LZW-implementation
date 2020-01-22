#pragma once
#include<iostream>
#include<string>

class InputStream {
public:
	virtual bool operator>>(unsigned int& i) = 0;
	virtual bool operator>>(char& ch) = 0;
};

class OutputStream {
public:
	virtual void operator<<(const unsigned int& num) = 0;
	virtual void operator<<(const std::string& str) = 0;
};


class FixedInputStream : public InputStream {

public:
	FixedInputStream(std::istream& isStream) : is(isStream) {};

	bool operator>>(unsigned int& num) {
		return (bool)this->is.read(reinterpret_cast<char*> (&num), 3);
	}

	bool operator>>(char& ch) {
		return (bool)this->is.get(ch);
	}

private:
	std::istream& is;
};

class FixedOutputStream : public OutputStream {

public:
	FixedOutputStream(std::ostream& outStream) : os(outStream) {};
	void operator<<(const unsigned int& num) {
		this->os.write(reinterpret_cast<const char*> (&num), 3);
	}

	void operator<<(const std::string& str) {
		this->os << str;
	}

private:
	std::ostream& os;
};


class BestInputStream : public InputStream {

public:
	BestInputStream(std::istream& in)
		: is(in), pending_bits(0), currentVal(0), max_bits(0)
	{ }
	bool operator>>(unsigned int& num)
	{
		while (this->pending_bits < this->max_bits)
		{
			char c;
			if (!this->is.get(c)) return false;
			unsigned int newValueShifted = (c & 0xff) << this->pending_bits;
			this->currentVal = this->currentVal | newValueShifted;
			this->pending_bits += 8;
		}
		unsigned int mask = ~(~0 << this->max_bits);
		num = this->currentVal & mask;
		this->currentVal = this->currentVal >> this->max_bits;
		this->pending_bits -= this->max_bits;
		return true;
	}

	bool operator>>(char& ch) {
		return (bool)this->is.get(ch);
	}

	void goToBeginning() {
		this->is.clear();
		this->is.seekg(0, std::ios::beg);
	}

	bool readBytesNeeded(unsigned int& num) {
		bool result = (bool)this->is.read(reinterpret_cast<char*> (&num), 1);
		this->max_bits = num;
		return result;
	}
private:
	std::istream& is;
	int max_bits;
	int pending_bits;
	unsigned int currentVal;
};

class BestOutputStream : public OutputStream {

public:

	BestOutputStream(std::ostream& out, unsigned int max_code)
		: os(out), pending_bits(0), value(0), max_bits(max_code)
	{ }
	~BestOutputStream() {
		this->writeBytesToStream(0);
	}

	void operator<<(const std::string& str) {
		this->os << str;
	}

	void writeBytesNeeded() {
		this->os.write(reinterpret_cast<const char*> (&this->max_bits), 1);
	}

	void operator<<(const unsigned int& i) {
		this->value |= i << this->pending_bits;
		this->pending_bits += this->max_bits;
		this->writeBytesToStream(8);
	}
private:
	void writeBytesToStream(const int maxBitsToLeave)
	{
		while (this->pending_bits >= maxBitsToLeave) {
			this->os.write(reinterpret_cast<const char*>(&this->value), 1);
			this->pending_bits -= 8;
			this->value >>= 8;
		}
	}
	std::ostream& os;
	int max_bits;
	int pending_bits;
	unsigned int value;
};




