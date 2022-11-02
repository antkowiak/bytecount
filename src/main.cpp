//
// main.cpp - Print a frequency count of bytes in a file (or input stream)
//
// Written by Ryan Antkowiak
//

#include <vector>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

// buffer size - 64K
constexpr static const std::size_t BUF_SIZE = 64 * 1024;

// print out the vector of the byte frequencies
void print_vec(const std::string& path, const std::vector<std::size_t>& vec)
{
	std::cout << path << ":" << std::endl;

	for (std::size_t i = 0; i < 32; ++i)
	{
		for (std::size_t j = 0 ; j < 8 ; ++j)
			std::cout << "0x" << std::right << std::setfill('0') << std::setw(2)
			          << std::hex << (i + j * 32) << ": " << std::setfill(' ')
			          << std::setw(9) << std::dec << std::left << vec[(i + j * 32)] << " ";

		std::cout << std::endl;
	}

	std::cout << std::endl;
}

// read input from stdin
void read_stdin()
{
	std::vector<std::size_t> vec(256);
	std::streamsize bytes_read{ 0 };
	char* buffer = static_cast<char*>(std::malloc(BUF_SIZE));

	if (buffer == nullptr)
		return;

	try
	{
		do
		{	
			bytes_read = std::fread(buffer, 1, BUF_SIZE, stdin);

			for (std::streamsize i = 0; i < bytes_read; ++i)
			{
				uint8_t b = buffer[i];
				++(vec[b]);
			}

		} while (bytes_read != 0);
	}
	catch (...)
	{
		std::free(buffer);
		return;
	}

	std::free(buffer);
	print_vec("[stdin]", vec);
}

// read input from a file
void read_file(const std::string& path)
{
	std::vector<std::size_t> vec(256);
	std::streamsize bytes_read{ 0 };
	char* buffer = static_cast<char*>(std::malloc(BUF_SIZE));

	if (buffer == nullptr)
		return;

	try
	{
		std::ifstream is(path, std::ifstream::binary);

		if (!is.good())
		{
			std::free(buffer);
			return;
		}

		do
		{
			is.read(buffer, BUF_SIZE);
			bytes_read = is.gcount();

			for (std::streamsize i = 0; i < bytes_read; ++i)
			{
				uint8_t b = buffer[i];
				++(vec[b]);
			}

		} while (bytes_read != 0);

		is.close();
	}
	catch (...)
	{
		std::free(buffer);
		return;
	}

	std::free(buffer);
	print_vec(path, vec);
}

// main program entry point
int main(int argc, const char* argv[])
{
	if (argc == 1)
		read_stdin();
	else
		for (int i = 1; i < argc; ++i)
			read_file(std::string(argv[i]));

	return EXIT_SUCCESS;
}
