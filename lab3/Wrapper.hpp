#pragma once
#include <unordered_map>
#include <string>

template<typename ClassType, typename... Args>
class Wrapper {
  private:
    using MethodType = int (ClassType::*)(Args...);
    ClassType* instance;
    MethodType method;
    std::unordered_map<std::string, int> args;

  public:
    Wrapper(ClassType* instance, MethodType function, std::unordered_map<std::string, int> args)
      : instance(instance), method(function), args(std::move(args)) {}

    int operator()(std::unordered_map<std::string, int> args) {
      for (const auto& arg : args) {
        args[arg.first] = arg.second;
      }
      return (instance->*method)(args["arg1"], args["arg2"]);
    };
};
