#pragma once

#include <map>
#include <stdint.h>
#include <optional>
#include <string>
#include <vector>
#include <iostream>

#include "inputBuffer.hpp"
#include "token.hpp"

namespace ast::fa {

template<typename R>
class Node {
    std::map<uint32_t, Node<R>*> edges_;
    uint32_t acceptance_;

    public:

    Node(uint32_t &&accept) : acceptance_(accept) {}
    Node(uint32_t &accept) : acceptance_(accept) {}

    R &acceptance() {return acceptance_;}

    void addTransition(uint32_t value, Node<R>* destination) {
        edges_[value] = destination;
    }

    virtual R undefinedEdge(InputBuffer<> &input, Token &t) {
        t.end = input.pos();
        t.id = acceptance_;
        return acceptance_;
    }

    R parse(InputBuffer<> &input, Token &t) {

        std::cout << "Parsing" << std::endl;
        char n;
        if (input.eof()) {
            t.end = input.pos();
            t.id = acceptance_;
            return acceptance_;
        }
        // std::cout << "code: " << *str << std::endl;
        auto next = edges_[input.get()];
        if (next == nullptr) return undefinedEdge(input, t);
        return next->parse(input.next(), t);
    }
};

// template<typename R>
// class Node_undefinedTransition : public Node<R> {
    
//     Node<R> *transition_;

//     public:
//     Node_undefinedTransition(R &accept, Node<R> *undefEdge)
//         : Node<R>(accept), transition_(undefEdge) {}
    

//     virtual R undefinedEdge(InputBuffer<> &input) override {
//         return transition_->parse(input.next());
//     }
// };

}