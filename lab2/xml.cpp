#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <fstream>
#include <algorithm>

class XML_node {
  public:
    std::string tag;
    std::string value;
    std::vector<std::unique_ptr<XML_node>> children;

    XML_node(const std::string &tag, const std::string &value) : tag(tag), value(value) {}

    void append(std::unique_ptr<XML_node> child) {
      children.push_back(std::move(child));
    }
    std::string stringify(const int depth = 0) {
      const std::string indent = std::string(depth * 2, ' ');
      std::string result = "";

      result += indent + "<" + tag + ">" + value;
      if (children.size()) {
        result += "\n";
      }
      for (const auto &child : children) {
        result += child->stringify(depth + 1);
      }
      if (children.size()) {
        result += indent;
      }
      result += "</" + tag + ">" + "\n";

      return result;
    };
    void for_each(std::function<void(const XML_node&)> callback){
      callback(*this);
      for (const auto &child : children) {
        child->for_each(callback);
      };
    };
};

class XML_document {
  public:
    XML_document(): root_node(nullptr) {};

    void parse(const std::string& xml) {
      int pos = 0;
      root_node = parse_node(xml, pos);
    };

    void load(const std::string& path) {
      const std::string xml = read_file(path);
      parse(xml);
    };
    void save(const std::string& path) {
      std::string xml = stringify();
      write_file(path, xml);
    };
    void print() {
      std::string xml = stringify();
      std::cout << xml << std::endl;
    };
    void for_each(std::function<void(const XML_node&)> callback){
      root_node->for_each(callback);
    };

  private:
    std::unique_ptr<XML_node> root_node;

    std::unique_ptr<XML_node> parse_node(const std::string &xml, int &pos) {
      std::string tag = get_next_tag(xml, pos);
      std::string value = get_next_value(xml, pos);
      std::unique_ptr<XML_node> node(new XML_node(tag, value));

      std::string next_tag = get_next_tag(xml, pos);
      while (next_tag != ("/" + tag) && pos < xml.size()) {
        pos -= next_tag.size() + 2;
        node->append(parse_node(xml, pos));
        next_tag = get_next_tag(xml, pos);
      }

      return node;
    };
    std::string get_next_tag(const std::string &xml, int &pos) {
      pos = xml.find("<", pos) + 1;
      int end = xml.find(">", pos);
      std::string tag = xml.substr(pos, end - pos);
      pos = end + 1;
      return tag;
    };
    std::string get_next_value(const std::string &xml, int &pos) {
      int start = pos;
      pos = xml.find("<", pos);
      return trim(xml.substr(start, pos - start));
    };
    std::string read_file(const std::string& path) {
      std::ifstream file(path);

      if (!file) {
        throw std::runtime_error("File not found");
      }

      return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    };
    void write_file(const std::string& path, const std::string& content) {
      std::ofstream file(path);

      if (!file) {
        throw std::runtime_error("File not found");
      }

      file << content;
    };
    std::string stringify() {
      return root_node->stringify();
    };
    std::string trim(const std::string& str) {
      std::string result = "";
      int start_pos = 0;
      int end_pos = str.length() - 1;


      for (; start_pos < str.length(); start_pos += 1) {
        char ch = str[start_pos];

        if (isspace(ch)) {
          continue;
        } else {
          break;
        }
      };
      for (; end_pos >= 0; end_pos -= 1) {
        char ch = str[end_pos];

        if (isspace(ch)) {
          continue;
        } else {
          break;
        }
      };

      return str.substr(start_pos, end_pos - start_pos + 1);
    };
};
