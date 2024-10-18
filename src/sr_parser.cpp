#include "sr_parser.hpp"

namespace  ast::srp {

    // Parser::TokenID::TokenID(Token &token)
    //     : id(token.id())
    // {
    //     if (token.type() == Token::Identifier)
    //         id = 0;
    // }

    // Parser::VariableTree::Node::~Node() {
    //     for (auto &it : next) {
    //         delete it.second;
    //     }
    // }

    // void Parser::VariableTree::loadRule(std::vector<TokenID> rule, TokenID result) {
    //     auto node = root_;
    //     std::map<uint32_t, Node*>::iterator it;
    //     std::cout << "loading rule: " << result.id << " : ";
    //     for (auto t : rule) {
    //         std::cout << t.id << ' ';
    //         it = node->next.find(t.id);
    //         if (it != node->next.end()) {
    //             // Move down tree if token already exists as branch
    //             node = it->second;
    //         } else {
    //             // Add new branch as token and move into it
    //             node = (node->next[t.id] = new Node(node));
    //         }
    //     }
    //     std::cout << "\n";
    //     node->terminals.push_back(result);
    // }

    // bool Parser::VariableTree::Iterator::next(TokenID t) {
    //     auto it = node_->next.find(t.id);
    //     if (it == node_->next.end()) return false;
    //     node_ = (*it).second;
    //     return true;
    // }

    // bool Parser::VariableTree::Iterator::back() {
    //     if (!node_->last) return false;
    //     node_ = node_->last;
    //     return true;
    // }

    // Parser::Node::Node(const VariableTree &tree, Token &t)
    //     : token(t), pred(tree.iterator()), depth(0)
    // {
    //     step(t);
    // }

    // Parser::Node::Node(const VariableTree &tree, TokenID &t)
    //     : token(t), pred(tree.iterator()), depth(0)
    // {
    //     step(t);
    // }

    // bool Parser::Node::step(TokenID next) {
    //     if (!pred.next(next)) return false;
    //     depth++;
    //     return true;
    // }

    // bool Parser::Node::backStep() {
    //     if (!pred.back()) return false;
    //     depth--;
    //     return true;
    // }

}