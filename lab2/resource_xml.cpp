#include "resource_xml.h"

std::unique_ptr<Resource_XML> Resource_XML::create(){
  return std::unique_ptr<Resource_XML>(new Resource_XML());
};
std::unique_ptr<Resource_XML> Resource_XML::create(const std::string& path){
  std::unique_ptr<Resource_XML> instance = Resource_XML::create();
  instance->load(path);
  return instance;
};
Resource_XML::Resource_XML(): document(std::make_unique<XML::Document>()) {};
void Resource_XML::parse(const std::string &xml) {
  return document->parse(xml);
};
void Resource_XML::load(const std::string &path) {
  return document->load(path);
};
void Resource_XML::save(const std::string &path) {
  return document->save(path);
};
void Resource_XML::print() {
  return document->print();
};
void Resource_XML::for_each(std::function<void (const XML::Node*)> callback) {
  return document->for_each(callback);
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
