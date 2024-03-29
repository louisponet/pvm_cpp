#include "pvm_cpp/utils.hpp"
#include <glob.h>
#include <string.h>
#include <chrono>
#include <fstream>
#include <stdexcept>
#include <string>
#include <thread>
#include "fmt/format.h"

namespace utils {

boost::filesystem::path portpath(const int child_id) {
    return boost::filesystem::path(fmt::format("servers/{}/port", child_id));
}

bool ispath(const std::string& p_string) {
    boost::filesystem::path p(p_string);
    return boost::filesystem::exists(p);
    ;
}

bool ispath(const boost::filesystem::path& path) {
    return boost::filesystem::exists(path);
    ;
}

void thread_ms_sleep(int milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

// based on https://stackoverflow.com/a/8615450/4765406
std::vector<std::string> glob(const std::string& pattern) {
    using namespace std;

    // glob struct resides on the stack
    glob_t glob_result;
    memset(&glob_result, 0, sizeof(glob_result));

    // do the glob operation
    int return_value = glob(pattern.c_str(), GLOB_TILDE, NULL, &glob_result);
    if (return_value != 0) {
        globfree(&glob_result);
        throw runtime_error(
            fmt::format("glob() failed with return_value {}", return_value));
    }

    // collect all the filenames into a std::list<std::string>
    vector<string> filenames;
    for (size_t i = 0; i < glob_result.gl_pathc; ++i) {
        filenames.push_back(string(glob_result.gl_pathv[i]));
    }

    // cleanup
    globfree(&glob_result);

    // done
    return filenames;
}

std::string join(const std::vector<std::string>& strings,
                 const std::string& delim) {
    std::string out = strings[0];

    for (int i = 1; i < strings.size(); i++) {
        out += delim;
        out += strings[i];
    }
    return out;
}

}  // namespace utils
