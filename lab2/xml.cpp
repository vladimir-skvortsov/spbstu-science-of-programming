#include "xml.h"

XML::Node::Node(const std::string& tag, const std::string& value) : tag(tag), value(value) {};
void XML::Node::add(std::unique_ptr<XML::Node> child) {
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
void XML::Node::for_each(std::function<void(const Node*)> callback) {
  callback(this);
  for (const auto& child : children) {
    child->for_each(callback);
  };
};
std::vector<XML::Node*> XML::Node::get_descendants() {
  std::vector<Node*> descendants = {};

  for (const auto& child : children) {
    descendants.push_back(child.get());
    std::vector<Node*> child_descendants = child->get_descendants();
    descendants.insert(descendants.end(), child_descendants.begin(), child_descendants.end());
  }

  return descendants;
};
std::unique_ptr<XML::Node> XML::Node::clone() const {
  std::unique_ptr<XML::Node> copy = std::make_unique<Node>(tag, value);

  for (const auto& child : children) {
    copy->children.push_back(child->clone());
  }

  return copy;
};
XML::Node::iterator XML::Node::find(std::function<bool (Node* node)> callback) {
  if (callback(this)) {
    return XML::Node::iterator(this);
  }
  for (const auto& child : children) {
    XML::Node::iterator node = child->find(callback);
    if (*node != nullptr) {
      return node;
    }
  }
  return XML::Node::iterator(nullptr);
};

XML::Node::iterator::iterator(Node* node) {
  if (node) {
    nodes = node->get_descendants();
    nodes.insert(nodes.begin(), node);
  }
  cur_node_index = 0;
};
XML::Node::iterator& XML::Node::iterator::operator ++ () {
  *this += 1;
  return *this;
};
XML::Node::iterator XML::Node::iterator::operator ++ (int) {
  iterator temp(*this);
  ++(*this);
  return temp;
};
XML::Node::iterator& XML::Node::iterator::operator += (int n) {
  cur_node_index += n;
  if (cur_node_index > nodes.size()) {
    cur_node_index = nodes.size();
  }
  return *this;
};
bool XML::Node::iterator::operator == (const iterator& other) const {
  return **this == *other;
};
bool XML::Node::iterator::operator != (const iterator& other) const {
  return !(*this == other);
};
XML::Node* XML::Node::iterator::operator * () const {
  if (nodes.empty() || cur_node_index == nodes.size()) {
    return nullptr;
  }
  return nodes[cur_node_index];
};
XML::Node* XML::Node::iterator::operator -> () const {
  return **this;
};
XML::Node::iterator XML::Node::begin() {
  return XML::Node::iterator(this);
};
XML::Node::iterator XML::Node::end() {
  return XML::Node::iterator(nullptr);
};

XML::Document::Document() : root_node(nullptr) {};
XML::Document::Document(const std::string& path) : root_node(nullptr) {
  load(path);
};
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
void XML::Document::for_each(std::function<void(const Node*)> callback) {
  if (!root_node)
    return;

  // Create a backup for strong exception guarantee
  std::unique_ptr<Node> backup = root_node->clone();

  try {
    root_node->for_each(callback);
  } catch (...) {
    root_node = std::move(backup);
    throw;
  }
};
std::unique_ptr<XML::Node> XML::Document::parse_node(const std::string& xml, int& pos) {
  std::string tag = get_next_tag(xml, pos);
  std::string value = get_next_value(xml, pos);
  std::unique_ptr<XML::Node> node(new XML::Node(tag, value));

  std::string next_tag = get_next_tag(xml, pos);
  while (next_tag != ("/" + tag) && pos < xml.size()) {
    pos -= next_tag.size() + 2;
    if (pos < 0) {
      throw std::runtime_error("Invalid XML");
    }
    node->add(parse_node(xml, pos));
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
  auto front = std::find_if_not(str.begin(), str.end(), [](int c){
    return std::isspace(c);
  });
  auto back = std::find_if_not(str.rbegin(), str.rend(), [](int c){
    return std::isspace(c);
  }).base();
  return (back <= front ? std::string() : std::string(front, back));
};
XML::Node::iterator XML::Document::begin() {
  return root_node->begin();
};
XML::Node::iterator XML::Document::end() {
  return root_node->end();
};
XML::Node::iterator XML::Document::find(std::function<bool (Node* node)> callback) {
  return root_node->find(callback);
};
XML::Node::iterator XML::Document::find_by_tag(const std::string& tag) {
  return root_node->find([&tag] (Node* node) {
    return node->tag == tag;
  });
};
XML::Node::iterator XML::Document::find_by_value(const std::string& value) {
  return root_node->find([&value] (Node* node) {
    return node->value == value;
  });
};
XML::Node::iterator XML::Document::add(XML::Node::iterator it, Node* node) {
  if (!*it) {
    throw std::runtime_error("Node does not exist");
  }
  it->add(std::unique_ptr<Node>(node));
  return XML::Node::iterator(node);
};
bool XML::Document::erase(Node::iterator it) {
  if (*it == root_node.get()) {
    return false;
  }

  auto parent_it = find([&] (Node* node) {
    auto vec_it = std::find_if(
      node->children.begin(),
      node->children.end(),
      [&](const std::unique_ptr<Node> &ptr) {
        return ptr.get() == *it;
      }
    );

    return vec_it != node->children.end();
  });

  if (parent_it == end()) {
    return false;
  }

  std::move(
    it->children.begin(),
    it->children.end(),
    std::back_inserter(parent_it->children)
  );
  auto vec_it = std::find_if(
    parent_it->children.begin(),
    parent_it->children.end(),
    [&](const std::unique_ptr<Node> &ptr) {
      return ptr.get() == *it;
    }
  );
  parent_it->children.erase(vec_it);

  return true;
};
