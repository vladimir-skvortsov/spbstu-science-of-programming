#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <fstream>
#include <algorithm>
#include "xml.h"

int main() {
  XML::Document* document = new XML::Document();

  document->load("./example.xml");
  document->print();

  // std::function<void(const XML_node&)> callback = [](XML_node& node) {
  //   std::cout << node << std::endl;
  // };

  // doc->for_each(callback);

  return 0;
};
