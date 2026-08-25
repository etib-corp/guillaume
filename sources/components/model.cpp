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

#include "guillaume/components/model.hpp"

namespace guillaume::components
{
	Model &Model::setModelPath(const std::string &path)
	{
		if (_modelPath == path) {
			return *this;
		}
		_modelPath = path;
		setHasChanged(true);
		return *this;
	}

	std::string Model::getModelPath(void) const
	{
		return _modelPath;
	}

	Model &Model::setTexturePath(const std::string &path)
	{
		if (_texturePath == path) {
			return *this;
		}
		_texturePath = path;
		setHasChanged(true);
		return *this;
	}

	std::string Model::getTexturePath(void) const
	{
		return _texturePath;
	}
}	 // namespace guillaume::components
