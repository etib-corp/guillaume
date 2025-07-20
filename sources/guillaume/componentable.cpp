#include "guillaume/componentable.hpp"

namespace guigui {

void Componentable::_mark_dirty()
{
    if (_dirty_callback) {
        _dirty_callback();
    }
}

void Componentable::set_dirty_callback(DirtyCallback callback)
{
    _dirty_callback = callback;
}

} // namespace guigui
