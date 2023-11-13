#pragma once
#include <unordered_map>
#include <vector>
#include <tuple>
#include <map>
#include <string>

template<typename ClassType, typename... Args>
class Wrapper {
  private:
    using MethodType = int (ClassType::*)(Args...);
    ClassType* instance;
    MethodType method;
    // vector of values that is set in () operator and used in call_func with
    // variadic template
    std::vector<int> arg_values;
    // Possible arguments and their order
    std::vector<std::string> arg_names;

    template<std::size_t... I>
    int call_func(std::index_sequence<I...>) {
      // Unpack indices ang unpack values from arg_values vector
      return (instance->*method)(arg_values[I]...);
    };

  public:
    Wrapper(ClassType* instance, MethodType method, std::unordered_map<std::string, int> args_map): instance(instance), method(method) {
      // Translate an unordered map of arguments into a vector of argument names
      for (const auto& pair : args_map) {
        arg_names.push_back(pair.first);
      }
    };

    int operator () (std::unordered_map<std::string, int> args_map) {
      for (const auto& arg_name : arg_names) {
        if (args_map.find(arg_name) == args_map.end()) {
          throw std::runtime_error("Could not find argument " + arg_name);
        }

        arg_values.push_back(args_map[arg_name]);
      }

      // Call function with std::make_index_sequence, that creates
      // sequence of indices from 0 to n - 1, that can be packed in the arguments
      // See more details: https://habr.com/ru/companies/otus/articles/655715/
      return call_func(std::make_index_sequence<sizeof...(Args)>{});
    };
};
