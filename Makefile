bytecount : Makefile src/main.cpp
	clang++ -std=c++17 -Wall -Wextra -Wpedantic src/main.cpp -o bytecount

clean :
	\rm -f bytecount

