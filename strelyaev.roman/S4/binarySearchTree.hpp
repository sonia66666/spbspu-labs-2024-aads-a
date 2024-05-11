#ifndef BINARYSEARCHTREE_HPP
#define BINARYSEARCHTREE_HPP
#include <functional>
#include "node.hpp"
#include "treeIterator.hpp"
#include "constTreeIterator.hpp"


#include <iostream>

namespace strelyaev
{
  template< typename Key, typename T, typename Compare = std::less< Key > >
  class Tree
  {
    using node_t = detail::Node< Key, T >;
    using iterator_t = Iterator< Key, T >;
    using c_iterator_t = ConstIterator< Key, T >;
    using tree_t = Tree< Key, T, Compare >;
    public:
      Tree():
        root_(nullptr),
        cmp_(Compare()),
        size_(0)
      {}

      Tree(const tree_t& other):
        Tree()
      {
        for (auto it = other.cbegin(); it != other.cend(); it++)
        {
          insert(*it);
        }
      }

      Tree(tree_t&& other)
      {}

      ~Tree()
      {
        clear(root_);
      }

      void swap(tree_t& other)
      {
        std::swap(other.root_, root_);
        std::swap(other.size_, size_);
      }

      void insert(const std::pair< Key, T >& pair)
      {
        insert(pair.first, pair.second);
      }

      void insert(const Key& key, const T& value)
      {
        if (!root_)
        {
          root_ = new node_t(nullptr, nullptr, nullptr, key, value);
          size_++;
          return;
        }
        node_t* current = root_;
        node_t* parent = nullptr;
        while (current)
        {
          parent = current;
          if (cmp_(key, current->data_.first))
          {
            current = current->left_;
          }
          else
          {
            current = current->right_;
          }
        }
        if (cmp_(key, parent->data_.first))
        {
          parent->left_ = new node_t(nullptr, parent, nullptr, key, value);
        }
        else
        {
          parent->right_ = new node_t(nullptr, parent, nullptr, key, value);
        }
        size_++;
        while (parent)
        {
          updateHeight(parent);
          balance(parent);
          parent = parent->parent_;
        }
      }

      iterator_t find(const Key& key)
      {
        node_t* current = root_;
        while (current)
        {
          if (current->data_.first == key)
          {
            return iterator_t(current);
          }
          else if (cmp_(current->data_.first, key))
          {
            current = current->right_;
          }
          else
          {
            current = current->left_;
          }
        }
        return iterator_t(current);
      }

      iterator_t begin() noexcept
      {
        node_t* current = root_;
        while (current->left_)
        {
          current = current->left_;
        }
        return iterator_t(current);
      }

      iterator_t end() noexcept
      {
        return iterator_t(nullptr);
      }

      c_iterator_t cbegin() const noexcept
      {
        node_t* current = root_;
        while (current->left_)
        {
          current = current->left_;
        }
        return c_iterator_t(current);
      }

      c_iterator_t cend() const noexcept
      {
        return c_iterator_t(nullptr);
      }

      T& at(const Key& key)
      {
        auto it_find = find(key);
        if (it_find.node_ == nullptr)
        {
          throw std::out_of_range("Out of range");
        }
        return it_find->second;
      }

      T& operator[](const Key& key) noexcept
      {
        auto it_find = find(key);
        if (it_find.node_ == nullptr)
        {
          insert(key, T());
          it_find = find(key);
        }
        return it_find->second;
      }

      bool empty() noexcept
      {
        return size_ == 0;
      }

      size_t size() noexcept
      {
        return size_;
      }

      void clear(node_t* node) noexcept
      {
        if (node)
        {
          clear(node->right_);
          clear (node->left_);
          delete node;
        }
        root_ = nullptr;
      }

    private:
      node_t* root_;
      Compare cmp_;
      size_t size_;

      void node_swap(node_t* lhs, node_t* rhs)
      {
        std::pair< Key, T > buffer = lhs->data_;
        lhs->data_ = rhs->data_;
        rhs->data_ = buffer;
      }

      int getHeight(node_t* node)
      {
        if (!node)
        {
          return -1;
        }
        return node->height_;
      }

      int getBalance(node_t* node)
      {
        if (!node)
        {
          return 0;
        }
        return getHeight(node->right_) - getHeight(node->left_);
      }

      void updateHeight(node_t* node)
      {
        node->height_ = std::max(getHeight(node->left_), getHeight(node->right_)) + 1;
      }

      void rotateRight(node_t* node)
      {
        node_swap(node->left_, node);
        node_t* buffer = node->right_;
        node->right_ = node->left_;

        node->left_ = node->right_->left_;
        if (node->left_)
        {
          node->left_->parent_ = node;
        }

        node->right_->left_ = node->right_->right_;

        node->right_->right_ = buffer;
        if (buffer)
        {
          buffer->parent_ = node->right_;
        }

        updateHeight(node->right_);
        updateHeight(node);
      }

      void rotateLeft(node_t* node)
      {
        node_swap(node->right_, node);
        node_t* buffer = node->left_;
        node->left_ = node->right_;

        node->right_ = node->left_->right_;
        if (node->right_)
        {
          node->right_->parent_ = node;
        }

        node->left_->right_ = node->left_->left_;

        node->left_->left_ = buffer;
        if (buffer)
        {
          buffer->parent_ = node->left_;
        }

        updateHeight(node->left_);
        updateHeight(node);
      }

      void balance(node_t* node)
      {
        int balance = getBalance(node);
        if (balance == -2)
        {
          if (getBalance(node->left_) == 1)
          {
            rotateLeft(node->left_);
          }
          rotateRight(node);
        }
        else if (balance == 2)
        {
          if (getBalance(node->right_) == -1)
          {
            rotateRight(node->right_);
          }
          rotateLeft(node);
        }
      }
  };
}

#endif

