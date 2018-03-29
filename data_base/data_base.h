#pragma once

#include <data_base/table.h>
#include <vector>
#include <regex>

namespace db
{
  class data_base : public common::interface<data_base>
  {
    public:
      data_base();
      result_t process_command(const std::string& a_cmd);

    private:
      result_t insert(const std::vector<std::string>& a_parsed_commands);
      result_t truncate(const std::vector<std::string>& a_parsed_commands);
      result_t intersection(const std::vector<std::string>& a_parsed_commands);
      result_t symmetric_difference(const std::vector<std::string>& a_parsed_commands);

      std::vector<std::string> parse(const std::string& a_cmd);

    private:
      std::map<std::string, table::ref> m_tables;
  };
} //namespace db
