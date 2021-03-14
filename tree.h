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
        char GetHeight() const { return height_;};
        Node* GetChild(Dir dir) const { return dir == Dir::Left ? left_ : right_;};
        //.....................................................................

        void SetHeight(char height) { height_ = height;};
        void SetChild(Node* child, Dir dir) { if (dir == Dir::Left) left_ = child;
                                              else                  right_ = child;};

    private:

        Key_t key_;
        char height_;
        Node* left_;
        Node* right_;

    };
    //...........................................................................

    template <typename Key_t>
    class Tree final {

    public:

        Tree() = default;

        Tree(const Tree& other) {

            Tree<Key_t> temp_tree;

            for (int i = 0; i < other.nodes_.size(); ++i)
                temp_tree.Insert(other.nodes_[i]->GetKey());

            std::swap(*this, temp_tree);
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

        char BalanceFactor(Node<Key_t>* node) const;
        Node<Key_t>* Rotate(Node<Key_t>* node, Dir dir);
        Node<Key_t>* Balance(Node<Key_t>* node);
        void UpdateHeight(Node<Key_t>* node);

        void my_swap (Tree& rhs) noexcept {
            std::swap (head_, rhs.head_);
            std::swap (nodes_, rhs.nodes_);
        }

    public:

        //...................Tree_Iterator......................

        class My_Iterator final {

        public:

            My_Iterator() = default;

            using iterator_category = std::input_iterator_tag;
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

            bool operator==(const My_Iterator& rhs) const { return node_ == rhs.node_;};
            bool operator!=(const My_Iterator& rhs) const { return !(*this == rhs);};

            My_Iterator operator++();
            My_Iterator operator++(int);

        private:

            explicit My_Iterator(const Tree<Key_t>& outer_tree, Node<Key_t>* node, std::stack<Node<Key_t>*>&& trace)
                : tree_(outer_tree), node_(node) { std::swap(trace, trace_);};

            const Tree<Key_t>& tree_ = nullptr;
            Node<Key_t>* node_ = nullptr;

            std::stack<Node<Key_t>*> trace_;

            friend Tree;
        };

        //.......................................................

        My_Iterator LowerBound(const Key_t& key) const;
        My_Iterator UpperBound(const Key_t& key) const;
        My_Iterator begin() const;
        My_Iterator end() const;

        My_Iterator Begin(Node<Key_t>* node) const;

    };

    template <typename Key_t>
    char Tree<Key_t>::BalanceFactor(Node<Key_t>* node) const {
        if (!node)
            return 0;

        char r = node->GetChild(Dir::Right) ? node->GetChild(Dir::Right)->GetHeight() : 0;
        char l = node->GetChild(Dir::Left) ? node->GetChild(Dir::Left)->GetHeight() : 0;

        return l - r;
    }

    template<typename Key_t>
    void Tree<Key_t>::UpdateHeight(Node<Key_t>* node)  {

        if (!node)
            return;

        char l = node->GetChild(Dir::Left) ? node->GetChild(Dir::Left)->GetHeight() : 0;
        char r = node->GetChild(Dir::Right) ? node->GetChild(Dir::Right)->GetHeight() : 0;

        node->SetHeight(1 + std::max(l, r));
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

        if(BalanceFactor(node) == -2) {
            if((node->GetChild(Dir::Right) != nullptr) && (BalanceFactor(node->GetChild(Dir::Right)) > 0)) {
                auto n = Rotate(node->GetChild(Dir::Right), Dir::Right);
                node->SetChild(n, Dir::Right);
            }
            return Rotate(node, Dir::Left);
        }
        if(BalanceFactor(node) == 2) {
            if((node->GetChild(Dir::Left) != nullptr) && (BalanceFactor(node->GetChild(Dir::Left)) < 0)) {
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
            nodes_.push_back(head_);
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

        for (auto* elem : nodes_)
            delete elem;
    }

    template<typename Key_t>
    typename Tree<Key_t>::My_Iterator Tree<Key_t>::LowerBound(const Key_t &key) const {

        auto current_node = head_;
        decltype(current_node) previous_node = nullptr;
        std::stack<Node<Key_t>*> trace;

        while(current_node) {

            auto cur_key = current_node->GetKey();
            if(cur_key < key) {
                previous_node = current_node;
                current_node = current_node->GetChild(Dir::Right);
            }
            else if(cur_key > key) {
                previous_node = current_node;
                current_node = current_node->GetChild(Dir::Left);
                if (current_node)
                    trace.push(previous_node);
                else
                    break;
            }
            else
                return My_Iterator(*this, current_node, std::move(trace));
        }

        My_Iterator ret(*this, previous_node, std::move(trace));

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

        if(!head_) {
            std::stack<Node<Key_t>*> empty;
            return My_Iterator(*this, nullptr, std::move(empty));
        }
        std::stack<Node<Key_t>*> trace;

        auto current_node = head_;

        while(current_node->GetChild(Dir::Left)) {
            trace.push(current_node);
            current_node = current_node->GetChild(Dir::Left);
        }

        return My_Iterator(*this, current_node, std::move(trace));
    }

    template<typename Key_t>
    typename Tree<Key_t>::My_Iterator Tree<Key_t>::Begin(Node<Key_t>* node) const {

        if(!node) {
            std::stack<Node<Key_t>*> empty;
            return My_Iterator(*this, nullptr, std::move(empty));
        }

        std::stack<Node<Key_t>*> trace;

        auto current_node = node;

        while(current_node->GetChild(Dir::Left)) {
            trace.push(current_node);
            current_node = current_node->GetChild(Dir::Left);
        }

        return My_Iterator(*this, current_node, std::move(trace));
    }

    template<typename Key_t>
    typename Tree<Key_t>::My_Iterator Tree<Key_t>::end() const {
        std::stack<Node<Key_t>*> empty;
        return My_Iterator(*this, nullptr, std::move(empty));
    }


    template<typename Key_t>
    typename Tree<Key_t>::My_Iterator Tree<Key_t>::My_Iterator::operator++() {

       if (!node_->GetChild(Dir::Right)) {

           if (!trace_.size())
               node_ = nullptr;
           else {
               node_ = trace_.top();
               trace_.pop();
           }

           auto temp(trace_);
           return My_Iterator(tree_, node_, std::move(temp));
       }
       else {
           auto it = tree_.Begin(node_->GetChild(Dir::Right));
           node_ = it.node_;
           std::vector<Node<Key_t>*> trace;

           while (!it.trace_.empty()) {
               trace.push_back(it.trace_.top());
               it.trace_.pop();
           }
           for (auto iter = trace.rbegin(); iter != trace.rend(); ++iter)
               trace_.push(*iter);

           auto temp(trace_);
           return My_Iterator(tree_, node_, std::move(temp));
       }

    }

    template<typename Key_t>
    typename Tree<Key_t>::My_Iterator Tree<Key_t>::My_Iterator::operator++(int) {
        auto tmp = *this;
        ++(*this);
        return tmp;
    }

}