#pragma once

#include <vector>
#include <cstdint>
#include <map>

namespace tree {
    class Tree {
        class Node {
        public:
            explicit Node(uint8_t ch = '\0', uint32_t weight = 0, Node *left = nullptr, Node *right = nullptr);
            ~Node();

            uint32_t const weight;
            Node *const left;
            Node *const right;
            uint8_t const ch;
        };

        struct NodeCmp {
            bool operator()(Node const *first, Node const *second);
        };

    public:
        Tree(std::map<uint8_t, uint32_t> const &data);
        Tree(Tree const&) = delete;
        Tree(Tree&&) = delete;
        ~Tree();
        Tree &operator=(Tree const&) = delete;
        Tree &operator=(Tree&&) = delete;

        std::map<uint8_t, std::vector<bool>> getCodes();

        bool move(bool flag);
        void moveToRoot();
        uint8_t getChar();

    private:
        Node *root;
        Node *current;

        void init(std::map<uint8_t, uint32_t> const &data);
        void dfs(Node const *root, std::vector<bool> &code, std::map<uint8_t, std::vector<bool>> &data);
    };
}