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

#include <array>
#include <cstdint>
#include <map>
#include <stdexcept>
#include <string>

#include <utility/math/color.hpp>

namespace guillaume {

/**
 * @brief Material Design color roles available in a scheme.
 */
enum class SchemeColorRole : std::size_t {
    Primary,
    SurfaceTint,
    OnPrimary,
    PrimaryContainer,
    OnPrimaryContainer,
    Secondary,
    OnSecondary,
    SecondaryContainer,
    OnSecondaryContainer,
    Tertiary,
    OnTertiary,
    TertiaryContainer,
    OnTertiaryContainer,
    Error,
    OnError,
    ErrorContainer,
    OnErrorContainer,
    Background,
    OnBackground,
    Surface,
    OnSurface,
    SurfaceVariant,
    OnSurfaceVariant,
    Outline,
    OutlineVariant,
    Shadow,
    Scrim,
    InverseSurface,
    InverseOnSurface,
    InversePrimary,
    PrimaryFixed,
    OnPrimaryFixed,
    PrimaryFixedDim,
    OnPrimaryFixedVariant,
    SecondaryFixed,
    OnSecondaryFixed,
    SecondaryFixedDim,
    OnSecondaryFixedVariant,
    TertiaryFixed,
    OnTertiaryFixed,
    TertiaryFixedDim,
    OnTertiaryFixedVariant,
    SurfaceDim,
    SurfaceBright,
    SurfaceContainerLowest,
    SurfaceContainerLow,
    SurfaceContainer,
    SurfaceContainerHigh,
    SurfaceContainerHighest,
    Count
};

/**
 * @brief Theme scheme variants (light/dark and contrast levels).
 */
enum class ThemeSchemeRole {
    Light,
    LightMediumContrast,
    LightHighContrast,
    Dark,
    DarkMediumContrast,
    DarkHighContrast
};

/**
 * @brief Theme palette categories used by Material Design.
 */
enum class ThemePaletteRole {
    Primary,
    Secondary,
    Tertiary,
    Neutral,
    NeutralVariant
};

/**
 * @brief Extended color class that includes additional metadata for a color.
 * Based on the Material Design color system.
 */
class ExtendedColor {
  public:
    using Color = utility::math::Color<std::uint8_t>; ///< Color type (RGBA)

  private:
    Color _color;
    std::string _name;
    std::string _description;
    bool _harmonized;

  public:
    /**
     * @brief Construct an empty extended color.
     */
    ExtendedColor(void)
        : _color(), _name(""), _description(""), _harmonized(false) {}

    /**
     * @brief Construct an extended color with metadata.
     * @param color RGBA color value.
     * @param name Human-readable name.
     * @param description Optional description.
     * @param harmonized True if harmonized from a source color.
     */
    ExtendedColor(const Color &color, const std::string &name,
                  const std::string &description, bool harmonized)
        : _color(color), _name(name), _description(description),
          _harmonized(harmonized) {}

    /**
     * @brief Default destructor.
     */
    ~ExtendedColor(void) = default;

    /**
     * @brief Get the RGBA color value.
     * @return Immutable color reference.
     */
    const Color &getColor(void) const { return _color; }

    /**
     * @brief Get the color name.
     * @return Immutable name reference.
     */
    const std::string &getName(void) const { return _name; }

    /**
     * @brief Get the color description.
     * @return Immutable description reference.
     */
    const std::string &getDescription(void) const { return _description; }

    /**
     * @brief Get harmonization state.
     * @return True when color is harmonized.
     */
    bool isHarmonized(void) const { return _harmonized; }

    /**
     * @brief Set the RGBA color value.
     * @param color New RGBA color.
     */
    void setColor(const Color &color) { _color = color; }

    /**
     * @brief Set the color name.
     * @param name New color name.
     */
    void setName(const std::string &name) { _name = name; }

    /**
     * @brief Set the color description.
     * @param description New color description.
     */
    void setDescription(const std::string &description) {
        _description = description;
    }

