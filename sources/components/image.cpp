/*
 Copyright (c) 2026 ETIB Corporation

 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 of the Software, and to permit persons to whom the Software is furnished to do
 so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#include "guillaume/components/image.hpp"

namespace guillaume::components
{
	Image &Image::setTexturePath(const std::string &path)
	{
		if (_texturePath == path) {
			return *this;
		}
		_texturePath   = path;
		_loadState	   = LoadState::Pending;
		_usingFallback = false;
		setHasChanged(true);
		return *this;
	}

	Image &Image::setFallbackPath(const std::string &path)
	{
		if (_fallbackPath == path) {
			return *this;
		}
		_fallbackPath = path;
		if (_loadState == LoadState::Failed) {
			_loadState = LoadState::Pending;
		}
		setHasChanged(true);
		return *this;
	}

	const std::string &Image::getFallbackPath(void) const
	{
		return _fallbackPath;
	}

	Image::LoadState Image::getLoadState(void) const
	{
		return _loadState;
	}

	Image &Image::setLoadState(LoadState state, bool usingFallback)
	{
		// Bookkeeping only: the render system records the outcome without
		// flagging a change, which would re-run entity updates every frame.
		_loadState	   = state;
		_usingFallback = usingFallback;
		return *this;
	}

	bool Image::isUsingFallback(void) const
	{
		return _usingFallback;
	}

	std::string Image::getTexturePath(void) const
	{
		return _texturePath;
	}
}	 // namespace guillaume::components
