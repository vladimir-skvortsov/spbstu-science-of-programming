#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <fstream>
#include <algorithm>
#include "xml.h"

class Resource_XML {
  public:
    static std::unique_ptr<Resource_XML> create(const std::string& path);

    XML::Node::iterator begin();
    XML::Node::iterator end();
    XML::Node::iterator find(std::function<bool(XML::Node *node)> callback);
    XML::Node::iterator find_by_tag(const std::string &tag);
    XML::Node::iterator find_by_value(const std::string &value);
    XML::Node::iterator add(XML::Node::iterator it, XML::Node *node);
    bool erase(XML::Node::iterator it);

  private:
    std::unique_ptr<XML::Document> document;

    Resource_XML(const std::string &path);
};
