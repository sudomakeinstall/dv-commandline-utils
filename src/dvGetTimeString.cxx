#include <dvGetTimeString.h>

#include <iomanip>
#include <ctime>
#include <sstream>

namespace dv {

std::string
GetTimeString(const std::string& format) {

    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, format.c_str());
    return oss.str();

}

}
