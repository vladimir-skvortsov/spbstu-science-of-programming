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

  // Test XML parser
  document.load("./example.xml");
  document.print();

  // Test iterator
  for (XML::Document::iterator it = document.begin(); it != document.end(); ++it) {
    std::cout << "tag: " << it->tag << ", value: " << it->value << std::endl;
  }

  std::cout << std::endl;

  // Test find methods
  XML::Document::iterator it1 = document.find_by_tag("city");
  std::cout << "The city is " << it1->value << std::endl;

  std::cout << std::endl;

  XML::Document::iterator it2 = document.find_by_value("Male");
  std::cout << "Gender is contained inside of " << it2->tag << " tag" << std::endl;

  // Test add method
  XML::Node* favorite_musician_node = new XML::Node("favorite-musician", "Kizaru");
  XML::Document::iterator it3 = document.find_by_tag("person");
  document.add(it3, favorite_musician_node);

  // Test erase method
  XML::Document::iterator it4 = document.find_by_tag("contact");
  document.erase(it4);

  // Write new file
  document.save("./output.xml");

  return 0;
};
