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

  private:
    std::unique_ptr<XML::Document> document;

    Resource_XML(const std::string &path);
};
