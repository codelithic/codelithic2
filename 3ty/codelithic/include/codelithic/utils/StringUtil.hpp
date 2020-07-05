#ifndef STRINGUTIL_H
#define STRINGUTIL_H

#include <algorithm>
#include <cctype>
#include <functional>
#include <iomanip>
#include <locale>
#include <sstream>
#include <string>
#include <vector>

class StringUtil {
public:
    //	http://en.cppreference.com/w/cpp/string/basic_string/to_string

    template<typename... Args>
    static std::string string_format(const std::string &format, Args... args) {
        size_t size = std::snprintf(nullptr, 0, format.c_str(), args...) +
                      1;// Extra space for '\0'
        std::unique_ptr<char[]> buf(new char[size]);
        std::snprintf(buf.get(), size, format.c_str(), args...);
        return std::string(buf.get(),
                           buf.get() + size - 1);// We don't want the '\0' inside
    }

    template<typename charT>
    struct my_equal {
        my_equal(const std::locale &loc) : loc_(loc) {}

        bool operator()(charT ch1, charT ch2) {
            return std::toupper(ch1, loc_) == std::toupper(ch2, loc_);
        }

    private:
        const std::locale &loc_;
    };

    template<typename T>
    static int ci_find_substr(const T &str1, const T &str2,
                              const std::locale &loc = std::locale()) {
        typename T::const_iterator it =
                std::search(str1.begin(), str1.end(), str2.begin(), str2.end(),
                            my_equal<typename T::value_type>(loc));
        if (it != str1.end())
            return it - str1.begin();
        else
            return -1;// try again
    }

    // std::string str1 = "FIRST HELLO";
    // std::wstring wstr1 = L"ОПЯТЬ ПРИВЕТ";

    //	static std::string myreplace(std::string &s,
    //			const std::string &toReplace,
    //			const std::string &replaceWith) {
    //
    //		return (s.replace(s.find(toReplace), toReplace.length(),
    //replaceWith));
    //	}

    //	static void jsonEncode(std::string& data);
    static void encode(std::string &data);
    static std::string url_encode(const std::string &value);

    static std::string innerReplace(std::string &str, const std::string &from,
                                    const std::string &to) {
        size_t start_pos = 0;
        while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
            str.replace(start_pos, from.length(), to);
            start_pos +=
                    to.length();// Handles case where 'to' is a substring of 'from'
        }
        return str;
    }

    static std::string myreplace(std::string str, const std::string &from,
                                 const std::string &to) {
        size_t start_pos = 0;
        while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
            str.replace(start_pos, from.length(), to);
            start_pos +=
                    to.length();// Handles case where 'to' is a substring of 'from'
        }
        return str;
    }

    StringUtil();
    virtual ~StringUtil();

    // static inline std::string& ltrim(std::string &s) {
    // 	s.erase(s.begin(), std::find_if(s.begin(), s.end(),
    // 									std::not1(std::ptr_fun<int,
    // int>(std::isspace)))); 	return s;
    // }

    // static inline std::string& rtrim(std::string &s) {
    // 	s.erase(std::find_if(s.rbegin(), s.rend(),
    // 						 std::not1(std::ptr_fun<int, int>(std::isspace))).base(),
    // s.end()); 	return s;
    // }

    // static inline std::string& trim(std::string &s) {
    // 	s.erase(std::remove(s.begin(), s.end(), '\r'), s.end());
    // 	s.erase(std::remove(s.begin(), s.end(), '\n'), s.end());

    // 	return ltrim(rtrim(s));
    // }

    static std::string &ltrim(std::string &str,
                              const std::string &chars = "\t\n\v\f\r ") {
        str.erase(0, str.find_first_not_of(chars));
        return str;
    }

    static std::string &rtrim(std::string &str,
                              const std::string &chars = "\t\n\v\f\r ") {
        str.erase(str.find_last_not_of(chars) + 1);
        return str;
    }

    static std::string &trim(std::string &str,
                             const std::string &chars = "\t\n\v\f\r ") {
        return ltrim(rtrim(str, chars), chars);
    }

    static std::string randomStrind(int length);

    template<typename Out>
    static void split(const std::string &s, char delim, Out result) {
        std::stringstream ss;
        ss.str(s);
        std::string item;
        while (std::getline(ss, item, delim)) {
            *(result++) = item;
        }
    }

    static std::vector<std::string> split(const std::string &s, char delim) {
        std::vector<std::string> elems;
        split(s, delim, std::back_inserter(elems));
        return elems;
    }

    static std::string toUpper(std::string &s) {
        std::string str = s;
        std::transform(str.begin(), str.end(), str.begin(), ::toupper);

        return str;
    }

    static std::string tolower(std::string &s) {
        std::string str = s;
        std::transform(str.begin(), str.end(), str.begin(), ::tolower);

        return str;
    }

    static void getIntoTag(std::string &value, std::string from, std::string to);
    static void filterLines(std::string &value, std::string filter,
                            std::stringbuf &buffer);

    //	//http://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
    //	static std::string string_format(const std::string fmt, ...) {
    //		int size = ((int) fmt.size()) * 2 + 50; // Use a rubric appropriate
    //for your code 		std::string str; 		va_list ap; 		while (1) { // Maximum two passes
    //on a POSIX system... 			str.resize(size); 			va_start(ap, fmt); 			int n =
    //vsnprintf((char *) str.data(), size, fmt.c_str(), ap); 			va_end(ap); 			if (n >
    //-1 && n < size) { // Everything worked 				str.resize(n); 				return str;
    //			}
    //			if (n > -1) // Needed size returned
    //				size = n + 1; // For null char
    //			else
    //				size *= 2; // Guess at a larger size (OS
    //specific)
    //		}
    //		return str;
    //	}

    static bool is_number(const std::string &s) {
        return !s.empty() && std::find_if(s.begin(), s.end(), [](char c) {
                                 return !std::isdigit(c);
                             }) == s.end();
    }

private:
};

#endif /* STRINGUTIL_H */
