#include <data_base/data_base.h>
#include <set>

namespace db
{
  data_base::data_base()
  {
    m_tables.insert(std::make_pair("A", std::make_shared<table>("A")));
    m_tables.insert(std::make_pair("B", std::make_shared<table>("B")));

    //auto table_a = m_tables.find("A");
    //table_a->second->insert(0, "lean");
    //table_a->second->insert(1, "sweater");
    //table_a->second->insert(2, "frank");
    //table_a->second->insert(3, "violation");
    //table_a->second->insert(4, "quality");
    //table_a->second->insert(5, "precision");

    //auto table_b = m_tables.find("B");
    //table_b->second->insert(3, "proposal");
    //table_b->second->insert(4, "example");
    //table_b->second->insert(5, "lake");
    //table_b->second->insert(6, "flour");
    //table_b->second->insert(7, "wonder");
    //table_b->second->insert(8, "selection");
  }

  result_t data_base::process_command(const std::string &a_cmd)
  {
    auto parsed_commands = parse(a_cmd);
    const std::string& cmd_type = parsed_commands.at(0);

    if(cmd_type == "INSERT")
      return insert(parsed_commands);
    else if(cmd_type == "TRUNCATE")
      return truncate(parsed_commands);
    else if(cmd_type == "INTERSECTION")
      return intersection(parsed_commands);
    else if(cmd_type == "SYMMETRIC_DIFFERENCE")
      return symmetric_difference(parsed_commands);
    else
      result::err();
  }

  result_t data_base::insert(const std::vector<std::string> &a_parsed_commands)
  {
    if(a_parsed_commands.size() != 4)
      return result::make_err("wrong number of parameters: " + std::to_string(a_parsed_commands.size()) + ", must be 4");

    const std::string& table_name = a_parsed_commands.at(1);
    const int id = std::stoi(a_parsed_commands.at(2));
    const std::string& name = a_parsed_commands.at(3);

    auto found = m_tables.find(table_name);
    if(found != m_tables.end())
      return found->second->insert(id, name);

    return result::make_err("table " + table_name + " not found");
  }

  result_t data_base::truncate(const std::vector<std::string> &a_parsed_commands)
  {
    if(a_parsed_commands.size() != 2)
      return result::make_err("wrong number of parameters: " + std::to_string(a_parsed_commands.size()) + ", must be 2");

    const std::string& table_name = a_parsed_commands.at(1);

    auto found = m_tables.find(table_name);
    if(found != m_tables.end())
      return found->second->truncate();

    return result::make_err("table " + table_name + " not found");
  }

  result_t data_base::intersection(const std::vector<std::string> &a_parsed_commands)
  {
    if(a_parsed_commands.size() != 1)
      return result::make_err("wrong number of parameters: " + std::to_string(a_parsed_commands.size()) + ", must be 1");


    std::set<int> intersection_set;
    std::set_intersection(m_tables.at("A")->get_keys().begin(), m_tables.at("A")->get_keys().end(),
                          m_tables.at("B")->get_keys().begin(), m_tables.at("B")->get_keys().end(),
                          std::inserter(intersection_set, intersection_set.begin()));

    std::map<int, std::pair<std::string*, std::string*>> intersection_map;
    for(int i : intersection_set)
    {
      auto found_a = m_tables.at("A")->get_records().find(i);
      auto found_b = m_tables.at("B")->get_records().find(i);
      std::string* name_a_ptr = nullptr;
      std::string* name_b_ptr = nullptr;

      if(found_a != m_tables.at("A")->get_records().end())
        name_a_ptr = const_cast<std::string*>(&found_a->second);

      if(found_b != m_tables.at("B")->get_records().end())
        name_b_ptr = const_cast<std::string*>(&found_b->second);

      intersection_map.insert(std::make_pair(i, std::make_pair(name_a_ptr, name_b_ptr)));
    }

    return result::make_intersection(intersection_map);
  }

  result_t data_base::symmetric_difference(const std::vector<std::string> &a_parsed_commands)
  {
    if(a_parsed_commands.size() != 1)
      return result::make_err("wrong number of parameters: " + std::to_string(a_parsed_commands.size()) + ", must be 1");


    std::set<int> symm_diff_set;
    std::set_symmetric_difference(m_tables.at("A")->get_keys().begin(), m_tables.at("A")->get_keys().end(),
                          m_tables.at("B")->get_keys().begin(), m_tables.at("B")->get_keys().end(),
                          std::inserter(symm_diff_set, symm_diff_set.begin()));

    std::map<int, std::pair<std::string*, std::string*>> symm_diff_map;
    for(int i : symm_diff_set)
    {
      auto found_a = m_tables.at("A")->get_records().find(i);
      auto found_b = m_tables.at("B")->get_records().find(i);
      std::string* name_a_ptr = nullptr;
      std::string* name_b_ptr = nullptr;

      if(found_a != m_tables.at("A")->get_records().end())
        name_a_ptr = const_cast<std::string*>(&found_a->second);

      if(found_b != m_tables.at("B")->get_records().end())
        name_b_ptr = const_cast<std::string*>(&found_b->second);

      symm_diff_map.insert(std::make_pair(i, std::make_pair(name_a_ptr, name_b_ptr)));
    }

    return result::make_symmetric_difference(symm_diff_map);
  }

  std::vector<std::string> data_base::parse(const std::string &a_cmd)
  {
    std::regex word_regex(R"([^\s]+)");
    auto words_begin = std::sregex_iterator(a_cmd.begin(), a_cmd.end(), word_regex);
    auto words_end = std::sregex_iterator();

    std::vector<std::string> packed_params;
    packed_params.clear();
    for(auto it = words_begin; it != words_end; it++)
    {
      std::smatch match = *it;
      packed_params.push_back(match.str());
    }
    return packed_params;
  }
} //namespace db
