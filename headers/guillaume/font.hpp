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

#pragma once

#include <memory>
#include <string>

namespace guillaume {

/**
 * @brief Abstract font interface for text rendering.
 *
 * Represents a font that can be used to render text. Implementations handle
 * loading and managing font resources from files.
 */
class Font {
  protected:
    std::string _fontPath; ///< Path to the font file
    int _fontSize;         ///< Size of the font in points
    void *_nativeHandle;   ///< Pointer to the native font handle

  public:
    /**
     * @brief Construct a new Font object.
     * @param fontPath Path to the font file (TTF, OTF, etc.)
     * @param fontSize Size of the font in points
     */
    Font(const std::string &fontPath, int fontSize);

    /**
     * @brief Virtual destructor.
     */
    virtual ~Font() = default;

    /**
     * @brief Get the font file path.
     * @return The path to the font file.
     */
    const std::string &getFontPath() const;

    /**
     * @brief Get the font size in points.
     * @return The font size.
     */
    int getFontSize() const;

    /**
     * @brief Get the native font handle (implementation specific).
     * @return Pointer to the native font handle (e.g., TTF_Font* for SDL).
     */
    void *getNativeHandle() const;

    /**
     * @brief Set the native font handle.
     * @param handle Pointer to the native font handle.
     */
    void setNativeHandle(void *handle);

    /**
     * @brief Check if the font is valid and loaded.
     * @return True if the font is loaded, false otherwise.
     */
    virtual bool isValid(void) const;

    /**
     * @brief Get the font family name.
     * @return The font family name if available, empty string otherwise.
     */
    virtual std::string getFamilyName(void) const;

    /**
     * @brief Get the font style name.
     * @return The font style name if available, empty string otherwise.
     */
    virtual std::string getStyleName(void) const;

    /**
     * @brief Get the line height for this font.
     * @return The recommended line height in pixels.
     */
    virtual int getLineHeight(void) const;
};

} // namespace guillaume
