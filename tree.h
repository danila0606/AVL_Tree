#pragma once
#include <iostream>
#include <vector>
#include <stack>

namespace tr {

    enum class Dir {
        Right,
        Left
    };

    //................................Node..................................
    template <typename Key_t>
    class Node final {

    public:
        Node(const Key_t& key, Node* left = nullptr, Node* right = nullptr) :
                key_(key), height_(1), left_(left), right_(right) {};


        Node(const Node& other) = delete;
        Node& operator=(const Node& other) = delete;

        Node(Node&& other) = delete;
        Node& operator=(Node&& other) = delete;

        //..........................const_methods.............................
        Key_t GetKey() const { return key_;};
        unsigned char GetHeight() const { return height_;};
        Node* GetChild(Dir dir) const { return dir == Dir::Left ? left_ : right_;};
        //.....................................................................

        void SetHeight(unsigned char height) { height_ = height;};
        void SetChild(Node* child, Dir dir) { if (dir == Dir::Left) left_ = child;
                                              else                  right_ = child;};

    private:

        Key_t key_;
        unsigned char height_;
        Node* left_;
        Node* right_;

    };
    //...........................................................................

    template <typename Key_t>
    class Tree final {

    public:
        class Iterator;

        Tree() = default;

        Tree(const Tree& other) {
            Key_t head_key = other.head_->GetKey();
            head_ = new Node<Key_t>(head_key);

            for (int i = 0; i < other.nodes_.size(); ++i) {
                auto* copy_node = new Node<Key_t> (other.nodes_[i]->GetKey());
                Insert(copy_node->GetKey());
            }
        }
        Tree& operator=(const Tree& other) {
            if (this != &other) {
                auto temp(other);
                my_swap(temp);
            }
            return *this;
        }

        Tree(Tree&& other) noexcept {
            my_swap(other);
        }
        Tree& operator=(Tree&& other) noexcept {
            my_swap(other);
        }


        void Insert(const Key_t& key);

        ~Tree();

    private:

        Node<Key_t>* head_ = nullptr;
        std::vector<Node<Key_t>*> nodes_;

    private:

        unsigned char BalanceFactor(Node<Key_t>* node) const;
        Node<Key_t>* Rotate(Node<Key_t>* node, Dir dir);
        Node<Key_t>* Balance(Node<Key_t>* node);
        void UpdateHeight(Node<Key_t>* node);

        void my_swap (Tree& rhs) noexcept {
            std::swap (this->head_, rhs.head_);
            std::swap (this->nodes_, rhs.nodes_);
        }

    public:

        //...................Tree_Iterator......................

        class My_Iterator final {

        public:

            My_Iterator() = default;

            using iterator_category = std::bidirectional_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = Key_t;
            using reference = Key_t&;
            using pointer = Node<Key_t>*;


            My_Iterator(const My_Iterator& other) = default;
            My_Iterator& operator=(const My_Iterator& other) = default;

            My_Iterator(My_Iterator&& other) noexcept = default;
            My_Iterator& operator=(My_Iterator&& other) noexcept = default;


            value_type operator*() const { return node_->GetKey();};
            pointer operator->() const { return node_;};

            bool operator==(const My_Iterator& rhs) const { return this->node_ == rhs.node_;};
            bool operator!=(const My_Iterator& rhs) const { return !(*this == rhs);};

            My_Iterator operator++();
            My_Iterator operator--();

        private:

            explicit My_Iterator(const Tree<Key_t>& outer_tree, Node<Key_t>* node) : tree_{outer_tree}, node_{node} {};

            const Tree<Key_t>& tree_ = nullptr;
            Node<Key_t>* node_ = nullptr;

            friend Tree;
        };

        //.......................................................

        My_Iterator LowerBound(const Key_t& key) const;
        My_Iterator UpperBound(const Key_t& key) const;
        My_Iterator begin() const;
        My_Iterator end() const;

    };

    template <typename Key_t>
    unsigned char Tree<Key_t>::BalanceFactor(Node<Key_t>* node) const {
        if (!node)
            return 0;

        unsigned char r = node->GetChild(Dir::Right) ? node->GetChild(Dir::Right)->GetHeight() : 0;
        unsigned char l = node->GetChild(Dir::Left) ? node->GetChild(Dir::Left)->GetHeight() : 0;

        return r - l;
    }

    template<typename Key_t>
    void Tree<Key_t>::UpdateHeight(Node<Key_t>* node)  {

        if (!node)
            return;

        unsigned char l = node->GetChild(Dir::Left) ? node->GetChild(Dir::Left)->GetHeight() : 0;
        unsigned char r = node->GetChild(Dir::Right) ? node->GetChild(Dir::Right)->GetHeight() : 0;

        node->SetHeight(std::max(l, r));
    }


    template<typename Key_t>
    Node<Key_t>* Tree<Key_t>::Rotate(Node<Key_t> *node, Dir dir) {

        Node<Key_t>* new_node;

        if(dir == Dir::Left) {
            new_node = node->GetChild(Dir::Right);
            node->SetChild(new_node->GetChild(Dir::Left), Dir::Right);
            new_node->SetChild(node, Dir::Left);
        }
        else if(dir == Dir::Right) {
            new_node = node->GetChild(Dir::Left);
            node->SetChild(new_node->GetChild(Dir::Right), Dir::Left);
            new_node->SetChild(node, Dir::Right);
        }

        UpdateHeight(node);
        UpdateHeight(new_node);

        return new_node;
    }

