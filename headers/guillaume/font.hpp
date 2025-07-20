/**
 * @file font.hpp
 * @brief Font representation class for the Guillaume GUI framework
 * 
 * This file contains the Font class which encapsulates font properties
 * including name, file path, and size for text rendering operations.
 */

#pragma once

#include <string>

namespace guigui {

/**
 * @class Font
 * @brief Represents a font with name, path, and size properties
 * 
 * The Font class encapsulates all the information needed to render text
 * with a specific font. It stores the font name (for identification),
 * the file path to the font file, and the size for rendering.
 * 
 * @details Features:
 * - Font name for identification and debugging
 * - File path to the actual font file (.ttf, .otf, etc.)
 * - Size specification for text rendering
 * - Copy constructor for font duplication
 * - String representation for debugging
 * 
 * @note The font file must be accessible at the specified path when
 * the font is used for rendering operations.
 */
class Font {
private:
    std::string _name;  ///< Display name of the font (e.g., "Arial", "Roboto")
    std::string _path;  ///< File system path to the font file
    float _size;        ///< Font size in points

protected:
public:
    /**
     * @brief Constructs a Font with specified properties
     * 
     * @param name Display name of the font
     * @param path File system path to the font file
     * @param size Font size in points
     * 
     * @details Creates a font object with the specified properties.
     * The path should point to a valid font file (e.g., .ttf, .otf).
     */
    Font(const std::string& name, const std::string& path, float size);
    
    /**
     * @brief Copy constructor
     * 
     * @param other Font to copy from
     * 
     * Creates a new Font with the same properties as the source font.
     */
    Font(const Font& other);
    
    /**
     * @brief Destructor
     * 
     * Cleans up the Font object (no special cleanup needed for this class).
     */
    ~Font();

    // Getters
    
    /**
     * @brief Gets the font name
     * 
     * @return const std::string& Reference to the font name
     * 
     * @details The font name is typically used for identification
     * and debugging purposes.
     */
    const std::string& get_name() const;
    
    /**
     * @brief Gets the font file path
     * 
     * @return const std::string& Reference to the font file path
     * 
     * @details This path should point to a valid font file that
     * can be loaded by the text rendering system.
     */
    const std::string& get_path() const;
    
    /**
     * @brief Gets the font size
     * 
     * @return float Font size in points
     * 
     * @details The size determines how large the text will appear
     * when rendered using this font.
     */
    float get_size() const;

    // Setters
    
    /**
     * @brief Sets the font name
     * 
     * @param new_name New display name for the font
     * 
     * @details Updates the font's display name. This is mainly
     * used for identification and debugging.
     */
    void set_name(const std::string& new_name);
    
    /**
     * @brief Sets the font file path
     * 
     * @param new_path New file system path to the font file
     * 
     * @details Updates the path to the font file. The new path
     * should point to a valid, accessible font file.
     */
    void set_path(const std::string& new_path);
    
    /**
     * @brief Sets the font size
     * 
     * @param new_size New font size in points
     * 
     * @details Updates the font size. Larger values produce
     * larger text when rendered.
     */
    void set_size(float new_size);

    /**
     * @brief Converts the font to a string representation
     * 
     * @return std::string String describing the font properties
     * 
     * @details Creates a human-readable string containing the font's
     * name, path, and size. Useful for debugging and logging.
     */
    std::string to_string() const;
};

} // namespace guigui