    /**
     * @brief Set harmonization state.
     * @param harmonized True if color is harmonized.
     */
    void setHarmonized(bool harmonized) { _harmonized = harmonized; }
};

/**
 * @brief Color scheme for a theme, containing all the defined colors. Based on
 * the Material Design color system.
 */
class Scheme {
  public:
    using Color = utility::math::Color<std::uint8_t>; ///< Color type (RGBA)

  private:
    std::array<ExtendedColor, static_cast<std::size_t>(SchemeColorRole::Count)>
        _colors;

    static constexpr std::size_t toIndex(SchemeColorRole role) {
        return static_cast<std::size_t>(role);
    }

  public:
    /**
     * @brief Construct an empty scheme.
     */
    Scheme(void) = default;

    /**
     * @brief Construct a scheme from an indexed color array.
     * @param colors Colors indexed by SchemeColorRole.
     */
    explicit Scheme(
        const std::array<ExtendedColor, static_cast<std::size_t>(
                                            SchemeColorRole::Count)> &colors)
        : _colors(colors) {}

    /**
     * @brief Default destructor.
     */
    ~Scheme(void) = default;

    /**
     * @brief Get a scheme color by role.
     * @param role Target role.
     * @return Immutable extended color reference.
     */
    const ExtendedColor &getColor(SchemeColorRole role) const {
        return _colors[toIndex(role)];
    }

    /**
     * @brief Get a mutable scheme color by role.
     * @param role Target role.
     * @return Mutable extended color reference.
     */
    ExtendedColor &getColor(SchemeColorRole role) {
        return _colors[toIndex(role)];
    }

    /**
     * @brief Set a scheme color by role.
     * @param role Target role.
     * @param color Extended color value.
     */
    void setColor(SchemeColorRole role, const ExtendedColor &color) {
        _colors[toIndex(role)] = color;
    }

    /**
     * @brief Set a scheme color by role from raw value and metadata.
     * @param role Target role.
     * @param color Raw RGBA color value.
     * @param name Optional color name.
     * @param description Optional color description.
     * @param harmonized True if harmonized from source color.
     */
    void setColor(SchemeColorRole role, const Color &color,
                  const std::string &name = "",
                  const std::string &description = "",
                  bool harmonized = false) {
        _colors[toIndex(role)] =
            ExtendedColor(color, name, description, harmonized);
    }

    /**
     * @brief Get all colors in role index order.
     * @return Immutable color array reference.
     */
    const auto &getColors(void) const { return _colors; }
};

/**
 * @brief Tonal palette storing tone-to-color mapping for a color family.
 */
class Palette {
  public:
    /**
     * @brief Enumerate the available tones in the palette.
     */
    enum class Tone : std::uint8_t {
        T0 = 0,
        T10 = 10,
        T20 = 20,
        T30 = 30,
        T40 = 40,
        T50 = 50,
        T60 = 60,
        T70 = 70,
        T80 = 80,
        T90 = 90,
        T100 = 100
    };

  private:
    std::string _name;
    std::string _description;
    std::map<Tone, ExtendedColor> _tones;

  public:
    /**
     * @brief Construct an unnamed palette.
     */
    Palette(void) : _name(""), _description(""), _tones() {}

    /**
     * @brief Construct a named palette.
     * @param name Palette name.
     * @param description Palette description.
     */
    Palette(const std::string &name, const std::string &description)
        : _name(name), _description(description), _tones() {}

    /**
     * @brief Default destructor.
     */
    ~Palette(void) = default;

    /**
     * @brief Get palette name.
     * @return Immutable name reference.
     */
    const std::string &getName(void) const { return _name; }

    /**
     * @brief Get palette description.
     * @return Immutable description reference.
     */
    const std::string &getDescription(void) const { return _description; }

    /**
     * @brief Get the full tone map.
     * @return Immutable map from tone to extended color.
     */
    const std::map<Tone, ExtendedColor> &getTones(void) const { return _tones; }

    /**
     * @brief Set palette name.
     * @param name New palette name.
     */
    void setName(const std::string &name) { _name = name; }

    /**
     * @brief Set palette description.
     * @param description New palette description.
     */
    void setDescription(const std::string &description) {
        _description = description;
    }