    template<typename Key_t>
    Node<Key_t>* Tree<Key_t>::Balance(Node<Key_t>* node) {

        if(BalanceFactor(node) == 2) {
            if((node->GetChild(Dir::Right) != nullptr) && (BalanceFactor(node->GetChild(Dir::Right)) < 0)) {
                auto n = Rotate(node->GetChild(Dir::Right), Dir::Right);
                node->SetChild(n, Dir::Right);
            }
            return Rotate(node, Dir::Left);
        }
        if(BalanceFactor(node) == -2) {
            if((node->GetChild(Dir::Left) != nullptr) && (BalanceFactor(node->GetChild(Dir::Left)) > 0)) {
                auto n = Rotate(node->GetChild(Dir::Left), Dir::Left);
                node->SetChild(n, Dir::Left);
            }
            return Rotate(node, Dir::Right);
        }
        else
            return node;
    }

    template<typename Key_t>
    void Tree<Key_t>::Insert(const Key_t& key) {

        if(!head_) {
            head_ = new Node(key);
            return;
        }

        auto current_node = head_;
        std::stack<Node<Key_t>*> trace;

        while(current_node) {

            trace.push(current_node);
            if(current_node->GetKey() < key)
                current_node = current_node->GetChild(Dir::Right);
            else if (current_node->GetKey() > key)
                current_node = current_node->GetChild(Dir::Left);
            else
                return;
        }

        current_node = trace.top();
        trace.pop();

        auto new_node = new Node(key);
        nodes_.push_back(new_node);

        if(current_node->GetKey() < key)
            current_node->SetChild(new_node, Dir::Right);
        else
            current_node->SetChild(new_node, Dir::Left);

        while(current_node) {

            UpdateHeight(current_node);
            if(!BalanceFactor(current_node))
                return; // already balanced

            auto balanced_node = Balance(current_node);
            if(!trace.size()) {
                head_ = balanced_node;
                return;
            }

            auto next_node = trace.top();
            trace.pop();

            if(next_node->GetChild(Dir::Left) == current_node)
                next_node->SetChild(balanced_node, Dir::Left);
            else
                next_node->SetChild(balanced_node, Dir::Right);

            current_node = next_node;
        }
    }

    template <typename Key_t>
    Tree<Key_t>::~Tree() {

        for (auto&& elem : nodes_)
            delete elem;
    }

    template<typename Key_t>
    typename Tree<Key_t>::My_Iterator Tree<Key_t>::LowerBound(const Key_t &key) const {

        auto current_node = head_;
        decltype(current_node) previous_node = nullptr;

        while(current_node) {

            auto cur_key = current_node->GetKey();
            if(cur_key < key) {
                previous_node = current_node;
                current_node = current_node->GetChild(Dir::Right);
            }
            else if(cur_key > key) {
                previous_node = current_node;
                current_node = current_node->GetChild(Dir::Left);
            }
            else
                return My_Iterator(*this, current_node);
        }

        My_Iterator ret(*this, previous_node);

        if(previous_node->GetKey() > key)
            return ret;

        return ++ret;
    }

    template<typename Key_t>
    typename Tree<Key_t>::My_Iterator Tree<Key_t>::UpperBound(const Key_t &key) const {

        auto ans = LowerBound(key);
        if(ans == end())
            return end();
        else if(*ans == key)
            return ++ans;

        return ans;
    }

    template<typename Key_t>
    typename Tree<Key_t>::My_Iterator Tree<Key_t>::begin() const {

        if(!head_)
            return My_Iterator(*this, nullptr);

        auto current_node = head_;
        while(current_node->GetChild(Dir::Left))
            current_node = current_node->GetChild(Dir::Left);

        return My_Iterator(*this, current_node);
    }

    template<typename Key_t>
    typename Tree<Key_t>::My_Iterator Tree<Key_t>::end() const {
        return My_Iterator(*this, nullptr);
    }


    template<typename Key_t>
    typename Tree<Key_t>::My_Iterator Tree<Key_t>::My_Iterator::operator++() {

        auto current_node = tree_.head_;
        decltype(current_node) ans = nullptr;

        auto key = node_->GetKey();

        while(current_node) {
            auto cur_key = current_node->GetKey();
            if(cur_key > key) {
                ans = current_node;
                current_node = current_node->GetChild(Dir::Left);
            }
            else
                current_node = current_node->GetChild(Dir::Right);
        }

        node_ = ans;
        return My_Iterator(tree_, node_);
    }

    template<typename Key_t>
    typename Tree<Key_t>::My_Iterator Tree<Key_t>::My_Iterator::operator--() {

        auto current_node = tree_.head_;
        decltype(current_node) ans = nullptr;

        auto key = node_->GetKey();

        while(current_node) {
            auto cur_key = current_node->GetKey();
            if(cur_key < key) {
                ans = current_node;
                current_node = current_node->GetChild(Dir::Right);
            }
            else
                current_node = current_node->GetChild(Dir::Left);
        }

        node_ = ans;
        return My_Iterator(tree_, node_);
    }

}