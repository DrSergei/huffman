#include "Tree.h"

#include <queue>
#include <stdexcept>

namespace tree {
    Tree::Node::Node(uint8_t ch, uint32_t weight, Tree::Node *left, Tree::Node *right) : weight(weight), left(left), right(right), ch(ch) {}

    Tree::Node::~Node() {
        delete left;
        delete right;
    }

    bool Tree::NodeCmp::operator()(const Tree::Node *first, const Tree::Node *second) {
        if (first == nullptr || second == nullptr)
            throw std::logic_error("Huffman tree error");
        return first->weight > second->weight;
    }

    Tree::Tree(std::map<uint8_t, uint32_t> const &data) : root(nullptr), current(nullptr) {
        init(data);
    }

    Tree::~Tree() {
        delete root;
    }

    void Tree::init(const std::map<uint8_t, uint32_t> &data) {
        if (data.empty())
            return;
        std::priority_queue<Node *, std::vector<Node *>, NodeCmp> buffer;
        for (const auto &[ch, weight]: data)
            buffer.push(new Node(ch, weight));
        while (buffer.size() > 1) {
            Node *left = buffer.top();
            buffer.pop();
            Node *right = buffer.top();
            buffer.pop();
            // left and right isn't nullptr
            buffer.push(new Node(' ', left->weight + right->weight, left, right));
        }
        root = buffer.top();
        current = root;
    }

    std::map<uint8_t, std::vector<bool>> Tree::getCodes() {
        std::map<uint8_t, std::vector<bool>> data;
        std::vector<bool> code = {};
        dfs(root, code, data);
        return data;
    }

    bool Tree::move(bool flag) {
        if (current == nullptr)
            return false;
        if (flag)
            current = current->right;
        else
            current = current->left;
        return !((current->left == nullptr) && (current->right == nullptr));
    }

    void Tree::moveToRoot() {
        current = root;
    }

    uint8_t Tree::getChar() {
        if (current == nullptr)
            throw std::logic_error("Huffman tree error");
        return current->ch;
    }

    void Tree::dfs(const Tree::Node *start, std::vector<bool> &code, std::map<uint8_t, std::vector<bool> > &data) {
        if (start == nullptr)
            return;
        if (start->left == nullptr && start->right == nullptr)
            data[start->ch] = code;
        code.push_back(false);
        dfs(start->left, code, data);
        code.pop_back();
        code.push_back(true);
        dfs(start->right, code, data);
        code.pop_back();
    }
}