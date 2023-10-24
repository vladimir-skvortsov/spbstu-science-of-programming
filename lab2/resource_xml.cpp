#include "resource_xml.h"

std::unique_ptr<Resource_XML> Resource_XML::create(const std::string& path){
  return std::unique_ptr<Resource_XML>(new Resource_XML(path));
};
Resource_XML::Resource_XML(const std::string& path) {
  document = std::make_unique<XML::Document>(path);
};
