#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <sstream>

class ImageLoader {
public:
	static std::vector<unsigned char> LoadImage(const std::string& path);
};

class Library {
public:
    void* handle;
    std::string name;
    ~Library();

    template<typename T>
    T GetFunction(const std::string& name) {
        return (T)GetFPtr(name);
    }

    void* GetFPtr(const std::string& name);
};

class LibraryLoader {
public:
    static Library Load(const std::string& path);
};

class Obj {
public:
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    bool LoadFromObjFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            return false;
        }

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string prefix;
            iss >> prefix;

            if (prefix == "v") {
                float x, y, z;
                iss >> x >> y >> z;
                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);
            }
            else if (prefix == "f") {
                std::string vertexIndices[3];
                iss >> vertexIndices[0] >> vertexIndices[1] >> vertexIndices[2];

                for (int i = 0; i < 3; ++i) {
                    std::size_t pos = vertexIndices[i].find("/");
                    vertexIndices[i] = vertexIndices[i].substr(0, pos);
                    unsigned int index = std::stoi(vertexIndices[i]) - 1;
                    indices.push_back(index);
                }
            }
        }

        return true;
    }
};
