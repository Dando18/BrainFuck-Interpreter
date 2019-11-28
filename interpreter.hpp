#pragma once

#include <algorithm>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <iterator>

namespace BF {

/* global constexprs */
constexpr size_t DEFAULT_MEM_SIZE = static_cast<size_t>(30000);

struct OpInfo;
/* typedefs */
typedef unsigned char mem_t;
typedef mem_t* Ptr;
typedef std::function<bool(OpInfo&)> Operation;



/* ------------------------------ */
/*          DEFINITION            */
/* ------------------------------ */
struct OpInfo {
    Ptr& ptr;
    const std::string& input;
    std::string::const_iterator& it;
    const std::vector<std::string::const_iterator>& jump_table;

    friend class Interpreter;

    OpInfo(Ptr& ptr, const std::string& input, std::string::const_iterator& it, const std::vector<std::string::const_iterator>& jump_table) :
        ptr(ptr), input(input), it(it), jump_table(jump_table) {}
};

class Interpreter {
   public:
    Interpreter();
    Interpreter(size_t mem_size);

    void reset();

    bool interpret(const std::string& input);

   private:
    size_t mem_size_;
    std::unique_ptr<mem_t[]> mem_;
    Ptr ptr_;

    std::string::const_iterator init_jump_table(const std::string& input, std::string::const_iterator);

    std::map<char, Operation> ops_;
    std::vector<std::string::const_iterator> jump_table_;
};

//static void add_operation(std::map<char, Operation>& ops, char op_name, Operation& op);
static void init_BF_operation_map(std::map<char, Operation>& ops);

/* ------------------------------ */
/*        IMPLEMENTATION          */
/* ------------------------------ */
Interpreter::Interpreter() : Interpreter(DEFAULT_MEM_SIZE) {}

Interpreter::Interpreter(size_t mem_size) : mem_size_(mem_size), mem_(new mem_t[mem_size_]), ptr_(mem_.get()) {
    /* init vals to 0 */
    reset();

    init_BF_operation_map(ops_);
}

void Interpreter::reset() {
    /* set all values of mem_ to 0 */
    std::fill(mem_.get(), mem_.get() + mem_size_, static_cast<mem_t>(0));

    /* set ptr_ to beginning of mem_ */
    ptr_ = mem_.get();
}

bool Interpreter::interpret(const std::string& input) {
    jump_table_.clear();
    jump_table_.resize(input.size(), input.end());
    init_jump_table(input, input.begin());

    /* interpret string */
    for (auto it = input.begin(); it != input.end(); it++) {
        auto& op_sym = *it;

        /* handle EOF and \n */
        if ((*it) == EOF || (*it) == '\n' || (*it) == '\r' || (*it) == '\t') {
            continue;
        }

        auto search = ops_.find(op_sym);

        if (search == ops_.end()) {
            /* bad input */
            std::cerr << "Invalid symbol: " << op_sym << "\n";
            continue;
        }

        Operation& op = search->second;

        OpInfo info (ptr_, input, it, jump_table_);
        if (!op(info)) {
            std::cerr << "Operation failed: " << op_sym << "\n";
        }
    }

    return true;
}

std::string::const_iterator Interpreter::init_jump_table(const std::string& input, std::string::const_iterator it) {
    for (; it != input.end(); it++) {

        std::string::const_iterator ending;
        std::string::const_iterator tmp;
        switch (*it) {
            case ']':
                return it;
            case '[':
                tmp = it;
                tmp++;
                ending = init_jump_table(input, tmp);

                /* bad match */
                if (ending == input.end()) return ending;
                
                jump_table_[std::distance(input.begin(), it)] = ending;
                jump_table_[std::distance(input.begin(), ending)] = it;
                it = ending;
        }
    }
    return input.end();
}

static void add_operation(std::map<char, Operation>& ops, char op_name, Operation op) {
    ops.insert(std::make_pair(op_name, op));
}

static void init_BF_operation_map(std::map<char, Operation>& ops) {
    /* clear to begin with */
    ops.clear();

    /* fill the ops map here */
    add_operation(ops, '>', [](OpInfo& info) -> bool {
        ++(info.ptr);
        return true;
    });
    add_operation(ops, '<', [](OpInfo& info) -> bool {
        --(info.ptr);
        return true;
    });
    add_operation(ops, '+', [](OpInfo& info) -> bool {
        ++(*(info.ptr));
        return true;
    });
    add_operation(ops, '-', [](OpInfo& info) -> bool {
        --(*(info.ptr));
        return true;
    });
    add_operation(ops, '.', [](OpInfo& info) -> bool {
        std::cout << (*(info.ptr));
        return true;
    });
    add_operation(ops, ',', [](OpInfo& info) -> bool {
        std::cin >> (*(info.ptr));
        return true;
    });
    add_operation(ops, '[', [](OpInfo& info) -> bool {
        /* move iterator to ending ] */

        /* only move to end if value at ptr is 0 */
        if ((*info.ptr) != static_cast<mem_t>(0)) {
            return true;
        }

        info.it = info.jump_table[std::distance(info.input.begin(), info.it)];

        return true;
    });
    add_operation(ops, ']', [](OpInfo& info) -> bool {
        if ((*info.ptr) == 0) {
            return true;
        }

        info.it = info.jump_table[std::distance(info.input.begin(), info.it)];
        return true;
    });
}

}  // namespace BF