    /**
     * @brief Set tone using an extended color value.
     * @param tone Tone key.
     * @param color Extended color value.
     */
    void setTone(Tone tone, const ExtendedColor &color) {
        _tones[tone] = color;
    }

    /**
     * @brief Set tone using raw color and metadata.
     * @param tone Tone key.
     * @param color Raw RGBA color value.
     * @param name Optional color name.
     * @param description Optional color description.
     * @param harmonized True if harmonized from source color.
     */
    void setTone(Tone tone, const ExtendedColor::Color &color,
                 const std::string &name = "",
                 const std::string &description = "", bool harmonized = false) {
        _tones[tone] = ExtendedColor(color, name, description, harmonized);
    }

    /**
     * @brief Check if a tone exists in the palette.
     * @param tone Tone key.
     * @return True when tone exists.
     */
    bool hasTone(Tone tone) const { return _tones.find(tone) != _tones.end(); }

    /**
     * @brief Get a tone from the palette.
     * @param tone Tone key.
     * @return Immutable extended color reference for the tone.
     * @throws std::out_of_range When tone does not exist.
     */
    const ExtendedColor &getTone(Tone tone) const {
        const auto iterator = _tones.find(tone);
        if (iterator == _tones.end()) {
            throw std::out_of_range("Requested tone does not exist in palette");
        }
        return iterator->second;
    }

    /**
     * @brief Remove a tone from the palette.
     * @param tone Tone key.
     */
    void removeTone(Tone tone) { _tones.erase(tone); }

    /**
     * @brief Remove all tones from the palette.
     */
    void clearTones(void) { _tones.clear(); }
};

/**
 * @brief Base class for application themes, based on the Material Design
 * guidelines.
 */
class Theme {
  private:
    std::string _name;
    std::string _description;
    Scheme _lightScheme;
    Scheme _lightMediumContrastScheme;
    Scheme _lightHighContrastScheme;
    Scheme _darkScheme;
    Scheme _darkMediumContrastScheme;
    Scheme _darkHighContrastScheme;
    Palette _primaryPalette;
    Palette _secondaryPalette;
    Palette _tertiaryPalette;
    Palette _neutralPalette;
    Palette _neutralVariantPalette;

  public:
    /**
     * @brief Construct an empty theme.
     */
    Theme(void) : _name(""), _description("") {}

    /**
     * @brief Construct a named theme.
     * @param name Theme name.
     * @param description Theme description.
     */
    Theme(const std::string &name, const std::string &description)
        : _name(name), _description(description) {}

    /**
     * @brief Default destructor.
     */
    virtual ~Theme(void) = default;

    /**
     * @brief Get theme name.
     * @return Immutable name reference.
     */
    const std::string &getName(void) const { return _name; }

    /**
     * @brief Get theme description.
     * @return Immutable description reference.
     */
    const std::string &getDescription(void) const { return _description; }

    /**
     * @brief Set theme name.
     * @param name New theme name.
     */
    void setName(const std::string &name) { _name = name; }

    /**
     * @brief Set theme description.
     * @param description New theme description.
     */
    void setDescription(const std::string &description) {
        _description = description;
    }

    /**
     * @brief Get light scheme.
     * @return Immutable light scheme reference.
     */
    const Scheme &getLightScheme(void) const { return _lightScheme; }

    /**
     * @brief Get light medium contrast scheme.
     * @return Immutable scheme reference.
     */
    const Scheme &getLightMediumContrastScheme(void) const {
        return _lightMediumContrastScheme;
    }

    /**
     * @brief Get light high contrast scheme.
     * @return Immutable scheme reference.
     */
    const Scheme &getLightHighContrastScheme(void) const {
        return _lightHighContrastScheme;
    }

    /**
     * @brief Get dark scheme.
     * @return Immutable dark scheme reference.
     */
    const Scheme &getDarkScheme(void) const { return _darkScheme; }

    /**
     * @brief Get dark medium contrast scheme.
     * @return Immutable scheme reference.
     */
    const Scheme &getDarkMediumContrastScheme(void) const {
        return _darkMediumContrastScheme;
    }

