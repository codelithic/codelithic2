

#ifndef CODELITHIC_COM_STRINGBUILDER_H
#define CODELITHIC_COM_STRINGBUILDER_H


#include <algorithm>// for for_each
#include <ctime>    // clock_t, clock, CLOCKS_PER_SEC
#include <deque>    // used in class StringBuilder, slightly faster than list.
#include <fstream>
#include <iostream>// for std::cout
#include <numeric> // for accumulate
#include <sstream>
#include <string>// for std::basic_string;
#include <vector>// used in main()


namespace codelithic {
    template<typename chr>
    class StringBuilder {
        typedef std::basic_string<chr> string_t;

        typedef std::deque<string_t> container_t;
        container_t m_Data;
        typedef typename string_t::size_type size_type;// Reuse the size type in the string.
        size_type m_totalSize;
        void append(const string_t &src) {
            m_Data.push_back(src);
            m_totalSize += src.size();
        }
        // No copy constructor, no assignment.
        StringBuilder(const StringBuilder &);
        StringBuilder &operator=(const StringBuilder &);

    public:
        StringBuilder(const string_t &src) {
            if (src.empty()) {
                m_totalSize = 0;
            } else {
                m_Data.push_back(src);
                m_totalSize = src.size();
            }
        }
        StringBuilder() {
            m_totalSize = 0;
        }

        StringBuilder &Append(const string_t &src) {
            append(src);
            return *this;// allow chaining.
        }
        template<class iterator>
        StringBuilder &Add(const iterator &first, const iterator &afterLast) {
            // std::for_each and a lambda look like overkill here.
            for (iterator f = first; f != afterLast; ++f) {
                append(*f);
            }
            return *this;
        }
        StringBuilder &AppendLine(const string_t &src) {
            chr lineFeed[]{10, 0};// C++ 11. Feel the love!
            m_Data.push_back(src + lineFeed);
            m_totalSize += 1 + src.size();
            return *this;// allow chaining.
        }
        StringBuilder &AppendLine() {
            chr lineFeed[]{10, 0};
            m_Data.push_back(lineFeed);
            ++m_totalSize;
            return *this;// allow chaining.
        }
        StringBuilder &Clear() {
            m_totalSize = 0;
            m_Data.clear();
            return *this;
        }

        // ADVANCED TOPIC: AppendFormat()

        // Like C# StringBuilder.ToString()
        string_t ToString() const {
            string_t result;

            result.reserve(1 + m_totalSize);

            for (auto iter = m_Data.begin(); iter != m_Data.end(); ++iter) {
                result += *iter;
            }
            return result;
        }

        // Like Javascript array join.
        string_t Join(const string_t &delim) const {
            if (delim.empty()) {
                return ToString();
            }
            string_t result;
            if (m_Data.empty()) {
                return result;
            }

            size_type st = (delim.size() * (m_Data.size() - 1)) + m_totalSize + 1;
            result.reserve(st);

            struct adder {
                string_t m_Joiner;
                adder(const string_t &s) : m_Joiner(s) {
                }
                string_t operator()(string_t &preAllocated, const string_t &current) {
                    preAllocated += m_Joiner;
                    preAllocated += current;
                    return preAllocated;
                }
            } adr(delim);
            auto iter = m_Data.begin();
            result += *iter;
            return std::accumulate(++iter, m_Data.end(), result, adr);
        }

    };// class StringBuilder

}// namespace codelithic


#endif// MONKEYCRYPT_COM_STRINGBUILDER_H
