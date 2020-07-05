#include "codelithic/utils/StringUtil.hpp"

#include <iostream>
#include <random>
#include <regex>
#include <string>

std::vector<char> charset() {
    return std::vector<char>(
            {/*'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',*/
             'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
             'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
             'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
             'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'});
};

StringUtil::StringUtil() {}

std::string StringUtil::randomStrind(int length) {
    const auto ch_set = charset();
    std::default_random_engine rng(std::random_device{}());
    std::uniform_int_distribution<> dist(0, ch_set.size() - 1);
    auto randchar = [ch_set, &dist, &rng]() { return ch_set[dist(rng)]; };
    std::string str(length, 0);
    std::generate_n(str.begin(), length, randchar);
    return str;
}

// void StringUtil::jsonEncode(std::string& data) {
//
//
//}

void StringUtil::encode(std::string &data) {
    std::string buffer;
    buffer.reserve(data.size());
    for (size_t pos = 0; pos != data.size(); ++pos) {
        switch (data[pos]) {
            case '&':
                buffer.append("&amp;");
                break;
            case '\"':
                buffer.append("&quot;");
                break;
            case '\'':
                buffer.append("&apos;");
                break;
            case '<':
                buffer.append("&lt;");
                break;
            case '>':
                buffer.append("&gt;");
                break;
            default:
                buffer.append(&data[pos], 1);
                break;
        }
    }
    data.swap(buffer);
}

std::string StringUtil::url_encode(const std::string &value) {
    using namespace std;

    ostringstream escaped;
    escaped.fill('0');
    escaped << hex;

    for (string::const_iterator i = value.begin(), n = value.end(); i != n; ++i) {
        string::value_type c = (*i);

        // Keep alphanumeric and other accepted characters intact
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            escaped << c;
            continue;
        }

        // Any other characters are percent-encoded
        escaped << uppercase;
        escaped << '%' << setw(2) << int((unsigned char) c);
        escaped << nouppercase;
    }

    return escaped.str();
}

void StringUtil::filterLines(std::string &value, std::string filter,
                             std::stringbuf &buffer) {
    std::istringstream iss(value);
    std::ostream os(&buffer);

    std::string line;
    while (std::getline(iss, line)) {
        if (line.find(filter) != std::string::npos) os << line << std::endl;
    }

    iss.seekg(0);
}

void StringUtil::getIntoTag(std::string &value, std::string from,
                            std::string to) {
    if (from.empty() || to.empty()) {
        return;
    }

    auto FROM = value.find(from);
    if (FROM == std::string::npos) {
        value.clear();
        return;
    } else {
        FROM = FROM + from.size();
    }

    auto TO = value.find(to, FROM);
    if (TO == std::string::npos) {
        value.clear();
        return;
    } else {
        //		TO = TO - to.size() - 1;
    }

    if (FROM > TO) {
        value.clear();
        return;
    }

    value.erase(TO);
    value.erase(0, FROM);

    // return value.substr(FROM, TO-FROM);
    //	return "";
}

StringUtil::~StringUtil() {}