    /**
     * @brief Get dark high contrast scheme.
     * @return Immutable scheme reference.
     */
    const Scheme &getDarkHighContrastScheme(void) const {
        return _darkHighContrastScheme;
    }

    /**
     * @brief Get primary palette.
     * @return Immutable palette reference.
     */
    const Palette &getPrimaryPalette(void) const { return _primaryPalette; }

    /**
     * @brief Get secondary palette.
     * @return Immutable palette reference.
     */
    const Palette &getSecondaryPalette(void) const { return _secondaryPalette; }

    /**
     * @brief Get tertiary palette.
     * @return Immutable palette reference.
     */
    const Palette &getTertiaryPalette(void) const { return _tertiaryPalette; }

    /**
     * @brief Get neutral palette.
     * @return Immutable palette reference.
     */
    const Palette &getNeutralPalette(void) const { return _neutralPalette; }

    /**
     * @brief Get neutral variant palette.
     * @return Immutable palette reference.
     */
    const Palette &getNeutralVariantPalette(void) const {
        return _neutralVariantPalette;
    }

    /**
     * @brief Get mutable light scheme.
     * @return Mutable scheme reference.
     */
    Scheme &getLightScheme(void) { return _lightScheme; }

    /**
     * @brief Get mutable light medium contrast scheme.
     * @return Mutable scheme reference.
     */
    Scheme &getLightMediumContrastScheme(void) {
        return _lightMediumContrastScheme;
    }

    /**
     * @brief Get mutable light high contrast scheme.
     * @return Mutable scheme reference.
     */
    Scheme &getLightHighContrastScheme(void) {
        return _lightHighContrastScheme;
    }

    /**
     * @brief Get mutable dark scheme.
     * @return Mutable scheme reference.
     */
    Scheme &getDarkScheme(void) { return _darkScheme; }

    /**
     * @brief Get mutable dark medium contrast scheme.
     * @return Mutable scheme reference.
     */
    Scheme &getDarkMediumContrastScheme(void) {
        return _darkMediumContrastScheme;
    }

    /**
     * @brief Get mutable dark high contrast scheme.
     * @return Mutable scheme reference.
     */
    Scheme &getDarkHighContrastScheme(void) { return _darkHighContrastScheme; }

    /**
     * @brief Get mutable primary palette.
     * @return Mutable palette reference.
     */
    Palette &getPrimaryPalette(void) { return _primaryPalette; }

    /**
     * @brief Get mutable secondary palette.
     * @return Mutable palette reference.
     */
    Palette &getSecondaryPalette(void) { return _secondaryPalette; }

    /**
     * @brief Get mutable tertiary palette.
     * @return Mutable palette reference.
     */
    Palette &getTertiaryPalette(void) { return _tertiaryPalette; }

    /**
     * @brief Get mutable neutral palette.
     * @return Mutable palette reference.
     */
    Palette &getNeutralPalette(void) { return _neutralPalette; }

    /**
     * @brief Get mutable neutral variant palette.
     * @return Mutable palette reference.
     */
    Palette &getNeutralVariantPalette(void) { return _neutralVariantPalette; }

    /**
     * @brief Set light scheme.
     * @param scheme New light scheme.
     */
    void setLightScheme(const Scheme &scheme) { _lightScheme = scheme; }

    /**
     * @brief Set light medium contrast scheme.
     * @param scheme New light medium contrast scheme.
     */
    void setLightMediumContrastScheme(const Scheme &scheme) {
        _lightMediumContrastScheme = scheme;
    }

    /**
     * @brief Set light high contrast scheme.
     * @param scheme New light high contrast scheme.
     */
    void setLightHighContrastScheme(const Scheme &scheme) {
        _lightHighContrastScheme = scheme;
    }

    /**
     * @brief Set dark scheme.
     * @param scheme New dark scheme.
     */
    void setDarkScheme(const Scheme &scheme) { _darkScheme = scheme; }

    /**
     * @brief Set dark medium contrast scheme.
     * @param scheme New dark medium contrast scheme.
     */
    void setDarkMediumContrastScheme(const Scheme &scheme) {
        _darkMediumContrastScheme = scheme;
    }

