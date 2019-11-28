#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "interpreter.hpp"
#include "compiler.hpp"

enum MODE {
    INTERPRET,
    COMPILE
};

int main(int argc, char **argv) {
    std::string input;

    MODE mode;

    if (argc == 3) {
        if (argv[1][0] == 'i') {
            mode = INTERPRET;
        } else {
            mode = COMPILE;
        }

        std::string f_name(argv[2]);

        std::ifstream t(f_name);
        std::stringstream buffer;
        buffer << t.rdbuf();

        input = buffer.str();
    } else {
        std::cerr << "usage: " << argv[0] << " i|c file.bf\n";
        std::exit(1);
    }

    if (mode == INTERPRET) {
        BF::Interpreter inter;

        inter.interpret(input);
    } else {
        std::ofstream out ("a.c");
        BF::Compiler comp (100000);
        comp.compile(input, out);
    }

    return 0;
}