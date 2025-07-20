#include "guillaume/font.hpp"

namespace guigui {

Font::Font(const std::string& name, const std::string& path, float size)
    : _name(name)
    , _path(path)
    , _size(size)
{
}

Font::Font(const Font& other)
    : _name(other._name)
    , _path(other._path)
    , _size(other._size)
{
}

Font::~Font() 
{
}

const std::string& Font::get_name() const 
{ 
    return _name; 
}

const std::string& Font::get_path() const 
{ 
    return _path; 
}

float Font::get_size() const 
{ 
    return _size; 
}

void Font::set_name(const std::string& new_name) 
{ 
    _name = new_name; 
}

void Font::set_path(const std::string& new_path) 
{ 
    _path = new_path; 
}

void Font::set_size(float new_size) 
{ 
    _size = new_size; 
}

std::string Font::to_string() const
{
    return "Font(name=" + _name + ", path=" + _path + ", size=" + std::to_string(_size) + ")";
}

} // namespace guigui