    /**
     * @brief Set dark high contrast scheme.
     * @param scheme New dark high contrast scheme.
     */
    void setDarkHighContrastScheme(const Scheme &scheme) {
        _darkHighContrastScheme = scheme;
    }

    /**
     * @brief Set primary palette.
     * @param palette New primary palette.
     */
    void setPrimaryPalette(const Palette &palette) {
        _primaryPalette = palette;
    }

    /**
     * @brief Set secondary palette.
     * @param palette New secondary palette.
     */
    void setSecondaryPalette(const Palette &palette) {
        _secondaryPalette = palette;
    }

    /**
     * @brief Set tertiary palette.
     * @param palette New tertiary palette.
     */
    void setTertiaryPalette(const Palette &palette) {
        _tertiaryPalette = palette;
    }

    /**
     * @brief Set neutral palette.
     * @param palette New neutral palette.
     */
    void setNeutralPalette(const Palette &palette) {
        _neutralPalette = palette;
    }

    /**
     * @brief Set neutral variant palette.
     * @param palette New neutral variant palette.
     */
    void setNeutralVariantPalette(const Palette &palette) {
        _neutralVariantPalette = palette;
    }

    /**
     * @brief Get an immutable scheme by role.
     * @param role Scheme role selector.
     * @return Immutable scheme reference.
     */
    const Scheme &getScheme(ThemeSchemeRole role) const {
        switch (role) {
        case ThemeSchemeRole::Light:
            return _lightScheme;
        case ThemeSchemeRole::LightMediumContrast:
            return _lightMediumContrastScheme;
        case ThemeSchemeRole::LightHighContrast:
            return _lightHighContrastScheme;
        case ThemeSchemeRole::Dark:
            return _darkScheme;
        case ThemeSchemeRole::DarkMediumContrast:
            return _darkMediumContrastScheme;
        case ThemeSchemeRole::DarkHighContrast:
            return _darkHighContrastScheme;
        }

        return _lightScheme;
    }

    /**
     * @brief Get a mutable scheme by role.
     * @param role Scheme role selector.
     * @return Mutable scheme reference.
     */
    Scheme &getScheme(ThemeSchemeRole role) {
        switch (role) {
        case ThemeSchemeRole::Light:
            return _lightScheme;
        case ThemeSchemeRole::LightMediumContrast:
            return _lightMediumContrastScheme;
        case ThemeSchemeRole::LightHighContrast:
            return _lightHighContrastScheme;
        case ThemeSchemeRole::Dark:
            return _darkScheme;
        case ThemeSchemeRole::DarkMediumContrast:
            return _darkMediumContrastScheme;
        case ThemeSchemeRole::DarkHighContrast:
            return _darkHighContrastScheme;
        }

        return _lightScheme;
    }

    /**
     * @brief Get an immutable palette by role.
     * @param role Palette role selector.
     * @return Immutable palette reference.
     */
    const Palette &getPalette(ThemePaletteRole role) const {
        switch (role) {
        case ThemePaletteRole::Primary:
            return _primaryPalette;
        case ThemePaletteRole::Secondary:
            return _secondaryPalette;
        case ThemePaletteRole::Tertiary:
            return _tertiaryPalette;
        case ThemePaletteRole::Neutral:
            return _neutralPalette;
        case ThemePaletteRole::NeutralVariant:
            return _neutralVariantPalette;
        }

        return _primaryPalette;
    }

    /**
     * @brief Get a mutable palette by role.
     * @param role Palette role selector.
     * @return Mutable palette reference.
     */
    Palette &getPalette(ThemePaletteRole role) {
        switch (role) {
        case ThemePaletteRole::Primary:
            return _primaryPalette;
        case ThemePaletteRole::Secondary:
            return _secondaryPalette;
        case ThemePaletteRole::Tertiary:
            return _tertiaryPalette;
        case ThemePaletteRole::Neutral:
            return _neutralPalette;
        case ThemePaletteRole::NeutralVariant:
            return _neutralVariantPalette;
        }

        return _primaryPalette;
    }
};

extern const Theme defaultTheme;

} // namespace guillaume
