#pragma once
#include <filesystem>
#include <vector>
#include <string>
namespace utils {

std::filesystem::path portpath(const int child_id);

bool ispath(const std::string& p_string);
void thread_ms_sleep(int milliseconds);

std::vector<std::string> glob(const std::string &pattern);

}
