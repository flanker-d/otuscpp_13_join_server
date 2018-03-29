#include <data_base/result.h>

namespace db
{
  namespace result
  {
    result_t ok()
    {
      return "OK\n";
    }

    result_t err()
    {
      return "ERR\n";
    }

    result_t make_err(const std::string& a_msg)
    {
      return "ERR " + a_msg + "\n";
    }

    result_t make_intersection(const std::map<int, std::pair<std::string*, std::string*>>& a_result)
    {
      result_t res;
      for(auto& i : a_result)
        res += std::to_string(i.first) + "," + *i.second.first + "," + *i.second.second + "\n";
      res += "OK\n";
      return res;
    }

    result_t make_symmetric_difference(const std::map<int, std::pair<std::string*, std::string*>>& a_result)
    {
      result_t res;
      for(auto& i : a_result)
        res += std::to_string(i.first) + ","
               + (i.second.first ? *i.second.first : "") + ","
               + (i.second.second ? *i.second.second : "") + "\n";
      res += "OK\n";
      return res;
    }
  } //namespace result
} //namespace db
