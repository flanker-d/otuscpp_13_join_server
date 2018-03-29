#include <data_base/table.h>

namespace db
{
  table::table(const std::string &a_table_name) : m_table_name(a_table_name)
  {
  }

  result_t table::insert(const int a_id, const std::string &a_name)
  {
    auto found = m_records.find(a_id);
    if(found == m_records.end())
    {
      m_records.emplace(std::make_pair(a_id, a_name));
      m_keys.emplace(a_id);
      return result::ok();
    }
    return result::make_err("duplicate " + std::to_string(a_id));
  }

  result_t table::truncate()
  {
    m_records.clear();
    m_keys.clear();
    return result::ok();
  }

  std::size_t table::size()
  {
    return m_records.size();
  }

  const records_t &table::get_records() const
  {
    return m_records;
  }

  const keys_t &table::get_keys() const
  {
    return m_keys;
  }

  const std::string &table::get_table_name() const
  {
    return  m_table_name;
  }

} //namespace db