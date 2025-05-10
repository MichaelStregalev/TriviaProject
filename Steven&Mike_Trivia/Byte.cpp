#include "Byte.h"
#include "TriviaExceptions.h"

// DEFAULT CONSTRUCTOR
Byte::Byte() :
_value(0)
{
}

// CONSTRUCTOR BY BYTE VALUE
Byte::Byte(unsigned char value) :
_value(value)
{
}

// CONSTRUCTOR BY INT VALUE
Byte::Byte(int value)
{
	// A byte can only hold values between 0-255
	if (value > MAX_BYTE_VALUE || value < MIN_BYTE_VALUE)
	{
		throw InvalidByteValueException();
	}

	_value = static_cast<unsigned char>(value);
}

// CONSTRUCTOR BY BINARY CODE
Byte::Byte(const std::string& binary)
{
	_value = binaryToByte(binary);
}

// SETTER BY BYTE VALUE
void Byte::setValue(unsigned char value)
{
	_value = value;
}

// SETTER BY INT VALUE
void Byte::setValue(int value)
{
	// A byte can only hold values between 0-255
	if (value > MAX_BYTE_VALUE || value < MIN_BYTE_VALUE)
	{
		throw InvalidByteValueException();
	}

	_value = static_cast<unsigned char>(value);
}

// SETTER BY BINARY CODE
void Byte::setValue(const std::string& binary)
{
	_value = binaryToByte(binary);
}

// GET THE BINARY REPRESENTATION OF THE BYTE
std::string Byte::binaryCode() const
{
	std::string binary = "";

	// Go through all of the 8 bits of the value and write them onto the string

	for (int i = 7; i >= 0; i--)
	{
		// 1 << i
		// is the left shift of 00000001 by i times.
		// which means multiplying it by i*2
		// if i = 3
		// it will result in
		// 00001000

		// with that, we can check each specific bit in the value.
		// by using & (AND binary), we can check if the bit in place i (where the first bit is place 8, and the last 1)
		// is turned on or is off.
		
		// EXAMPLE:
		// value:			01101010
		// comparison:		00001000
		// result of &:		00001000
		// if the result isn't 0 - it means the bit in place i is 1.
		// if the result has gone to 0 - it means the bit in place i is 0!

		if ((_value & (1 << i)) != 0)
		{
			binary += BIT_ON;
		}
		else
		{
			binary += BIT_OFF;
		}
	}

	return binary;
}

// GET THE DECIMAL VALUE OF THE BYTE
int Byte::decimalValue() const
{
	return static_cast<int>(_value);
}

// GET THE ASCII CHAR VALUE OF THE BYTE
char Byte::asciiChar() const
{
	return static_cast<char>(_value);
}

bool Byte::operator==(const Byte& other) const
{
	return _value == other._value;
}

bool Byte::operator==(int value) const
{
	return _value == static_cast<unsigned char>(value);
}

bool Byte::operator==(const std::string& binary) const
{
	return binaryCode() == binary;
}

bool Byte::operator==(char asciiChar) const
{
	return _value == static_cast<unsigned char>(asciiChar);
}

Byte::Buffer Byte::separateBinary(const std::string& binary)
{
	Buffer buffer;
	std::string tempBinary = binary;

	// Make sure the binary string length is a multiple of 8
	while (tempBinary.length() % BITS_IN_BYTE != 0)
	{
		tempBinary += BIT_OFF;  // pad the string with leading zeros
	}

	// Split the binary string into bytes (8 bits each), and cast each part to a byte
	for (int i = 0; i < tempBinary.length(); i += BITS_IN_BYTE)
	{
		std::string byteBinary = tempBinary.substr(i, BITS_IN_BYTE);	// get a byte (8 bits)
		Byte byte(byteBinary);											// convert the binary to a Byte object
		buffer.push_back(byte);											// add it to the buffer
	}

	return buffer;
}

int Byte::calculateDecimalValue(const std::string& binary)
{
	int decimalValue = 0;

	// Traverse the binary string from left to right
	for (int i = 0; i < binary.length(); i++) 
	{
		if (binary[i] == BIT_ON) 
		{
			decimalValue += (1 << (binary.length() - 1 - i));  // calculate the decimal value
		}
	}

	return decimalValue;
}

std::string Byte::deserializeBytesToString(const Buffer& bytes)
{
	std::string result;

	for (const Byte& byte : bytes) 
	{
		result += byte.asciiChar();  // convert each Byte to its ASCII character and append it
	}

	return result;
}

Byte::Buffer Byte::intToBuffer(int number)
{
	Buffer buffer;

	// An int has 4 bytes, so we will make a buffer with 4 bytes that represent the number!

	for (int i = 3; i >= 0; i--) // 4 bytes for int
	{
		// Using this bit manipulation, we are harvesting a single byte at a time from the number,
		// by using AND binary with 0xFF (11111111) we will replicate the last byte of the number.
		buffer.push_back(Byte((number >> (BITS_IN_BYTE * i)) & 0xFF));
	}

	return buffer;
}

Byte::Buffer Byte::stringToBuffer(const std::string& content)
{
	Buffer buffer;

	for (char c : content)
	{
		buffer.push_back(Byte(static_cast<unsigned char>(c)));
	}

	return buffer;
}

// STATIC HELPER METHOD - BINARY CODE TO BYTE VALUE (unsigned char)
unsigned char Byte::binaryToByte(const std::string& binary)
{
	// First, we will need to check that the string represents a binary code at first,
	// and then we will need to make sure it is a byte by being 8 bits long.

	if (!isBinaryCode(binary))
	{
		throw InvalidBinaryStringFormatException();
	}
	else if (binary.length() != BITS_IN_BYTE)
	{
		throw InvalidBinaryStringLengthException();
	}

	unsigned char value = 0;

	// We will now calculate the value by going through the binary code - and replicating it onto the value.
	// Each time we shift left (multiplying the value by 2) the value by 1 bit, it means we are ready to go on to the next bit.
	// If the current bit in the binary is turned on -> we will set the least significant bit (the last one-current one) to 1,
	// if its turned off, we will let is stay off.
	
	for (char bit : binary) 
	{
		value <<= 1;		// Shift result to the left by 1 bit
		if (bit == BIT_ON)
		{
			value |= 1;		// If current bit is turned on, set on the least significant bit
		}
	}

	return value;
}

// IS A STRING REPRESENTING BINARY CODE?
bool Byte::isBinaryCode(const std::string& binary)
{
	if (binary.empty())
	{
		return false; // Return false if the string is empty
	}

	for (char c : binary)
	{
		if (c != BIT_OFF && c != BIT_ON	)
		{
			return false; // Found a non-binary character!
		}
	}
	return true; // All characters are '0' or '1' -> Binary code
}