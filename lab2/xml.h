#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <fstream>
#include <algorithm>

namespace XML {class Node {
    public:
      std::string tag;
      std::string value;
      std::vector<std::unique_ptr<Node>> children;

      Node(const std::string& tag, const std::string& value);

      void append(std::unique_ptr<Node> child);
      std::string stringify(const int depth);
      void for_each(std::function<void(const Node&)> callback);
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
}

