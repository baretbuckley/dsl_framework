#pragma once

#include "tokenizer.hpp"

#include "token.hpp"

#include <vector>
#include <iostream>
#include <cstring>
#include <map>
#include <cstdlib>
#include <memory>
#include <set>
#include <stack>
#include <list>

namespace ast::srp {

class Parser {
    public:
    
    class VariableTree {
        struct Node {
            std::vector<TokenID> terminals;
            std::map<uint32_t, Node*> next;
            Node *last;
            Node(Node* last) : terminals(), next(), last(last) {}
            Node() : terminals(), next(), last(nullptr) {}
            ~Node();
        };

        Node *root_;

        public:
        VariableTree()
            : root_(new Node{}) {}
        ~VariableTree() {delete root_;}

        void loadRule(std::vector<TokenID> rule, TokenID result);

        class Iterator {
            Node * node_;
            public:
            Iterator(Node *node)
                : node_(node) {}
            
            bool next(TokenID t);
            bool back();
            bool terminal() {return !node_->terminals.empty();}
            const std::vector<TokenID> &terminals() {return node_->terminals;}
        };
        
        Iterator iterator() const {return Iterator(root_);}

        void tmpPrintTreeR(Node *n, int d, TokenMap &map) {
            // if (!n) return;
            // // Print terminals
            // for (auto k : n->terminals) {
            //     std::cout << " - " << map.getKeyword(k.id);
            // }
            // std::cout << '\n';
            // for (auto t : n->next) {
            //     for (int i = 0; i < d; i++)
            //         std::cout << "  ";
            //     std::cout << map.getKeyword(t.first) << " (" << t.first << ")";
            //     tmpPrintTreeR(t.second, d+1, map);
            // }
        }

        void tmpPrintTree(TokenMap &map) {
            tmpPrintTreeR(root_, 0, map);
        }

    };
    struct Node {
        TokenID token;
        VariableTree::Iterator pred;
        int depth;
        bool step(TokenID next);
        bool backStep();
        Node(const VariableTree &tree, Token &t);
        Node(const VariableTree &tree, TokenID &t);
    };

    private:
    VariableTree tree_;
    std::vector<Node> parserStack_;
    std::stack<uint32_t> active_;

    public:

    void tmpPrintTree(TokenMap &map) {
        tree_.tmpPrintTree(map);
    }

    void loadRule(std::vector<TokenID> rule, TokenID result) {
        tree_.loadRule(rule, result);
    }
    // T -> T + V | V
    // V -> V * L | L
    // L -> 0-9
    // 5 * 2 + 4
    // 5
    // l
    // v
    // v *  
    // 


    void parse(std::vector<Token> tokens) {
        // for (int i = 0; i < tokens.size(); i++) {
        //     std::cout << "insert: current-" << tokens[i].id() << '\n';
        //     parserStack_.emplace_back(tree_, tokens[i]);
        //     if (i != tokens.size()-1) stepStack(tokens[i+1]);
        //     else attemptReduce(TokenID(0));
        //     std::cout << "end of insert";
        //     for (auto t : parserStack_) std::cout << " - " << t.token.id;
        //     std::cout << "\n\n"z;
        // }
        // while (attemptReduce(TokenID(0)));
    }

    void stepStack(TokenID next) {
        // std::cout << "Step stack: next-" << next.id << " Successes";
        // // go through parser stack and try to step prediction tree
        // // by the given token, if step fails, attempt reduction
        // bool success = false;
        // for (int i = 0; i < parserStack_.size(); i++) {
        //     if (i + parserStack_[i].depth - 1 == parserStack_.size()) {
        //         if (parserStack_[i].step(next)) {
        //             success = true;
        //             std::cout << " - id:" << parserStack_[i].token.id << " idx:" << i;
        //         }
        //     }
        // }
        // std::cout << std::endl;
        // if (!success) attemptReduce(next);
    }

    bool attemptReduce(TokenID next) {
        // // Find first token in parserstack handling all following tokens
        // // in the parserstack and reduce it
        // std::cout << "Attempting Reduction\n";
        // for (int i = 0; i < parserStack_.size(); i++) {
        //     //tmp
        //     // std::cout << i + parserStack_[i].depth << ' ' << parserStack_.size() << ' ' << parserStack_[i].pred.terminal() << ' ' << parserStack_[i].pred.terminals().size() << std::endl;
        //     if (i + parserStack_[i].depth == parserStack_.size()) {
        //         if (parserStack_[i].pred.terminal()) return reduce(i, next);
        //     }
        // }
        // return false;
        // std::cout << "No valid reduction found\n";
    }

