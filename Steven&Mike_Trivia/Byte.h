#pragma once

#include <string>
#include <vector>

// DEFINES
#define MAX_BYTE_VALUE 255				// The maximum value a byte can hold is 255
#define MIN_BYTE_VALUE 0				// The minimum value a byte can hold is 0

#define BIT_ON	'1'						// The value of a bit when it is on
#define BIT_OFF '0'						// The value of a bit when it is off

#define BITS_IN_BYTE	8				// The amount of bits in a byte

/*
							BYTE
	This class represents a byte, which is represented usually
	by an unsigned char.
	Though, for efficiency, we created this class!
	This class lets us use the bytes in the serialization and deserialization
	in a way simpler way.
*/

class Byte
{
public:

	// A buffer is a vector of bytes.
	using Buffer = std::vector<Byte>;

	// <-- CONSTRUCTORS -->
	Byte();								// default - sets to 0
	Byte(unsigned char value);			// by byte value
	Byte(int value);					// by int value
	Byte(const std::string& binary);	// by binary code

	// <-- SETTERS -->
	void setValue(unsigned char value);
	void setValue(int value);
	void setValue(const std::string& binary);

	// <-- METHODS -->
	std::string binaryCode() const;		// Returns a string representing the binary code of the byte
	int decimalValue() const;			// Returns the int value of the byte
	char asciiChar() const;				// Returns the ascii character in the byte

	// <-- OPERATORS -->
	bool operator==(const Byte& other) const;
	bool operator==(int value) const;
	bool operator==(const std::string& binary) const;
	bool operator==(char asciiChar) const;

	// <-- STATIC METHODS -->
	// This function is responsible for getting a binary code, and seperating each byte and returning them all as
	// a buffer - a vector of all the bytes.
	static Buffer separateBinary(const std::string& binary);
	// This function is responsible for calculating the int value of a larger than usual binary code sequence,
	// will help us when figuring out the data length in the serialization.
	static int calculateDecimalValue(const std::string& binary);
	// This function will help us serialize a buffer of bytes onto a string by translating each byte onto an ascii character.
	static std::string deserializeBytesToString(const Buffer& bytes);
	// This function will get a number, and return a buffer that represents all the bytes that together make up the number.
	static Buffer intToBuffer(int number);
	// This function will get a string, and return a buffer that represents all of the bytes that together make up the string!
	static Buffer stringToBuffer(const std::string& content);
	// ....
	static char* bufferToCharArray(const Buffer& bytes);

private:

	// <-- FIELDS -->
	unsigned char _value;	// The value of the byte itself

	// <-- PRIVATE HELPER METHODS -->
	// This function will help us translate binary code onto a byte - unsigned char.
	static unsigned char binaryToByte(const std::string& binary);
	// This function makes sure that the string given to us represents a binary code.
	static bool isBinaryCode(const std::string& binary);
};