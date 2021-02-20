# Compile the main executable
main.exe: blackjack.cpp blackjack.h
	g++ -Wall -Werror -pedantic -g --std=c++11 blackjack.cpp -o main.exe

# Remove automatically generated files
clean :
	rm -rvf *.exe *~ *.out *.dSYM *.stackdump
