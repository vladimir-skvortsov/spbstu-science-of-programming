#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <fstream>
#include <algorithm>
#include "xml.h"
#include "resource_xml.h"

int main() {
  XML::Document document = XML::Document();

  document.load("./example.xml");
  document.print();

  for (XML::Document::iterator it = document.begin(); it != document.end(); ++it) {
    std::cout << "tag: " << it->tag << ", value: " << it->value << std::endl;
  }

  return 0;
};
