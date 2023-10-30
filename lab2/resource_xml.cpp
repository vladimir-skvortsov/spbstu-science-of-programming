#include "resource_xml.h"

std::unique_ptr<Resource_XML> Resource_XML::create(const std::string& path){
  return std::unique_ptr<Resource_XML>(new Resource_XML(path));
};
Resource_XML::Resource_XML(const std::string& path) {
  document = std::make_unique<XML::Document>(path);
};
XML::Node::iterator Resource_XML::begin() {
  return document->begin();
};
XML::Node::iterator Resource_XML::end() {
  return document->end();
};
XML::Node::iterator Resource_XML::find(std::function<bool(XML::Node *node)> callback) {
  return document->find(callback);
};
XML::Node::iterator Resource_XML::find_by_tag(const std::string& tag) {
  return document->find_by_tag(tag);
};
XML::Node::iterator Resource_XML::find_by_value(const std::string& tag) {
  return document->find_by_value(tag);
};
XML::Node::iterator Resource_XML::add(XML::Node::iterator it, XML::Node* node) {
  return document->add(it, node);
};
bool Resource_XML::erase(XML::Node::iterator it) {
  return document->erase(it);
};
