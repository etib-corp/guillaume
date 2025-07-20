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
    Font(const std::string& name, const std::string& path, float size);
    Font(const Font& other);
    ~Font();

    const std::string& get_name() const;
    const std::string& get_path() const;
    float get_size() const;

    void set_name(const std::string& new_name);
    void set_path(const std::string& new_path);
    void set_size(float new_size);

    std::string to_string() const;
};

} // namespace guigui
