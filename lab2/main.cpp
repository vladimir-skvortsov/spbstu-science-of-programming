#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <fstream>
#include <algorithm>
#include "xml.h"
#include "resource_xml.h"

int main() {
  std::unique_ptr<Resource_XML> resource = Resource_XML::create("./example.xml");

  // // Test XML parser
  resource->print();

  // Test iterator
  for (Resource_XML::iterator it = resource->begin(); it != resource->end(); ++it) {
    std::cout << "tag: " << it->tag << ", value: " << it->value << std::endl;
  }

  std::cout << std::endl;

  // Test find methods
  Resource_XML::iterator it1 = resource->find_by_tag("city");
  std::cout << "The city is " << it1->value << std::endl;

  std::cout << std::endl;

  Resource_XML::iterator it2 = resource->find_by_value("Male");
  std::cout << "Gender is contained inside of " << it2->tag << " tag" << std::endl;

  // Test add method
  XML::Node* favorite_musician_node = new XML::Node("favorite-musician", "Kizaru");
  Resource_XML::iterator it3 = resource->find_by_tag("person");
  resource->add(it3, favorite_musician_node);

  // Test erase method
  Resource_XML::iterator it4 = resource->find_by_tag("contact");
  resource->erase(it4);

  // Write new file
  resource->save("./output.xml");

  return 0;
};
