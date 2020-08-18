#include "Utils.h"

std::vector<std::string> split(const std::string& str, const std::string& delimiter) {
    int lastPos = 0;
    std::vector<std::string> pieces;
    while (true) {
        auto pos = str.find(delimiter, lastPos);
        if (pos != std::string::npos) {
            pieces.push_back(str.substr(lastPos, pos - lastPos));
            lastPos = pos + delimiter.size();
        } else {
            pieces.push_back(str.substr(lastPos));
            break;
        }
    }
    return pieces;
}