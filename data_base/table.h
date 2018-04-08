#pragma once

#include <map>
#include <set>
#include <common/interface/interface.h>
#include <data_base/result.h>

namespace db
{
  using namespace common;

  using records_t = std::map<int, std::string>;
  using keys_t = std::set<int>;

  class table : public common::interface<table>
  {
    public:
      table(const std::string& a_table_name);
      result_t insert(const int a_id, const std::string& a_name);
      result_t truncate();

      std::size_t size();
      const records_t& get_records() const;
      const keys_t& get_keys() const;
      const std::string& get_table_name() const;

    private:
      std::string m_table_name;
      records_t m_records;
      keys_t m_keys;
  };
} //namespace db
