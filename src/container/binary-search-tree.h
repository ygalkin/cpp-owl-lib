#ifndef _CONTAINER_BINARY_SEARCH_TREE_
#define _CONTAINER_BINARY_SEARCH_TREE_

#include <functional>

namespace container {

    enum class order_type { pre_order, in_order, sort_order = in_order, post_order };

    template< class KeyT >
    class binary_search_tree {
    protected:
        template< class NodeKeyT >
        struct tree_node {
            tree_node() = delete;
            tree_node(const NodeKeyT& key) : _left(nullptr), _right(nullptr), _key(key) {};
            tree_node(const tree_node& other) = delete;
            tree_node(tree_node&& other) = delete;
            tree_node& operator = (const tree_node& other) = delete;
            tree_node& operator = (tree_node&& other) = delete;
            ~tree_node() = default;

            tree_node<NodeKeyT>* _left;
            tree_node<NodeKeyT>* _right;
            NodeKeyT _key;
        };

    private:
        tree_node<KeyT>* _root{ nullptr };

        void _insert(tree_node<KeyT>** parent, const KeyT& key) {
            if (*parent == nullptr) {
                *parent = new tree_node<KeyT>(key);
                return;
            }

            _insert((key < (*parent)->_key) ? &(*parent)->_left : &(*parent)->_right, key);
        }

        tree_node<KeyT>* _find(tree_node<KeyT>* parent, const KeyT& key) const {
            if (parent == nullptr || parent->_key == key) {
                return parent; // nullptr or parent
            }
            return _find(key < parent->_key ? parent->_left : parent->_right, key);
        }

        template <order_type Order>
        void _for_each(tree_node<KeyT>* parent, std::function<void(const KeyT&)> f) const {
            if (parent == nullptr) {
                return;
            }

            if (Order == order_type::pre_order) f(parent->_key);
            _for_each<Order>(parent->_left, f);
            if (Order == order_type::in_order) f(parent->_key);
            _for_each<Order>(parent->_right, f);
            if (Order == order_type::post_order) f(parent->_key);
        }

        void _clear_post_order(tree_node<KeyT>* parent) {
            if (parent == nullptr) {
                return;
            }

            _clear_post_order(parent->_left);
            _clear_post_order(parent->_right);
            delete parent;
        }

        bool _is_bst(const tree_node<KeyT>* node, const tree_node<KeyT>* min, const tree_node<KeyT>* max) const {
            if (node == nullptr) {
                return true;
            }

            if ((min && node->_key < min->_key) || (max && node->_key > max->_key)) {
                return false;
            }

            return _is_bst(node->_left, min, node) && _is_bst(node->_right, node, max);
        }

    public:
        binary_search_tree() = default;
        binary_search_tree(const binary_search_tree& other) = default;
        binary_search_tree(binary_search_tree&& other) = default;
        binary_search_tree& operator = (const binary_search_tree& other) = default;
        binary_search_tree& operator = (binary_search_tree&& other) = default;
        virtual ~binary_search_tree() { clear(); }

        template <order_type Order>
        void for_each(std::function<void(const KeyT&)> f) const {
            _for_each<Order>(_root, f);
        }

        void clear() {
            _clear_post_order(_root);
            _root = nullptr;
        }

        void insert(const KeyT& key) {
            _insert(&_root, key);
        }

        bool find(const KeyT& key) const {
            return (_find(_root, key) != nullptr);
        }

        bool empty() const {
            return (_root == nullptr);
        }

        // TODO:
        // erase(key)
        // size_type size

        bool is_bst() const {
            return _is_bst(_root, nullptr, nullptr);
        }
    };
}

#endif