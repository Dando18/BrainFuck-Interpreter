#pragma once

#include <algorithm>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <iterator>

namespace BF {

/* ------------------------------ */
/*          DEFINITION            */
/* ------------------------------ */
class Compiler {
   public:
    Compiler(size_t mem_size=30000);

    bool compile(const std::string& input, std::ostream& out=std::cout);

   private:
    size_t mem_size_;


    std::map<char, std::string> ops_;
};

static void init_BF_compile_map(std::map<char, std::string>& ops);

/* ------------------------------ */
/*        IMPLEMENTATION          */
/* ------------------------------ */
Compiler::Compiler(size_t mem_size) : mem_size_(mem_size) {
    init_BF_compile_map(ops_);
}

bool Compiler::compile(const std::string& input, std::ostream& out) {

    out << "#include <stdio.h>\n";
    out << "int main() { ";
    out << "char array[" << mem_size_ << "] = {0}; ";
    out << "char *ptr=array; ";

    /* interpret string */
    for (auto it = input.begin(); it != input.end(); it++) {
        auto& op_sym = *it;

        /* handle EOF and \n */
        if ((*it) == EOF || (*it) == '\n' || (*it) == '\r' || (*it) == '\t' || (*it) == ' ') {
            continue;
        }

        auto search = ops_.find(op_sym);

        if (search == ops_.end()) {
            /* bad input */
            std::cerr << "Invalid symbol: " << op_sym << "\n";
            continue;
        }

        out << search->second;
    }

    out << " return 0; }\n";

    return true;
}




void init_BF_compile_map(std::map<char, std::string>& ops) {
    /* clear to begin with */
    ops.clear();

    /* fill the ops map here */
    ops.insert( std::make_pair('>', "++ptr; ") );
    ops.insert( std::make_pair('<', "--ptr; ") );
    ops.insert( std::make_pair('+', "++*ptr; ") );
    ops.insert( std::make_pair('-', "--*ptr; ") );
    ops.insert( std::make_pair('.', "putchar(*ptr); ") );
    ops.insert( std::make_pair(',', "*ptr=getchar(); ") );
    ops.insert( std::make_pair('[', "while(*ptr){ ") );
    ops.insert( std::make_pair(']', "} ") );
}

}  // namespace BF