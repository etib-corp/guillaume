#pragma once

#include <string>

namespace guigui {

class Font {
private:
    std::string _name;
    std::string _path;
    float _size;

protected:
public:
    Font(const std::string& name, const std::string& path,
        float size)
        : _name(name)
        , _path(path)
        , _size(size)
    {
    }

    Font(const Font& other)
        : _name(other._name)
        , _path(other._path)
        , _size(other._size)
    {
    }

    ~Font() { }

    const std::string& get_name() const { return _name; }
    const std::string& get_path() const { return _path; }
    float get_size() const { return _size; }

    void set_name(const std::string& new_name) { _name = new_name; }
    void set_path(const std::string& new_path) { _path = new_path; }
    void set_size(float new_size) { _size = new_size; }

    std::string to_string() const
    {
        return "Font(name=" + _name + ", path=" + _path + ", size=" + std::to_string(_size) + ")";
    }
};

} // namespace guigui
