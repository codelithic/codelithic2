#ifndef PROJECT_FILEUTIL_HPP
#define PROJECT_FILEUTIL_HPP

#include <cstdio>
#include <fstream>
#include <map>
#include <sstream>
#include <stdbool.h>
#include <vector>

class FileUtil {
public:
    FileUtil() = default;
    virtual ~FileUtil() = default;

    static int copyTextFile(char *source, char *dest) {
        std::string line;
        std::ifstream fsource(source);
        std::ofstream fdest(dest);
        if (!fsource) {
            return 1;
        }
        if (!fdest) {
            return 2;
        }
        if (fsource.is_open()) {
            while (getline(fsource, line)) {
                fdest << line << '\n';
            }
        }
        fdest.close();
        fsource.close();
        return 0;
    }

    static bool is_file_exist(const char *fileName) {
        std::ifstream infile(fileName);
        return infile.good();
    }

    static void writeContent(const char *fileName, const std::string &content) {
        std::ofstream file(fileName);
        if (file.good()) {
            file << content;
            file.flush();
            file.close();
        } else {
            std::string err;
            err.append("Impossible to write the file: ");
            err.append(fileName);
            throw std::runtime_error(err);
        }
    }

    static void loadFile(std::string_view filePath, std::string &buffer) {
        if (FileUtil::is_file_exist(/*(char*)filePath.c_str())*/ filePath.data())) {
            std::ifstream t(filePath.data());
            std::stringstream sBuffer;
            sBuffer << t.rdbuf();

            buffer.clear();
            buffer.assign(sBuffer.str());
        }
    }

    static int remove(const char *filename) {
        if (!filename) return 0;
        return std::remove(filename);
    }

    static void deserialize(std::istream &in,
                            std::map<std::string, std::string> &map) {
        map.clear();
        int mappaLen = 0;
        int len = 0;
        char PIPE;
        if (in) {
            in >> mappaLen;
            if (mappaLen > 0) {
                std::string key;
                std::string value;
                for (int i = 0; i < mappaLen; i++) {
                    key.clear();
                    value.clear();
                    in >> PIPE >> len >> PIPE;
                    if (len) {
                        std::vector<char> tmp(len);
                        in.read(tmp.data(), len);
                        key.assign(tmp.data(), len);
                    }
                    in >> PIPE >> len >> PIPE;
                    if (len) {
                        std::vector<char> tmp(len);
                        in.read(tmp.data(), len);
                        value.assign(tmp.data(), len);
                    }
                    map[key] = value;
                }
            }
        }
    }

    static void deserialize(std::istream &in, std::vector<std::string> &vector) {
        vector.clear();
        int vectorLen = 0;
        int len = 0;
        char PIPE;
        if (in) {
            std::string key;
            in >> vectorLen;
            for (int i = 0; i < vectorLen; i++) {
                key.clear();
                in >> PIPE >> len >> PIPE;
                if (len) {
                    std::vector<char> tmp(len);
                    in.read(tmp.data(), len);
                    key.assign(tmp.data(), len);
                }
                vector.push_back(key);
            }
        }
    }

    static void serialize(std::ostream &out,
                          std::map<std::string, std::string> &map) {
        unsigned char S = 15;
        if (out) {
            out << map.size();
            for (auto &m : map) {
                out << S << m.first.size() << S << m.first;
                out << S << m.second.size() << S << m.second;
            }
        }
    }

    static void serialize(std::ostream &out, std::vector<std::string> &vector) {
        unsigned char S = 15;
        if (out) {
            out << vector.size();
            for (auto &m : vector) {
                out << S << m.size() << S << m;
            }
        }
    }


    //	std::stringbuf strBuffer;
    //	std::ostream os(&strBuffer);
    //	std::istream is(&strBuffer);
    //	is.seekg (0, is.beg);

private:
};


#endif// PROJECT_FILEUTIL_HPP