    bool reduce(int idx, TokenID next) {
        // std::cout << "reducing: id:" << parserStack_[idx].token.id << " idx:" << idx;
        // // Reduce index in parser stack by replaceing it with terminal
        // // token, and remove all following indexes. Any token
        // // proceding reduction, if it predicts at or past reduction, revive it
        // TokenID newToken = parserStack_[idx].pred.terminals().front();
        // std::cout << " - new ID: " << newToken.id << " - Stack size: " << parserStack_.size() << '\n'; 
        // parserStack_.erase(parserStack_.begin()+idx, parserStack_.end());
        // parserStack_.emplace_back(tree_, newToken);

        // for (int i = 0; i < idx; i++) {
        //     if (parserStack_[i].depth+i > idx) revive(i);
        // }
        // if (next.id) stepStack(next);
        // return true;
    }

    inline void revive(int idx) {
        // std::cout << "reviving: idx:" << idx << " id:" << parserStack_[idx].token.id
        //         << " - depth: " << parserStack_[idx].depth << std::endl;
        // // revive token by backstepping prediction to n-2 of parserStack size,
        // // then restep to last.
        // // back step to before the last idx due to change in token from reduction
        // while(parserStack_[idx].depth+idx > parserStack_.size()-1)
        //     parserStack_[idx].backStep();
        // parserStack_[idx].step(parserStack_.back().token);
    }

    // void parse(std::vector<Token> tokens) {
    //     for (auto &t : tokens) {
    //         std::cout << "Parsing token: " << t.id() << '\n';
    //         pushToken(t);
    //     }
    //     std::cout << "\nfinalizing\n";
    //     finalizeStack();
    // }

    // void pushToken(TokenID t) {
    //     if (pushParserStack(t)) return;
    //     reviveActive();
    // }

    // bool pushParserStack(TokenID t) {
    //     parserStack_.emplace_back(tree_, t);
    //     if (!parserStack_.back().depth) return false;
    //     active_.push(parserStack_.size()-1);
    //     return true;
    // }

    private:

    // void reduceActive() {
    //     if (active_.empty()) return;
    //     uint32_t i = active_.top();
    //     if (!parserStack_[i].pred.terminals().empty()) {
    //         std::cout << "reducing: " << parserStack_[i].token.id;
    //         for (int j = i+1; j < i+parserStack_[i].depth; j++) {
    //             std::cout << ", " << parserStack_[j].token.id;
    //         }
    //         std::cout << " - ";
            
    //         TokenID reduction = parserStack_[i].pred.terminals().front();
    //         std::cout << reduction.id << std::endl;

    //         parserStack_.erase(parserStack_.begin()+i+1,
    //                 parserStack_.begin()+i+parserStack_[i].depth);
    //         parserStack_[i] = Node(tree_, reduction);
            
    //         if (parserStack_[i].depth == 0) active_.pop();
    //     } else {
    //         active_.pop();
    //     }
    //     return reviveActive();
    // }

    // void reviveActive() {
    //     if (active_.size() == 0) return;
    //     // std::cout << "reviving : ";
    //     uint32_t i = active_.top();
    //     Node &top = parserStack_[i];
    //     // std::cout << top.token.id << " :";
    //     auto target = parserStack_.begin() + (i+top.depth);
    //     while (target != parserStack_.end()) {
    //         // std::cout << " - " << target->token.id;
    //         if (!top.pred.next(target->token))
    //             return reduceActive();
    //         target++;
    //         top.depth++;
    //     }
    //     // std::cout << '\n';
    // }

    // // bool attemptReviveActive() {
    // //     if (active_.size() == 0) return;
    // //     // std::cout << "reviving : ";
    // //     uint32_t i = active_.top();
    // //     Node &top = parserStack_[i];
    // //     // std::cout << top.token.id << " :";
    // //     auto target = parserStack_.begin() + (i+top.depth);
    // //     while (target != parserStack_.end()) {
    // //         // std::cout << " - " << target->token.id;
    // //         if (!top.pred.next(target->token)) return false;
    // //         target++;
    // //         top.depth++;
    // //     }
    // //     return true;
    // //     // std::cout << '\n';
    // // }

    // void finalizeStack() {
    //     while (!active_.empty()) {
    //         reduceActive();
    //     }
    // }

};

}