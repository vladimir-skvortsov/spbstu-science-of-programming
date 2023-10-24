#include "xml.h"

XML::Node::Node(const std::string& tag, const std::string& value) : tag(tag), value(value) {};
void XML::Node::append(std::unique_ptr<XML::Node> child) {
  children.push_back(std::move(child));
};
std::string XML::Node::stringify(const int depth = 0) {
  const std::string indent = std::string(depth * 2, ' ');
  std::string result = "";

  result += indent + "<" + tag + ">" + value;
  if (children.size()) {
    result += "\n";
  }
  for (const auto& child : children) {
    result += child->stringify(depth + 1);
  }
  if (children.size()) {
    result += indent;
  }
  result += "</" + tag + ">" + "\n";

  return result;
};
void XML::Node::for_each(std::function<void(const Node&)> callback) {
  callback(*this);
  for (const auto& child : children) {
    child->for_each(callback);
  };
};

XML::Document::Document() : root_node(nullptr){};
void XML::Document::parse(const std::string& xml) {
  int pos = 0;
  root_node = parse_node(xml, pos);
};
void XML::Document::load(const std::string& path) {
  const std::string xml = read_file(path);
  parse(xml);
};
void XML::Document::save(const std::string& path) {
  std::string xml = stringify();
  write_file(path, xml);
};
void XML::Document::print() {
  std::string xml = stringify();
  std::cout << xml << std::endl;
};
void XML::Document::for_each(std::function<void(const Node&)> callback) {
  root_node->for_each(callback);
};
std::unique_ptr<XML::Node> XML::Document::parse_node(const std::string& xml, int& pos) {
  std::string tag = get_next_tag(xml, pos);
  std::string value = get_next_value(xml, pos);
  std::unique_ptr<XML::Node> node(new XML::Node(tag, value));

  std::string next_tag = get_next_tag(xml, pos);
  while (next_tag != ("/" + tag) && pos < xml.size()) {
    pos -= next_tag.size() + 2;
    node->append(parse_node(xml, pos));
    next_tag = get_next_tag(xml, pos);
  }

  return node;
};
std::string XML::Document::get_next_tag(const std::string& xml, int& pos) {
  pos = xml.find("<", pos) + 1;
  int end = xml.find(">", pos);
  std::string tag = xml.substr(pos, end - pos);
  pos = end + 1;
  return tag;
};
std::string XML::Document::get_next_value(const std::string&xml, int& pos) {
  int start = pos;
  pos = xml.find("<", pos);
  return trim(xml.substr(start, pos - start));
};
std::string XML::Document::read_file(const std::string& path) {
  std::ifstream file(path);

  if (!file) {
    throw std::runtime_error("File not found");
  }

  return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
};
void XML::Document::write_file(const std::string& path, const std::string& content) {
  std::ofstream file(path);

  if (!file) {
    throw std::runtime_error("File not found");
  }

  file << content;
};
std::string XML::Document::stringify() {
  return root_node->stringify();
};
std::string XML::Document::trim(const std::string& str) {
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
