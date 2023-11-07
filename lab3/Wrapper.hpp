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
    std::vector<int> arg_values;
    std::vector<std::string> arg_names;

    template<std::size_t... I>
    int call_func(std::index_sequence<I...>) {
      return (instance->*method)(arg_values[I]...);
    };

  public:
    Wrapper(ClassType* instance, MethodType method, std::unordered_map<std::string, int> args_map): instance(instance), method(method) {
      for (const auto& pair : args_map) {
        arg_names.push_back(pair.first);
      }
    }

    int operator () (std::unordered_map<std::string, int> args_map) {
      int index = 0;
      for (const auto& arg_name : arg_names) {
        arg_values.push_back(args_map[arg_name]);
      }

      return call_func(std::make_index_sequence<sizeof...(Args)>{});
    };
};
