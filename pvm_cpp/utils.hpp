#include <boost/filesystem.hpp>
#include <string>
#include <vector>

namespace utils {

boost::filesystem::path portpath(const int child_id);

bool ispath(const std::string& p_string);
bool ispath(const boost::filesystem::path& p_string);

void thread_ms_sleep(int milliseconds);

std::vector<std::string> glob(const std::string& pattern);

}  // namespace utils
