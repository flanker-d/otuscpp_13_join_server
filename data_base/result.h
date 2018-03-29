#pragma once

#include <map>
#include <string>

namespace db
{
  using result_t = std::string;

  namespace result
  {
    result_t ok();
    result_t err();
    result_t make_err(const std::string& a_msg);
    result_t make_intersection(const std::map<int, std::pair<std::string*, std::string*>>& a_result);
    result_t make_symmetric_difference(const std::map<int, std::pair<std::string*, std::string*>>& a_result);
  } //namespace result
} //namespace db