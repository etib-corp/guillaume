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

#include "font.hpp"

namespace guillaume {

Font::Font(const std::string &fontPath, int fontSize)
    : _fontPath(fontPath), _fontSize(fontSize), _nativeHandle(nullptr) {}

const std::string &Font::getFontPath() const { return _fontPath; }

int Font::getFontSize() const { return _fontSize; }

void *Font::getNativeHandle() const { return _nativeHandle; }

void Font::setNativeHandle(void *handle) { _nativeHandle = handle; }

bool Font::isValid() const { return _nativeHandle != nullptr; }

std::string Font::getFamilyName() const { return ""; }

std::string Font::getStyleName() const { return ""; }

int Font::getLineHeight() const { return _fontSize; }

} // namespace guillaume
