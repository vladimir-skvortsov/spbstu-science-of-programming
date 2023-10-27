#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <cmath>
#include <fstream>
#include <algorithm>
#include <memory>

namespace XML {
  class Node {
    public:
      std::string tag;
      std::string value;
      std::vector<std::unique_ptr<Node>> children;

      Node(const std::string& tag, const std::string& value);

      void add(std::unique_ptr<Node> child);
      std::string stringify(const int depth);
      void for_each(std::function<void(const Node&)> callback);
      std::vector<Node*> get_descendants();

      class iterator {
        public:
          using iterator_category = std::forward_iterator_tag;
          using value_type = Node;
          using difference_type = std::ptrdiff_t;
          using pointer = Node*;
          using reference = Node&;

          explicit iterator(Node* node);

          iterator& operator ++ ();
          iterator operator ++ (int);
          iterator& operator += (int n);
          bool operator == (const iterator& other) const;
          bool operator != (const iterator& other) const;
          Node* operator * () const;
          Node* operator -> () const;

        private:
          std::vector<Node*> nodes;
          std::vector<Node*>::iterator it;
      };

      iterator begin();
      iterator end();
      iterator find(std::function<bool (Node* node)> callback);
      iterator find_by_tag(const std::string &tag);
      iterator find_by_value(const std::string &value);
  };

  class Document {
    public:
      Document();
      Document(const std::string& path);

      void parse(const std::string& xml);

      void load(const std::string& path);
      void save(const std::string& path);
      void print();
      void for_each(std::function<void(const Node&)> callback);

      using iterator = Node::iterator;
      Node::iterator begin();
      Node::iterator end();
      Node::iterator find(std::function<bool (Node* node)>);
      Node::iterator find_by_tag(const std::string &tag);
      Node::iterator find_by_value(const std::string &value);
      Node::iterator add(Node::iterator it, Node* node);
      bool erase(Node::iterator it);

    private:
      std::unique_ptr<Node> root_node;

      std::unique_ptr<Node> parse_node(const std::string &xml, int &pos);
      std::string get_next_tag(const std::string &xml, int &pos);
      std::string get_next_value(const std::string &xml, int &pos);
      std::string read_file(const std::string& path);
      void write_file(const std::string& path, const std::string& content);
      std::string stringify();
      std::string trim(const std::string& str);
  };
};

