#pragma once

#include "fsHelper.cpp"
const std::vector<element_t> collect_relivant_files (const std::vector<element_t>& directory);

const std::vector<element_t> collect_relivant_files (const element_t& directory);

void create_directory_from_extension_type (const element_t base_save_location, const element_t curr_extension);

inline std::vector<element_t> remove_duplicates (std::vector<element_t>& list_of_files);

void loading_bar (double curr_percent, int curr_cnt, int& prev_per, std::vector<std::string>& curr_bar);

void end_program_signal (const unsigned short times);

