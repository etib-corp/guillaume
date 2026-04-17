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

#include "guillaume/theme.hpp"

namespace guillaume
{
	ExtendedColor::ExtendedColor(void)
		: _color()
		, _name("")
		, _description("")
		, _harmonized(false)
	{
	}

	ExtendedColor::ExtendedColor(const utility::graphic::Color32Bit &color,
							   const std::string &name,
							   const std::string &description,
							   bool harmonized)
		: _color(color)
		, _name(name)
		, _description(description)
		, _harmonized(harmonized)
	{
	}

	const utility::graphic::Color32Bit &ExtendedColor::getColor(void) const
	{
		return _color;
	}

	const std::string &ExtendedColor::getName(void) const
	{
		return _name;
	}

	const std::string &ExtendedColor::getDescription(void) const
	{
		return _description;
	}

	bool ExtendedColor::isHarmonized(void) const
	{
		return _harmonized;
	}

	void ExtendedColor::setColor(const utility::graphic::Color32Bit &color)
	{
		_color = color;
	}

	void ExtendedColor::setName(const std::string &name)
	{
		_name = name;
	}

	void ExtendedColor::setDescription(const std::string &description)
	{
		_description = description;
	}

	void ExtendedColor::setHarmonized(bool harmonized)
	{
		_harmonized = harmonized;
	}

	Scheme::Scheme(
		const std::array<ExtendedColor,
						 static_cast<std::size_t>(SchemeColorRole::Count)> &colors)
		: _colors(colors)
	{
	}

	const ExtendedColor &Scheme::getColor(SchemeColorRole role) const
	{
		return _colors[toIndex(role)];
	}

	ExtendedColor &Scheme::getColor(SchemeColorRole role)
	{
		return _colors[toIndex(role)];
	}

	void Scheme::setColor(SchemeColorRole role, const ExtendedColor &color)
	{
		_colors[toIndex(role)] = color;
	}

	void Scheme::setColor(SchemeColorRole role,
					 const utility::graphic::Color32Bit &color,
					 const std::string &name,
					 const std::string &description,
					 bool harmonized)
	{
		_colors[toIndex(role)] =
			ExtendedColor(color, name, description, harmonized);
	}

	const auto &Scheme::getColors(void) const
	{
		return _colors;
	}

	Palette::Palette(void)
		: _name("")
		, _description("")
		, _tones()
	{
	}

	Palette::Palette(const std::string &name, const std::string &description)
		: _name(name)
		, _description(description)
		, _tones()
	{
	}

	const std::string &Palette::getName(void) const
	{
		return _name;
	}

	const std::string &Palette::getDescription(void) const
	{
		return _description;
	}

	const std::map<Palette::Tone, ExtendedColor> &Palette::getTones(void) const
	{
		return _tones;
	}

	void Palette::setName(const std::string &name)
	{
		_name = name;
	}

	void Palette::setDescription(const std::string &description)
	{
		_description = description;
	}

	void Palette::setTone(Tone tone, const ExtendedColor &color)
	{
		_tones[tone] = color;
	}

	void Palette::setTone(Tone tone,
					 const utility::graphic::Color32Bit &color,
					 const std::string &name,
					 const std::string &description,
					 bool harmonized)
	{
		_tones[tone] = ExtendedColor(color, name, description, harmonized);
	}

	bool Palette::hasTone(Tone tone) const
	{
		return _tones.find(tone) != _tones.end();
	}

	const ExtendedColor &Palette::getTone(Tone tone) const
	{
		const auto iterator = _tones.find(tone);
		if (iterator == _tones.end()) {
			throw std::out_of_range("Requested tone does not exist in palette");
		}
		return iterator->second;
	}

	void Palette::removeTone(Tone tone)
	{
		_tones.erase(tone);
	}

	void Palette::clearTones(void)
	{
		_tones.clear();
	}

	Theme::Theme(void)
		: _name("")
		, _description("")
	{
	}

	Theme::Theme(const std::string &name, const std::string &description)
		: _name(name)
		, _description(description)
	{
	}

	const std::string &Theme::getName(void) const
	{
		return _name;
	}

	const std::string &Theme::getDescription(void) const
	{
		return _description;
	}

	void Theme::setName(const std::string &name)
	{
		_name = name;
	}

	void Theme::setDescription(const std::string &description)
	{
		_description = description;
	}

	const Scheme &Theme::getLightScheme(void) const
	{
		return _lightScheme;
	}

	const Scheme &Theme::getLightMediumContrastScheme(void) const
	{
		return _lightMediumContrastScheme;
	}

	const Scheme &Theme::getLightHighContrastScheme(void) const
	{
		return _lightHighContrastScheme;
	}

	const Scheme &Theme::getDarkScheme(void) const
	{
		return _darkScheme;
	}

	const Scheme &Theme::getDarkMediumContrastScheme(void) const
	{
		return _darkMediumContrastScheme;
	}

	const Scheme &Theme::getDarkHighContrastScheme(void) const
	{
		return _darkHighContrastScheme;
	}

	const Palette &Theme::getPrimaryPalette(void) const
	{
		return _primaryPalette;
	}

	const Palette &Theme::getSecondaryPalette(void) const
	{
		return _secondaryPalette;
	}

	const Palette &Theme::getTertiaryPalette(void) const
	{
		return _tertiaryPalette;
	}

	const Palette &Theme::getNeutralPalette(void) const
	{
		return _neutralPalette;
	}

	const Palette &Theme::getNeutralVariantPalette(void) const
	{
		return _neutralVariantPalette;
	}

	Scheme &Theme::getLightScheme(void)
	{
		return _lightScheme;
	}

	Scheme &Theme::getLightMediumContrastScheme(void)
	{
		return _lightMediumContrastScheme;
	}

	Scheme &Theme::getLightHighContrastScheme(void)
	{
		return _lightHighContrastScheme;
	}

	Scheme &Theme::getDarkScheme(void)
	{
		return _darkScheme;
	}

	Scheme &Theme::getDarkMediumContrastScheme(void)
	{
		return _darkMediumContrastScheme;
	}

	Scheme &Theme::getDarkHighContrastScheme(void)
	{
		return _darkHighContrastScheme;
	}

	Palette &Theme::getPrimaryPalette(void)
	{
		return _primaryPalette;
	}

	Palette &Theme::getSecondaryPalette(void)
	{
		return _secondaryPalette;
	}

	Palette &Theme::getTertiaryPalette(void)
	{
		return _tertiaryPalette;
	}

	Palette &Theme::getNeutralPalette(void)
	{
		return _neutralPalette;
	}

	Palette &Theme::getNeutralVariantPalette(void)
	{
		return _neutralVariantPalette;
	}

	void Theme::setLightScheme(const Scheme &scheme)
	{
		_lightScheme = scheme;
	}

	void Theme::setLightMediumContrastScheme(const Scheme &scheme)
	{
		_lightMediumContrastScheme = scheme;
	}

	void Theme::setLightHighContrastScheme(const Scheme &scheme)
	{
		_lightHighContrastScheme = scheme;
	}

	void Theme::setDarkScheme(const Scheme &scheme)
	{
		_darkScheme = scheme;
	}

	void Theme::setDarkMediumContrastScheme(const Scheme &scheme)
	{
		_darkMediumContrastScheme = scheme;
	}

	void Theme::setDarkHighContrastScheme(const Scheme &scheme)
	{
		_darkHighContrastScheme = scheme;
	}

	void Theme::setPrimaryPalette(const Palette &palette)
	{
		_primaryPalette = palette;
	}

	void Theme::setSecondaryPalette(const Palette &palette)
	{
		_secondaryPalette = palette;
	}

	void Theme::setTertiaryPalette(const Palette &palette)
	{
		_tertiaryPalette = palette;
	}

	void Theme::setNeutralPalette(const Palette &palette)
	{
		_neutralPalette = palette;
	}

	void Theme::setNeutralVariantPalette(const Palette &palette)
	{
		_neutralVariantPalette = palette;
	}

	const Scheme &Theme::getScheme(ThemeSchemeRole role) const
	{
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

	Scheme &Theme::getScheme(ThemeSchemeRole role)
	{
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

	const Palette &Theme::getPalette(ThemePaletteRole role) const
	{
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

	Palette &Theme::getPalette(ThemePaletteRole role)
	{
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

	namespace
	{
		/**
		 * @brief Initialize light color scheme.
		 */
		void initializeLightScheme(Scheme &scheme)
		{
			scheme.setColor(SchemeColorRole::Primary,
							utility::graphic::Color32Bit(143, 76, 56, 255),
							"primary");
			scheme.setColor(SchemeColorRole::SurfaceTint,
							utility::graphic::Color32Bit(143, 76, 56, 255),
							"surfaceTint");
			scheme.setColor(SchemeColorRole::OnPrimary,
							utility::graphic::Color32Bit(255, 255, 255, 255),
							"onPrimary");
			scheme.setColor(SchemeColorRole::PrimaryContainer,
							utility::graphic::Color32Bit(255, 219, 209, 255),
							"primaryContainer");
			scheme.setColor(SchemeColorRole::OnPrimaryContainer,
							utility::graphic::Color32Bit(114, 53, 35, 255),
							"onPrimaryContainer");
			scheme.setColor(SchemeColorRole::Secondary,
							utility::graphic::Color32Bit(119, 87, 78, 255),
							"secondary");
			scheme.setColor(SchemeColorRole::OnSecondary,
							utility::graphic::Color32Bit(255, 255, 255, 255),
							"onSecondary");
			scheme.setColor(SchemeColorRole::SecondaryContainer,
							utility::graphic::Color32Bit(255, 219, 209, 255),
							"secondaryContainer");
			scheme.setColor(SchemeColorRole::OnSecondaryContainer,
							utility::graphic::Color32Bit(93, 64, 55, 255),
							"onSecondaryContainer");
			scheme.setColor(SchemeColorRole::Tertiary,
							utility::graphic::Color32Bit(108, 93, 47, 255),
							"tertiary");
			scheme.setColor(SchemeColorRole::OnTertiary,
							utility::graphic::Color32Bit(255, 255, 255, 255),
							"onTertiary");
			scheme.setColor(SchemeColorRole::TertiaryContainer,
							utility::graphic::Color32Bit(245, 225, 167, 255),
							"tertiaryContainer");
			scheme.setColor(SchemeColorRole::OnTertiaryContainer,
							utility::graphic::Color32Bit(83, 70, 25, 255),
							"onTertiaryContainer");
			scheme.setColor(SchemeColorRole::Error,
							utility::graphic::Color32Bit(186, 26, 26, 255),
							"error");
			scheme.setColor(SchemeColorRole::OnError,
							utility::graphic::Color32Bit(255, 255, 255, 255),
							"onError");
			scheme.setColor(SchemeColorRole::ErrorContainer,
							utility::graphic::Color32Bit(255, 218, 214, 255),
							"errorContainer");
			scheme.setColor(SchemeColorRole::OnErrorContainer,
							utility::graphic::Color32Bit(147, 0, 10, 255),
							"onErrorContainer");
			scheme.setColor(SchemeColorRole::Background,
							utility::graphic::Color32Bit(255, 248, 246, 255),
							"background");
			scheme.setColor(SchemeColorRole::OnBackground,
							utility::graphic::Color32Bit(35, 25, 23, 255),
							"onBackground");
			scheme.setColor(SchemeColorRole::Surface,
							utility::graphic::Color32Bit(255, 248, 246, 255),
							"surface");
			scheme.setColor(SchemeColorRole::OnSurface,
							utility::graphic::Color32Bit(35, 25, 23, 255),
							"onSurface");
			scheme.setColor(SchemeColorRole::SurfaceVariant,
							utility::graphic::Color32Bit(245, 222, 216, 255),
							"surfaceVariant");
			scheme.setColor(SchemeColorRole::OnSurfaceVariant,
							utility::graphic::Color32Bit(83, 67, 63, 255),
							"onSurfaceVariant");
			scheme.setColor(SchemeColorRole::Outline,
							utility::graphic::Color32Bit(133, 115, 110, 255),
							"outline");
			scheme.setColor(SchemeColorRole::OutlineVariant,
							utility::graphic::Color32Bit(216, 194, 188, 255),
							"outlineVariant");
			scheme.setColor(SchemeColorRole::Shadow,
							utility::graphic::Color32Bit(0, 0, 0, 255),
							"shadow");
			scheme.setColor(SchemeColorRole::Scrim,
							utility::graphic::Color32Bit(0, 0, 0, 255),
							"scrim");
			scheme.setColor(SchemeColorRole::InverseSurface,
							utility::graphic::Color32Bit(57, 46, 43, 255),
							"inverseSurface");
			scheme.setColor(SchemeColorRole::InverseOnSurface,
							utility::graphic::Color32Bit(255, 237, 232, 255),
							"inverseOnSurface");
			scheme.setColor(SchemeColorRole::InversePrimary,
							utility::graphic::Color32Bit(255, 181, 160, 255),
							"inversePrimary");
			scheme.setColor(SchemeColorRole::PrimaryFixed,
							utility::graphic::Color32Bit(255, 219, 209, 255),
							"primaryFixed");
			scheme.setColor(SchemeColorRole::OnPrimaryFixed,
							utility::graphic::Color32Bit(58, 11, 1, 255),
							"onPrimaryFixed");
			scheme.setColor(SchemeColorRole::PrimaryFixedDim,
							utility::graphic::Color32Bit(255, 181, 160, 255),
							"primaryFixedDim");
			scheme.setColor(SchemeColorRole::OnPrimaryFixedVariant,
							utility::graphic::Color32Bit(114, 53, 35, 255),
							"onPrimaryFixedVariant");
			scheme.setColor(SchemeColorRole::SecondaryFixed,
							utility::graphic::Color32Bit(255, 219, 209, 255),
							"secondaryFixed");
			scheme.setColor(SchemeColorRole::OnSecondaryFixed,
							utility::graphic::Color32Bit(44, 21, 15, 255),
							"onSecondaryFixed");
			scheme.setColor(SchemeColorRole::SecondaryFixedDim,
							utility::graphic::Color32Bit(231, 189, 178, 255),
							"secondaryFixedDim");
			scheme.setColor(SchemeColorRole::OnSecondaryFixedVariant,
							utility::graphic::Color32Bit(93, 64, 55, 255),
							"onSecondaryFixedVariant");
			scheme.setColor(SchemeColorRole::TertiaryFixed,
							utility::graphic::Color32Bit(245, 225, 167, 255),
							"tertiaryFixed");
			scheme.setColor(SchemeColorRole::OnTertiaryFixed,
							utility::graphic::Color32Bit(35, 27, 0, 255),
							"onTertiaryFixed");
			scheme.setColor(SchemeColorRole::TertiaryFixedDim,
							utility::graphic::Color32Bit(216, 197, 141, 255),
							"tertiaryFixedDim");
			scheme.setColor(SchemeColorRole::OnTertiaryFixedVariant,
							utility::graphic::Color32Bit(83, 70, 25, 255),
							"onTertiaryFixedVariant");
			scheme.setColor(SchemeColorRole::SurfaceDim,
							utility::graphic::Color32Bit(232, 214, 210, 255),
							"surfaceDim");
			scheme.setColor(SchemeColorRole::SurfaceBright,
							utility::graphic::Color32Bit(255, 248, 246, 255),
							"surfaceBright");
			scheme.setColor(SchemeColorRole::SurfaceContainerLowest,
							utility::graphic::Color32Bit(255, 255, 255, 255),
							"surfaceContainerLowest");
			scheme.setColor(SchemeColorRole::SurfaceContainerLow,
							utility::graphic::Color32Bit(255, 241, 237, 255),
							"surfaceContainerLow");
			scheme.setColor(SchemeColorRole::SurfaceContainer,
							utility::graphic::Color32Bit(252, 234, 229, 255),
							"surfaceContainer");
			scheme.setColor(SchemeColorRole::SurfaceContainerHigh,
							utility::graphic::Color32Bit(247, 228, 224, 255),
							"surfaceContainerHigh");
			scheme.setColor(SchemeColorRole::SurfaceContainerHighest,
							utility::graphic::Color32Bit(241, 223, 218, 255),
							"surfaceContainerHighest");
		}

		/**
		 * @brief Initialize light medium contrast scheme.
		 */
		void initializeLightMediumContrastScheme(Scheme &scheme)
		{
			scheme.setColor(SchemeColorRole::Primary,
							utility::graphic::Color32Bit(93, 37, 20, 255),
							"primary");
			scheme.setColor(SchemeColorRole::SurfaceTint,
							utility::graphic::Color32Bit(143, 76, 56, 255),
							"surfaceTint");
			scheme.setColor(SchemeColorRole::OnPrimary,
							utility::graphic::Color32Bit(255, 255, 255, 255),
							"onPrimary");
			scheme.setColor(SchemeColorRole::PrimaryContainer,
							utility::graphic::Color32Bit(161, 90, 69, 255),
							"primaryContainer");
			scheme.setColor(SchemeColorRole::OnPrimaryContainer,
							utility::graphic::Color32Bit(255, 255, 255, 255),
							"onPrimaryContainer");
			scheme.setColor(SchemeColorRole::Secondary,
							utility::graphic::Color32Bit(75, 47, 40, 255),
							"secondary");
			scheme.setColor(SchemeColorRole::OnSecondary,
							utility::graphic::Color32Bit(255, 255, 255, 255),
							"onSecondary");
			scheme.setColor(SchemeColorRole::SecondaryContainer,
							utility::graphic::Color32Bit(135, 101, 92, 255),
							"secondaryContainer");
			scheme.setColor(SchemeColorRole::OnSecondaryContainer,
							utility::graphic::Color32Bit(255, 255, 255, 255),
							"onSecondaryContainer");
			scheme.setColor(SchemeColorRole::Tertiary,
							utility::graphic::Color32Bit(65, 53, 10, 255),
							"tertiary");
			scheme.setColor(SchemeColorRole::OnTertiary,
							utility::graphic::Color32Bit(255, 255, 255, 255),
							"onTertiary");
			scheme.setColor(SchemeColorRole::TertiaryContainer,
							utility::graphic::Color32Bit(123, 108, 60, 255),
							"tertiaryContainer");
			scheme.setColor(SchemeColorRole::OnTertiaryContainer,
							utility::graphic::Color32Bit(255, 255, 255, 255),
							"onTertiaryContainer");
			scheme.setColor(SchemeColorRole::Error,
							utility::graphic::Color32Bit(116, 0, 6, 255),
							"error");
			scheme.setColor(SchemeColorRole::OnError,
							utility::graphic::Color32Bit(255, 255, 255, 255),
							"onError");
			scheme.setColor(SchemeColorRole::ErrorContainer,
							utility::graphic::Color32Bit(207, 44, 39, 255),
							"errorContainer");
			scheme.setColor(SchemeColorRole::OnErrorContainer,
							utility::graphic::Color32Bit(255, 255, 255, 255),
							"onErrorContainer");
			scheme.setColor(SchemeColorRole::Background,
							utility::graphic::Color32Bit(255, 248, 246, 255),
							"background");
			scheme.setColor(SchemeColorRole::OnBackground,
							utility::graphic::Color32Bit(35, 25, 23, 255),
							"onBackground");
			scheme.setColor(SchemeColorRole::Surface,
							utility::graphic::Color32Bit(255, 248, 246, 255),
							"surface");
			scheme.setColor(SchemeColorRole::OnSurface,
							utility::graphic::Color32Bit(24, 15, 13, 255),
							"onSurface");
			scheme.setColor(SchemeColorRole::SurfaceVariant,
							utility::graphic::Color32Bit(245, 222, 216, 255),
							"surfaceVariant");
			scheme.setColor(SchemeColorRole::OnSurfaceVariant,
							utility::graphic::Color32Bit(65, 51, 47, 255),
							"onSurfaceVariant");
			scheme.setColor(SchemeColorRole::Outline,
							utility::graphic::Color32Bit(95, 79, 74, 255),
							"outline");
			scheme.setColor(SchemeColorRole::OutlineVariant,
							utility::graphic::Color32Bit(123, 105, 100, 255),
							"outlineVariant");
			scheme.setColor(SchemeColorRole::Shadow,
							utility::graphic::Color32Bit(0, 0, 0, 255),
							"shadow");
			scheme.setColor(SchemeColorRole::Scrim,
							utility::graphic::Color32Bit(0, 0, 0, 255),
							"scrim");
			scheme.setColor(SchemeColorRole::InverseSurface,
							utility::graphic::Color32Bit(57, 46, 43, 255),
							"inverseSurface");
			scheme.setColor(SchemeColorRole::InverseOnSurface,
							utility::graphic::Color32Bit(255, 237, 232, 255),
							"inverseOnSurface");
			scheme.setColor(SchemeColorRole::InversePrimary,
							utility::graphic::Color32Bit(255, 181, 160, 255),
							"inversePrimary");
			scheme.setColor(SchemeColorRole::PrimaryFixed,
							utility::graphic::Color32Bit(161, 90, 69, 255),
							"primaryFixed");
			scheme.setColor(SchemeColorRole::OnPrimaryFixed,
							utility::graphic::Color32Bit(255, 255, 255, 255),
							"onPrimaryFixed");
			scheme.setColor(SchemeColorRole::PrimaryFixedDim,
							utility::graphic::Color32Bit(132, 66, 47, 255),
							"primaryFixedDim");
			scheme.setColor(SchemeColorRole::OnPrimaryFixedVariant,
							utility::graphic::Color32Bit(255, 255, 255, 255),
							"onPrimaryFixedVariant");
			scheme.setColor(SchemeColorRole::SecondaryFixed,
							utility::graphic::Color32Bit(135, 101, 92, 255),
							"secondaryFixed");
			scheme.setColor(SchemeColorRole::OnSecondaryFixed,
							utility::graphic::Color32Bit(255, 255, 255, 255),
							"onSecondaryFixed");
			scheme.setColor(SchemeColorRole::SecondaryFixedDim,
							utility::graphic::Color32Bit(109, 77, 69, 255),
							"secondaryFixedDim");
			scheme.setColor(SchemeColorRole::OnSecondaryFixedVariant,
							utility::graphic::Color32Bit(255, 255, 255, 255),
							"onSecondaryFixedVariant");
			scheme.setColor(SchemeColorRole::TertiaryFixed,
							utility::graphic::Color32Bit(123, 108, 60, 255),
							"tertiaryFixed");
			scheme.setColor(SchemeColorRole::OnTertiaryFixed,
							utility::graphic::Color32Bit(255, 255, 255, 255),
							"onTertiaryFixed");
			scheme.setColor(SchemeColorRole::TertiaryFixedDim,
							utility::graphic::Color32Bit(97, 84, 38, 255),
							"tertiaryFixedDim");
			scheme.setColor(SchemeColorRole::OnTertiaryFixedVariant,
							utility::graphic::Color32Bit(255, 255, 255, 255),
							"onTertiaryFixedVariant");
			scheme.setColor(SchemeColorRole::SurfaceDim,
							utility::graphic::Color32Bit(212, 195, 190, 255),
							"surfaceDim");
			scheme.setColor(SchemeColorRole::SurfaceBright,
							utility::graphic::Color32Bit(255, 248, 246, 255),
							"surfaceBright");
			scheme.setColor(SchemeColorRole::SurfaceContainerLowest,
							utility::graphic::Color32Bit(255, 255, 255, 255),
							"surfaceContainerLowest");
			scheme.setColor(SchemeColorRole::SurfaceContainerLow,
							utility::graphic::Color32Bit(255, 241, 237, 255),
							"surfaceContainerLow");
			scheme.setColor(SchemeColorRole::SurfaceContainer,
							utility::graphic::Color32Bit(247, 228, 224, 255),
							"surfaceContainer");
			scheme.setColor(SchemeColorRole::SurfaceContainerHigh,
							utility::graphic::Color32Bit(235, 217, 212, 255),
							"surfaceContainerHigh");
			scheme.setColor(SchemeColorRole::SurfaceContainerHighest,
							utility::graphic::Color32Bit(223, 206, 201, 255),
							"surfaceContainerHighest");
		}

		/**
		 * @brief Initialize light high contrast scheme.
		 */
		void initializeLightHighContrastScheme(Scheme &scheme)
		{
			scheme.setColor(SchemeColorRole::Primary,
							utility::graphic::Color32Bit(80, 27, 11, 255),
							"primary");
			scheme.setColor(SchemeColorRole::SurfaceTint,
							utility::graphic::Color32Bit(143, 76, 56, 255),
							"surfaceTint");
			scheme.setColor(SchemeColorRole::OnPrimary,
							utility::graphic::Color32Bit(255, 255, 255, 255),
							"onPrimary");
			scheme.setColor(SchemeColorRole::PrimaryContainer,
							utility::graphic::Color32Bit(117, 55, 37, 255),
							"primaryContainer");
			scheme.setColor(SchemeColorRole::OnPrimaryContainer,
							utility::graphic::Color32Bit(255, 255, 255, 255),
							"onPrimaryContainer");
			scheme.setColor(SchemeColorRole::Secondary,
							utility::graphic::Color32Bit(63, 38, 30, 255),
							"secondary");
			scheme.setColor(SchemeColorRole::OnSecondary,
							utility::graphic::Color32Bit(255, 255, 255, 255),
							"onSecondary");
			scheme.setColor(SchemeColorRole::SecondaryContainer,
							utility::graphic::Color32Bit(96, 66, 58, 255),
							"secondaryContainer");
			scheme.setColor(SchemeColorRole::OnSecondaryContainer,
							utility::graphic::Color32Bit(255, 255, 255, 255),
							"onSecondaryContainer");
			scheme.setColor(SchemeColorRole::Tertiary,
							utility::graphic::Color32Bit(54, 43, 2, 255),
							"tertiary");
			scheme.setColor(SchemeColorRole::OnTertiary,
							utility::graphic::Color32Bit(255, 255, 255, 255),
							"onTertiary");
			scheme.setColor(SchemeColorRole::TertiaryContainer,
							utility::graphic::Color32Bit(85, 72, 28, 255),
							"tertiaryContainer");
			scheme.setColor(SchemeColorRole::OnTertiaryContainer,
							utility::graphic::Color32Bit(255, 255, 255, 255),
							"onTertiaryContainer");
			scheme.setColor(SchemeColorRole::Error,
							utility::graphic::Color32Bit(96, 0, 4, 255),
							"error");
			scheme.setColor(SchemeColorRole::OnError,
							utility::graphic::Color32Bit(255, 255, 255, 255),
							"onError");
			scheme.setColor(SchemeColorRole::ErrorContainer,
							utility::graphic::Color32Bit(152, 0, 10, 255),
							"errorContainer");
			scheme.setColor(SchemeColorRole::OnErrorContainer,
							utility::graphic::Color32Bit(255, 255, 255, 255),
							"onErrorContainer");
			scheme.setColor(SchemeColorRole::Background,
							utility::graphic::Color32Bit(255, 248, 246, 255),
							"background");
			scheme.setColor(SchemeColorRole::OnBackground,
							utility::graphic::Color32Bit(35, 25, 23, 255),
							"onBackground");
			scheme.setColor(SchemeColorRole::Surface,
							utility::graphic::Color32Bit(255, 248, 246, 255),
							"surface");
			scheme.setColor(SchemeColorRole::OnSurface,
							utility::graphic::Color32Bit(0, 0, 0, 255),
							"onSurface");
			scheme.setColor(SchemeColorRole::SurfaceVariant,
							utility::graphic::Color32Bit(245, 222, 216, 255),
							"surfaceVariant");
			scheme.setColor(SchemeColorRole::OnSurfaceVariant,
							utility::graphic::Color32Bit(0, 0, 0, 255),
							"onSurfaceVariant");
			scheme.setColor(SchemeColorRole::Outline,
							utility::graphic::Color32Bit(55, 41, 37, 255),
							"outline");
			scheme.setColor(SchemeColorRole::OutlineVariant,
							utility::graphic::Color32Bit(85, 70, 65, 255),
							"outlineVariant");
			scheme.setColor(SchemeColorRole::Shadow,
							utility::graphic::Color32Bit(0, 0, 0, 255),
							"shadow");
			scheme.setColor(SchemeColorRole::Scrim,
							utility::graphic::Color32Bit(0, 0, 0, 255),
							"scrim");
			scheme.setColor(SchemeColorRole::InverseSurface,
							utility::graphic::Color32Bit(57, 46, 43, 255),
							"inverseSurface");
			scheme.setColor(SchemeColorRole::InverseOnSurface,
							utility::graphic::Color32Bit(255, 255, 255, 255),
							"inverseOnSurface");
			scheme.setColor(SchemeColorRole::InversePrimary,
							utility::graphic::Color32Bit(255, 181, 160, 255),
							"inversePrimary");
			scheme.setColor(SchemeColorRole::PrimaryFixed,
							utility::graphic::Color32Bit(117, 55, 37, 255),
							"primaryFixed");
			scheme.setColor(SchemeColorRole::OnPrimaryFixed,
							utility::graphic::Color32Bit(255, 255, 255, 255),
							"onPrimaryFixed");
			scheme.setColor(SchemeColorRole::PrimaryFixedDim,
							utility::graphic::Color32Bit(89, 33, 17, 255),
							"primaryFixedDim");
			scheme.setColor(SchemeColorRole::OnPrimaryFixedVariant,
							utility::graphic::Color32Bit(255, 255, 255, 255),
							"onPrimaryFixedVariant");
			scheme.setColor(SchemeColorRole::SecondaryFixed,
							utility::graphic::Color32Bit(96, 66, 58, 255),
							"secondaryFixed");
			scheme.setColor(SchemeColorRole::OnSecondaryFixed,
							utility::graphic::Color32Bit(255, 255, 255, 255),
							"onSecondaryFixed");
			scheme.setColor(SchemeColorRole::SecondaryFixedDim,
							utility::graphic::Color32Bit(71, 44, 36, 255),
							"secondaryFixedDim");
			scheme.setColor(SchemeColorRole::OnSecondaryFixedVariant,
							utility::graphic::Color32Bit(255, 255, 255, 255),
							"onSecondaryFixedVariant");
			scheme.setColor(SchemeColorRole::TertiaryFixed,
							utility::graphic::Color32Bit(85, 72, 28, 255),
							"tertiaryFixed");
			scheme.setColor(SchemeColorRole::OnTertiaryFixed,
							utility::graphic::Color32Bit(255, 255, 255, 255),
							"onTertiaryFixed");
			scheme.setColor(SchemeColorRole::TertiaryFixedDim,
							utility::graphic::Color32Bit(61, 50, 6, 255),
							"tertiaryFixedDim");
			scheme.setColor(SchemeColorRole::OnTertiaryFixedVariant,
							utility::graphic::Color32Bit(255, 255, 255, 255),
							"onTertiaryFixedVariant");
			scheme.setColor(SchemeColorRole::SurfaceDim,
							utility::graphic::Color32Bit(198, 181, 177, 255),
							"surfaceDim");
			scheme.setColor(SchemeColorRole::SurfaceBright,
							utility::graphic::Color32Bit(255, 248, 246, 255),
							"surfaceBright");
			scheme.setColor(SchemeColorRole::SurfaceContainerLowest,
							utility::graphic::Color32Bit(255, 255, 255, 255),
							"surfaceContainerLowest");
			scheme.setColor(SchemeColorRole::SurfaceContainerLow,
							utility::graphic::Color32Bit(255, 237, 232, 255),
							"surfaceContainerLow");
			scheme.setColor(SchemeColorRole::SurfaceContainer,
							utility::graphic::Color32Bit(241, 223, 218, 255),
							"surfaceContainer");
			scheme.setColor(SchemeColorRole::SurfaceContainerHigh,
							utility::graphic::Color32Bit(226, 209, 204, 255),
							"surfaceContainerHigh");
			scheme.setColor(SchemeColorRole::SurfaceContainerHighest,
							utility::graphic::Color32Bit(212, 195, 190, 255),
							"surfaceContainerHighest");
		}

		/**
		 * @brief Initialize dark color scheme.
		 */
		void initializeDarkScheme(Scheme &scheme)
		{
			scheme.setColor(SchemeColorRole::Primary,
							utility::graphic::Color32Bit(255, 181, 160, 255),
							"primary");
			scheme.setColor(SchemeColorRole::SurfaceTint,
							utility::graphic::Color32Bit(255, 181, 160, 255),
							"surfaceTint");
			scheme.setColor(SchemeColorRole::OnPrimary,
							utility::graphic::Color32Bit(86, 31, 15, 255),
							"onPrimary");
			scheme.setColor(SchemeColorRole::PrimaryContainer,
							utility::graphic::Color32Bit(114, 53, 35, 255),
							"primaryContainer");
			scheme.setColor(SchemeColorRole::OnPrimaryContainer,
							utility::graphic::Color32Bit(255, 219, 209, 255),
							"onPrimaryContainer");
			scheme.setColor(SchemeColorRole::Secondary,
							utility::graphic::Color32Bit(231, 189, 178, 255),
							"secondary");
			scheme.setColor(SchemeColorRole::OnSecondary,
							utility::graphic::Color32Bit(68, 42, 34, 255),
							"onSecondary");
			scheme.setColor(SchemeColorRole::SecondaryContainer,
							utility::graphic::Color32Bit(93, 64, 55, 255),
							"secondaryContainer");
			scheme.setColor(SchemeColorRole::OnSecondaryContainer,
							utility::graphic::Color32Bit(255, 219, 209, 255),
							"onSecondaryContainer");
			scheme.setColor(SchemeColorRole::Tertiary,
							utility::graphic::Color32Bit(216, 197, 141, 255),
							"tertiary");
			scheme.setColor(SchemeColorRole::OnTertiary,
							utility::graphic::Color32Bit(59, 47, 5, 255),
							"onTertiary");
			scheme.setColor(SchemeColorRole::TertiaryContainer,
							utility::graphic::Color32Bit(83, 70, 25, 255),
							"tertiaryContainer");
			scheme.setColor(SchemeColorRole::OnTertiaryContainer,
							utility::graphic::Color32Bit(245, 225, 167, 255),
							"onTertiaryContainer");
			scheme.setColor(SchemeColorRole::Error,
							utility::graphic::Color32Bit(255, 180, 171, 255),
							"error");
			scheme.setColor(SchemeColorRole::OnError,
							utility::graphic::Color32Bit(105, 0, 5, 255),
							"onError");
			scheme.setColor(SchemeColorRole::ErrorContainer,
							utility::graphic::Color32Bit(147, 0, 10, 255),
							"errorContainer");
			scheme.setColor(SchemeColorRole::OnErrorContainer,
							utility::graphic::Color32Bit(255, 218, 214, 255),
							"onErrorContainer");
			scheme.setColor(SchemeColorRole::Background,
							utility::graphic::Color32Bit(26, 17, 15, 255),
							"background");
			scheme.setColor(SchemeColorRole::OnBackground,
							utility::graphic::Color32Bit(241, 223, 218, 255),
							"onBackground");
			scheme.setColor(SchemeColorRole::Surface,
							utility::graphic::Color32Bit(26, 17, 15, 255),
							"surface");
			scheme.setColor(SchemeColorRole::OnSurface,
							utility::graphic::Color32Bit(241, 223, 218, 255),
							"onSurface");
			scheme.setColor(SchemeColorRole::SurfaceVariant,
							utility::graphic::Color32Bit(83, 67, 63, 255),
							"surfaceVariant");
			scheme.setColor(SchemeColorRole::OnSurfaceVariant,
							utility::graphic::Color32Bit(216, 194, 188, 255),
							"onSurfaceVariant");
			scheme.setColor(SchemeColorRole::Outline,
							utility::graphic::Color32Bit(160, 140, 135, 255),
							"outline");
			scheme.setColor(SchemeColorRole::OutlineVariant,
							utility::graphic::Color32Bit(83, 67, 63, 255),
							"outlineVariant");
			scheme.setColor(SchemeColorRole::Shadow,
							utility::graphic::Color32Bit(0, 0, 0, 255),
							"shadow");
			scheme.setColor(SchemeColorRole::Scrim,
							utility::graphic::Color32Bit(0, 0, 0, 255),
							"scrim");
			scheme.setColor(SchemeColorRole::InverseSurface,
							utility::graphic::Color32Bit(241, 223, 218, 255),
							"inverseSurface");
			scheme.setColor(SchemeColorRole::InverseOnSurface,
							utility::graphic::Color32Bit(57, 46, 43, 255),
							"inverseOnSurface");
			scheme.setColor(SchemeColorRole::InversePrimary,
							utility::graphic::Color32Bit(143, 76, 56, 255),
							"inversePrimary");
			scheme.setColor(SchemeColorRole::PrimaryFixed,
							utility::graphic::Color32Bit(255, 219, 209, 255),
							"primaryFixed");
			scheme.setColor(SchemeColorRole::OnPrimaryFixed,
							utility::graphic::Color32Bit(58, 11, 1, 255),
							"onPrimaryFixed");
			scheme.setColor(SchemeColorRole::PrimaryFixedDim,
							utility::graphic::Color32Bit(255, 181, 160, 255),
							"primaryFixedDim");
			scheme.setColor(SchemeColorRole::OnPrimaryFixedVariant,
							utility::graphic::Color32Bit(114, 53, 35, 255),
							"onPrimaryFixedVariant");
			scheme.setColor(SchemeColorRole::SecondaryFixed,
							utility::graphic::Color32Bit(255, 219, 209, 255),
							"secondaryFixed");
			scheme.setColor(SchemeColorRole::OnSecondaryFixed,
							utility::graphic::Color32Bit(44, 21, 15, 255),
							"onSecondaryFixed");
			scheme.setColor(SchemeColorRole::SecondaryFixedDim,
							utility::graphic::Color32Bit(231, 189, 178, 255),
							"secondaryFixedDim");
			scheme.setColor(SchemeColorRole::OnSecondaryFixedVariant,
							utility::graphic::Color32Bit(93, 64, 55, 255),
							"onSecondaryFixedVariant");
			scheme.setColor(SchemeColorRole::TertiaryFixed,
							utility::graphic::Color32Bit(245, 225, 167, 255),
							"tertiaryFixed");
			scheme.setColor(SchemeColorRole::OnTertiaryFixed,
							utility::graphic::Color32Bit(35, 27, 0, 255),
							"onTertiaryFixed");
			scheme.setColor(SchemeColorRole::TertiaryFixedDim,
							utility::graphic::Color32Bit(216, 197, 141, 255),
							"tertiaryFixedDim");
			scheme.setColor(SchemeColorRole::OnTertiaryFixedVariant,
							utility::graphic::Color32Bit(83, 70, 25, 255),
							"onTertiaryFixedVariant");
			scheme.setColor(SchemeColorRole::SurfaceDim,
							utility::graphic::Color32Bit(26, 17, 15, 255),
							"surfaceDim");
			scheme.setColor(SchemeColorRole::SurfaceBright,
							utility::graphic::Color32Bit(66, 55, 52, 255),
							"surfaceBright");
			scheme.setColor(SchemeColorRole::SurfaceContainerLowest,
							utility::graphic::Color32Bit(20, 12, 10, 255),
							"surfaceContainerLowest");
			scheme.setColor(SchemeColorRole::SurfaceContainerLow,
							utility::graphic::Color32Bit(35, 25, 23, 255),
							"surfaceContainerLow");
			scheme.setColor(SchemeColorRole::SurfaceContainer,
							utility::graphic::Color32Bit(39, 29, 27, 255),
							"surfaceContainer");
			scheme.setColor(SchemeColorRole::SurfaceContainerHigh,
							utility::graphic::Color32Bit(50, 40, 37, 255),
							"surfaceContainerHigh");
			scheme.setColor(SchemeColorRole::SurfaceContainerHighest,
							utility::graphic::Color32Bit(61, 50, 47, 255),
							"surfaceContainerHighest");
		}

		/**
		 * @brief Initialize dark medium contrast scheme.
		 */
		void initializeDarkMediumContrastScheme(Scheme &scheme)
		{
			scheme.setColor(SchemeColorRole::Primary,
							utility::graphic::Color32Bit(255, 210, 198, 255),
							"primary");
			scheme.setColor(SchemeColorRole::SurfaceTint,
							utility::graphic::Color32Bit(255, 181, 160, 255),
							"surfaceTint");
			scheme.setColor(SchemeColorRole::OnPrimary,
							utility::graphic::Color32Bit(72, 21, 6, 255),
							"onPrimary");
			scheme.setColor(SchemeColorRole::PrimaryContainer,
							utility::graphic::Color32Bit(203, 124, 101, 255),
							"primaryContainer");
			scheme.setColor(SchemeColorRole::OnPrimaryContainer,
							utility::graphic::Color32Bit(0, 0, 0, 255),
							"onPrimaryContainer");
			scheme.setColor(SchemeColorRole::Secondary,
							utility::graphic::Color32Bit(253, 211, 199, 255),
							"secondary");
			scheme.setColor(SchemeColorRole::OnSecondary,
							utility::graphic::Color32Bit(56, 31, 24, 255),
							"onSecondary");
			scheme.setColor(SchemeColorRole::SecondaryContainer,
							utility::graphic::Color32Bit(174, 136, 126, 255),
							"secondaryContainer");
			scheme.setColor(SchemeColorRole::OnSecondaryContainer,
							utility::graphic::Color32Bit(0, 0, 0, 255),
							"onSecondaryContainer");
			scheme.setColor(SchemeColorRole::Tertiary,
							utility::graphic::Color32Bit(239, 219, 161, 255),
							"tertiary");
			scheme.setColor(SchemeColorRole::OnTertiary,
							utility::graphic::Color32Bit(47, 37, 0, 255),
							"onTertiary");
			scheme.setColor(SchemeColorRole::TertiaryContainer,
							utility::graphic::Color32Bit(160, 144, 92, 255),
							"tertiaryContainer");
			scheme.setColor(SchemeColorRole::OnTertiaryContainer,
							utility::graphic::Color32Bit(0, 0, 0, 255),
							"onTertiaryContainer");
			scheme.setColor(SchemeColorRole::Error,
							utility::graphic::Color32Bit(255, 210, 204, 255),
							"error");
			scheme.setColor(SchemeColorRole::OnError,
							utility::graphic::Color32Bit(84, 0, 3, 255),
							"onError");
			scheme.setColor(SchemeColorRole::ErrorContainer,
							utility::graphic::Color32Bit(255, 84, 73, 255),
							"errorContainer");
			scheme.setColor(SchemeColorRole::OnErrorContainer,
							utility::graphic::Color32Bit(0, 0, 0, 255),
							"onErrorContainer");
			scheme.setColor(SchemeColorRole::Background,
							utility::graphic::Color32Bit(26, 17, 15, 255),
							"background");
			scheme.setColor(SchemeColorRole::OnBackground,
							utility::graphic::Color32Bit(241, 223, 218, 255),
							"onBackground");
			scheme.setColor(SchemeColorRole::Surface,
							utility::graphic::Color32Bit(26, 17, 15, 255),
							"surface");
			scheme.setColor(SchemeColorRole::OnSurface,
							utility::graphic::Color32Bit(255, 255, 255, 255),
							"onSurface");
			scheme.setColor(SchemeColorRole::SurfaceVariant,
							utility::graphic::Color32Bit(83, 67, 63, 255),
							"surfaceVariant");
			scheme.setColor(SchemeColorRole::OnSurfaceVariant,
							utility::graphic::Color32Bit(238, 215, 209, 255),
							"onSurfaceVariant");
			scheme.setColor(SchemeColorRole::Outline,
							utility::graphic::Color32Bit(194, 157, 152, 255),
							"outline");
			scheme.setColor(SchemeColorRole::OutlineVariant,
							utility::graphic::Color32Bit(160, 140, 135, 255),
							"outlineVariant");
			scheme.setColor(SchemeColorRole::Shadow,
							utility::graphic::Color32Bit(0, 0, 0, 255),
							"shadow");
			scheme.setColor(SchemeColorRole::Scrim,
							utility::graphic::Color32Bit(0, 0, 0, 255),
							"scrim");
			scheme.setColor(SchemeColorRole::InverseSurface,
							utility::graphic::Color32Bit(241, 223, 218, 255),
							"inverseSurface");
			scheme.setColor(SchemeColorRole::InverseOnSurface,
							utility::graphic::Color32Bit(50, 40, 37, 255),
							"inverseOnSurface");
			scheme.setColor(SchemeColorRole::InversePrimary,
							utility::graphic::Color32Bit(116, 54, 36, 255),
							"inversePrimary");
			scheme.setColor(SchemeColorRole::PrimaryFixed,
							utility::graphic::Color32Bit(255, 219, 209, 255),
							"primaryFixed");
			scheme.setColor(SchemeColorRole::OnPrimaryFixed,
							utility::graphic::Color32Bit(40, 5, 0, 255),
							"onPrimaryFixed");
			scheme.setColor(SchemeColorRole::PrimaryFixedDim,
							utility::graphic::Color32Bit(255, 181, 160, 255),
							"primaryFixedDim");
			scheme.setColor(SchemeColorRole::OnPrimaryFixedVariant,
							utility::graphic::Color32Bit(93, 37, 20, 255),
							"onPrimaryFixedVariant");
			scheme.setColor(SchemeColorRole::SecondaryFixed,
							utility::graphic::Color32Bit(255, 219, 209, 255),
							"secondaryFixed");
			scheme.setColor(SchemeColorRole::OnSecondaryFixed,
							utility::graphic::Color32Bit(32, 11, 6, 255),
							"onSecondaryFixed");
			scheme.setColor(SchemeColorRole::SecondaryFixedDim,
							utility::graphic::Color32Bit(231, 189, 178, 255),
							"secondaryFixedDim");
			scheme.setColor(SchemeColorRole::OnSecondaryFixedVariant,
							utility::graphic::Color32Bit(75, 47, 40, 255),
							"onSecondaryFixedVariant");
			scheme.setColor(SchemeColorRole::TertiaryFixed,
							utility::graphic::Color32Bit(245, 225, 167, 255),
							"tertiaryFixed");
			scheme.setColor(SchemeColorRole::OnTertiaryFixed,
							utility::graphic::Color32Bit(23, 17, 0, 255),
							"onTertiaryFixed");
			scheme.setColor(SchemeColorRole::TertiaryFixedDim,
							utility::graphic::Color32Bit(216, 197, 141, 255),
							"tertiaryFixedDim");
			scheme.setColor(SchemeColorRole::OnTertiaryFixedVariant,
							utility::graphic::Color32Bit(65, 53, 10, 255),
							"onTertiaryFixedVariant");
			scheme.setColor(SchemeColorRole::SurfaceDim,
							utility::graphic::Color32Bit(26, 17, 15, 255),
							"surfaceDim");
			scheme.setColor(SchemeColorRole::SurfaceBright,
							utility::graphic::Color32Bit(78, 67, 63, 255),
							"surfaceBright");
			scheme.setColor(SchemeColorRole::SurfaceContainerLowest,
							utility::graphic::Color32Bit(13, 6, 4, 255),
							"surfaceContainerLowest");
			scheme.setColor(SchemeColorRole::SurfaceContainerLow,
							utility::graphic::Color32Bit(37, 27, 25, 255),
							"surfaceContainerLow");
			scheme.setColor(SchemeColorRole::SurfaceContainer,
							utility::graphic::Color32Bit(48, 38, 35, 255),
							"surfaceContainer");
			scheme.setColor(SchemeColorRole::SurfaceContainerHigh,
							utility::graphic::Color32Bit(59, 48, 45, 255),
							"surfaceContainerHigh");
			scheme.setColor(SchemeColorRole::SurfaceContainerHighest,
							utility::graphic::Color32Bit(70, 59, 56, 255),
							"surfaceContainerHighest");
		}

		/**
		 * @brief Initialize dark high contrast scheme.
		 */
		void initializeDarkHighContrastScheme(Scheme &scheme)
		{
			scheme.setColor(SchemeColorRole::Primary,
							utility::graphic::Color32Bit(255, 236, 231, 255),
							"primary");
			scheme.setColor(SchemeColorRole::SurfaceTint,
							utility::graphic::Color32Bit(255, 181, 160, 255),
							"surfaceTint");
			scheme.setColor(SchemeColorRole::OnPrimary,
							utility::graphic::Color32Bit(0, 0, 0, 255),
							"onPrimary");
			scheme.setColor(SchemeColorRole::PrimaryContainer,
							utility::graphic::Color32Bit(255, 175, 152, 255),
							"primaryContainer");
			scheme.setColor(SchemeColorRole::OnPrimaryContainer,
							utility::graphic::Color32Bit(30, 3, 0, 255),
							"onPrimaryContainer");
			scheme.setColor(SchemeColorRole::Secondary,
							utility::graphic::Color32Bit(255, 236, 231, 255),
							"secondary");
			scheme.setColor(SchemeColorRole::OnSecondary,
							utility::graphic::Color32Bit(0, 0, 0, 255),
							"onSecondary");
			scheme.setColor(SchemeColorRole::SecondaryContainer,
							utility::graphic::Color32Bit(227, 185, 174, 255),
							"secondaryContainer");
			scheme.setColor(SchemeColorRole::OnSecondaryContainer,
							utility::graphic::Color32Bit(25, 6, 3, 255),
							"onSecondaryContainer");
			scheme.setColor(SchemeColorRole::Tertiary,
							utility::graphic::Color32Bit(255, 239, 196, 255),
							"tertiary");
			scheme.setColor(SchemeColorRole::OnTertiary,
							utility::graphic::Color32Bit(0, 0, 0, 255),
							"onTertiary");
			scheme.setColor(SchemeColorRole::TertiaryContainer,
							utility::graphic::Color32Bit(212, 194, 137, 255),
							"tertiaryContainer");
			scheme.setColor(SchemeColorRole::OnTertiaryContainer,
							utility::graphic::Color32Bit(16, 11, 0, 255),
							"onTertiaryContainer");
			scheme.setColor(SchemeColorRole::Error,
							utility::graphic::Color32Bit(255, 236, 233, 255),
							"error");
			scheme.setColor(SchemeColorRole::OnError,
							utility::graphic::Color32Bit(0, 0, 0, 255),
							"onError");
			scheme.setColor(SchemeColorRole::ErrorContainer,
							utility::graphic::Color32Bit(255, 170, 164, 255),
							"errorContainer");
			scheme.setColor(SchemeColorRole::OnErrorContainer,
							utility::graphic::Color32Bit(34, 0, 1, 255),
							"onErrorContainer");
			scheme.setColor(SchemeColorRole::Background,
							utility::graphic::Color32Bit(26, 17, 15, 255),
							"background");
			scheme.setColor(SchemeColorRole::OnBackground,
							utility::graphic::Color32Bit(241, 223, 218, 255),
							"onBackground");
			scheme.setColor(SchemeColorRole::Surface,
							utility::graphic::Color32Bit(26, 17, 15, 255),
							"surface");
			scheme.setColor(SchemeColorRole::OnSurface,
							utility::graphic::Color32Bit(255, 255, 255, 255),
							"onSurface");
			scheme.setColor(SchemeColorRole::SurfaceVariant,
							utility::graphic::Color32Bit(83, 67, 63, 255),
							"surfaceVariant");
			scheme.setColor(SchemeColorRole::OnSurfaceVariant,
							utility::graphic::Color32Bit(255, 255, 255, 255),
							"onSurfaceVariant");
			scheme.setColor(SchemeColorRole::Outline,
							utility::graphic::Color32Bit(255, 236, 231, 255),
							"outline");
			scheme.setColor(SchemeColorRole::OutlineVariant,
							utility::graphic::Color32Bit(212, 190, 184, 255),
							"outlineVariant");
			scheme.setColor(SchemeColorRole::Shadow,
							utility::graphic::Color32Bit(0, 0, 0, 255),
							"shadow");
			scheme.setColor(SchemeColorRole::Scrim,
							utility::graphic::Color32Bit(0, 0, 0, 255),
							"scrim");
			scheme.setColor(SchemeColorRole::InverseSurface,
							utility::graphic::Color32Bit(241, 223, 218, 255),
							"inverseSurface");
			scheme.setColor(SchemeColorRole::InverseOnSurface,
							utility::graphic::Color32Bit(0, 0, 0, 255),
							"inverseOnSurface");
			scheme.setColor(SchemeColorRole::InversePrimary,
							utility::graphic::Color32Bit(116, 54, 36, 255),
							"inversePrimary");
			scheme.setColor(SchemeColorRole::PrimaryFixed,
							utility::graphic::Color32Bit(255, 219, 209, 255),
							"primaryFixed");
			scheme.setColor(SchemeColorRole::OnPrimaryFixed,
							utility::graphic::Color32Bit(0, 0, 0, 255),
							"onPrimaryFixed");
			scheme.setColor(SchemeColorRole::PrimaryFixedDim,
							utility::graphic::Color32Bit(255, 181, 160, 255),
							"primaryFixedDim");
			scheme.setColor(SchemeColorRole::OnPrimaryFixedVariant,
							utility::graphic::Color32Bit(40, 5, 0, 255),
							"onPrimaryFixedVariant");
			scheme.setColor(SchemeColorRole::SecondaryFixed,
							utility::graphic::Color32Bit(255, 219, 209, 255),
							"secondaryFixed");
			scheme.setColor(SchemeColorRole::OnSecondaryFixed,
							utility::graphic::Color32Bit(0, 0, 0, 255),
							"onSecondaryFixed");
			scheme.setColor(SchemeColorRole::SecondaryFixedDim,
							utility::graphic::Color32Bit(231, 189, 178, 255),
							"secondaryFixedDim");
			scheme.setColor(SchemeColorRole::OnSecondaryFixedVariant,
							utility::graphic::Color32Bit(32, 11, 6, 255),
							"onSecondaryFixedVariant");
			scheme.setColor(SchemeColorRole::TertiaryFixed,
							utility::graphic::Color32Bit(245, 225, 167, 255),
							"tertiaryFixed");
			scheme.setColor(SchemeColorRole::OnTertiaryFixed,
							utility::graphic::Color32Bit(0, 0, 0, 255),
							"onTertiaryFixed");
			scheme.setColor(SchemeColorRole::TertiaryFixedDim,
							utility::graphic::Color32Bit(216, 197, 141, 255),
							"tertiaryFixedDim");
			scheme.setColor(SchemeColorRole::OnTertiaryFixedVariant,
							utility::graphic::Color32Bit(23, 17, 0, 255),
							"onTertiaryFixedVariant");
			scheme.setColor(SchemeColorRole::SurfaceDim,
							utility::graphic::Color32Bit(26, 17, 15, 255),
							"surfaceDim");
			scheme.setColor(SchemeColorRole::SurfaceBright,
							utility::graphic::Color32Bit(90, 77, 74, 255),
							"surfaceBright");
			scheme.setColor(SchemeColorRole::SurfaceContainerLowest,
							utility::graphic::Color32Bit(0, 0, 0, 255),
							"surfaceContainerLowest");
			scheme.setColor(SchemeColorRole::SurfaceContainerLow,
							utility::graphic::Color32Bit(39, 29, 27, 255),
							"surfaceContainerLow");
			scheme.setColor(SchemeColorRole::SurfaceContainer,
							utility::graphic::Color32Bit(57, 46, 43, 255),
							"surfaceContainer");
			scheme.setColor(SchemeColorRole::SurfaceContainerHigh,
							utility::graphic::Color32Bit(68, 57, 54, 255),
							"surfaceContainerHigh");
			scheme.setColor(SchemeColorRole::SurfaceContainerHighest,
							utility::graphic::Color32Bit(80, 68, 65, 255),
							"surfaceContainerHighest");
		}

		/**
		 * @brief Initialize all palettes with their tones.
		 */
		void initializePalettes(Theme &theme)
		{
			// Primary palette
			Palette primaryPalette("Primary", "Primary color palette");
			primaryPalette.setTone(Palette::Tone::T0,
								   utility::graphic::Color32Bit(0, 0, 0, 255),
								   "0");
			primaryPalette.setTone(Palette::Tone::T10,
								   utility::graphic::Color32Bit(58, 11, 1, 255),
								   "10");
			primaryPalette.setTone(Palette::Tone::T20,
								   utility::graphic::Color32Bit(62, 21, 9, 255),
								   "20");
			primaryPalette.setTone(
				Palette::Tone::T30,
				utility::graphic::Color32Bit(134, 34, 0, 255), "30");
			primaryPalette.setTone(
				Palette::Tone::T40,
				utility::graphic::Color32Bit(171, 53, 15, 255), "40");
			primaryPalette.setTone(
				Palette::Tone::T50,
				utility::graphic::Color32Bit(205, 77, 38, 255), "50");
			primaryPalette.setTone(
				Palette::Tone::T60,
				utility::graphic::Color32Bit(239, 102, 61, 255), "60");
			primaryPalette.setTone(
				Palette::Tone::T70,
				utility::graphic::Color32Bit(255, 139, 105, 255), "70");
			primaryPalette.setTone(
				Palette::Tone::T80,
				utility::graphic::Color32Bit(255, 181, 160, 255), "80");
			primaryPalette.setTone(
				Palette::Tone::T90,
				utility::graphic::Color32Bit(255, 219, 209, 255), "90");
			primaryPalette.setTone(
				Palette::Tone::T100,
				utility::graphic::Color32Bit(255, 255, 255, 255), "100");
			theme.setPrimaryPalette(primaryPalette);

			// Secondary palette
			Palette secondaryPalette("Secondary", "Secondary color palette");
			secondaryPalette.setTone(Palette::Tone::T0,
									 utility::graphic::Color32Bit(0, 0, 0, 255),
									 "0");
			secondaryPalette.setTone(
				Palette::Tone::T10, utility::graphic::Color32Bit(36, 8, 3, 255),
				"10");
			secondaryPalette.setTone(
				Palette::Tone::T20,
				utility::graphic::Color32Bit(49, 19, 9, 255), "20");
			secondaryPalette.setTone(
				Palette::Tone::T30,
				utility::graphic::Color32Bit(62, 28, 13, 255), "30");
			secondaryPalette.setTone(
				Palette::Tone::T40,
				utility::graphic::Color32Bit(127, 83, 71, 255), "40");
			secondaryPalette.setTone(
				Palette::Tone::T50,
				utility::graphic::Color32Bit(155, 108, 98, 255), "50");
			secondaryPalette.setTone(
				Palette::Tone::T60,
				utility::graphic::Color32Bit(184, 133, 124, 255), "60");
			secondaryPalette.setTone(
				Palette::Tone::T70,
				utility::graphic::Color32Bit(213, 159, 143, 255), "70");
			secondaryPalette.setTone(
				Palette::Tone::T80,
				utility::graphic::Color32Bit(242, 185, 169, 255), "80");
			secondaryPalette.setTone(
				Palette::Tone::T90,
				utility::graphic::Color32Bit(255, 219, 209, 255), "90");
			secondaryPalette.setTone(
				Palette::Tone::T100,
				utility::graphic::Color32Bit(255, 255, 255, 255), "100");
			theme.setSecondaryPalette(secondaryPalette);

			// Tertiary palette
			Palette tertiaryPalette("Tertiary", "Tertiary color palette");
			tertiaryPalette.setTone(Palette::Tone::T0,
									utility::graphic::Color32Bit(0, 0, 0, 255),
									"0");
			tertiaryPalette.setTone(
				Palette::Tone::T10,
				utility::graphic::Color32Bit(35, 16, 0, 255), "10");
			tertiaryPalette.setTone(
				Palette::Tone::T20,
				utility::graphic::Color32Bit(47, 27, 0, 255), "20");
			tertiaryPalette.setTone(
				Palette::Tone::T30,
				utility::graphic::Color32Bit(59, 39, 0, 255), "30");
			tertiaryPalette.setTone(
				Palette::Tone::T40,
				utility::graphic::Color32Bit(72, 51, 16, 255), "40");
			tertiaryPalette.setTone(
				Palette::Tone::T50,
				utility::graphic::Color32Bit(138, 117, 45, 255), "50");
			tertiaryPalette.setTone(
				Palette::Tone::T60,
				utility::graphic::Color32Bit(165, 143, 68, 255), "60");
			tertiaryPalette.setTone(
				Palette::Tone::T70,
				utility::graphic::Color32Bit(193, 170, 92, 255), "70");
			tertiaryPalette.setTone(
				Palette::Tone::T80,
				utility::graphic::Color32Bit(222, 197, 116, 255), "80");
			tertiaryPalette.setTone(
				Palette::Tone::T90,
				utility::graphic::Color32Bit(252, 225, 141, 255), "90");
			tertiaryPalette.setTone(
				Palette::Tone::T100,
				utility::graphic::Color32Bit(255, 255, 255, 255), "100");
			theme.setTertiaryPalette(tertiaryPalette);

			// Neutral palette
			Palette neutralPalette("Neutral", "Neutral color palette");
			neutralPalette.setTone(Palette::Tone::T0,
								   utility::graphic::Color32Bit(0, 0, 0, 255),
								   "0");
			neutralPalette.setTone(
				Palette::Tone::T10,
				utility::graphic::Color32Bit(21, 16, 14, 255), "10");
			neutralPalette.setTone(
				Palette::Tone::T20,
				utility::graphic::Color32Bit(32, 26, 24, 255), "20");
			neutralPalette.setTone(
				Palette::Tone::T30,
				utility::graphic::Color32Bit(45, 37, 35, 255), "30");
			neutralPalette.setTone(
				Palette::Tone::T40,
				utility::graphic::Color32Bit(54, 46, 42, 255), "40");
			neutralPalette.setTone(
				Palette::Tone::T50,
				utility::graphic::Color32Bit(127, 115, 114, 255), "50");
			neutralPalette.setTone(
				Palette::Tone::T60,
				utility::graphic::Color32Bit(152, 140, 140, 255), "60");
			neutralPalette.setTone(
				Palette::Tone::T70,
				utility::graphic::Color32Bit(180, 169, 166, 255), "70");
			neutralPalette.setTone(
				Palette::Tone::T80,
				utility::graphic::Color32Bit(208, 196, 193, 255), "80");
			neutralPalette.setTone(
				Palette::Tone::T90,
				utility::graphic::Color32Bit(237, 224, 220, 255), "90");
			neutralPalette.setTone(
				Palette::Tone::T100,
				utility::graphic::Color32Bit(255, 255, 255, 255), "100");
			theme.setNeutralPalette(neutralPalette);

			// Neutral Variant palette
			Palette neutralVariantPalette("Neutral Variant",
										  "Neutral variant color palette");
			neutralVariantPalette.setTone(
				Palette::Tone::T0, utility::graphic::Color32Bit(0, 0, 0, 255),
				"0");
			neutralVariantPalette.setTone(
				Palette::Tone::T10,
				utility::graphic::Color32Bit(25, 14, 11, 255), "10");
			neutralVariantPalette.setTone(
				Palette::Tone::T20,
				utility::graphic::Color32Bit(37, 25, 21, 255), "20");
			neutralVariantPalette.setTone(
				Palette::Tone::T30,
				utility::graphic::Color32Bit(48, 35, 31, 255), "30");
			neutralVariantPalette.setTone(
				Palette::Tone::T40,
				utility::graphic::Color32Bit(108, 91, 86, 255), "40");
			neutralVariantPalette.setTone(
				Palette::Tone::T50,
				utility::graphic::Color32Bit(133, 115, 110, 255), "50");
			neutralVariantPalette.setTone(
				Palette::Tone::T60,
				utility::graphic::Color32Bit(160, 140, 135, 255), "60");
			neutralVariantPalette.setTone(
				Palette::Tone::T70,
				utility::graphic::Color32Bit(188, 167, 161, 255), "70");
			neutralVariantPalette.setTone(
				Palette::Tone::T80,
				utility::graphic::Color32Bit(216, 194, 188, 255), "80");
			neutralVariantPalette.setTone(
				Palette::Tone::T90,
				utility::graphic::Color32Bit(245, 222, 216, 255), "90");
			neutralVariantPalette.setTone(
				Palette::Tone::T100,
				utility::graphic::Color32Bit(255, 255, 255, 255), "100");
			theme.setNeutralVariantPalette(neutralVariantPalette);
		}
	}	 // namespace

	/**
	 * @brief Initialize the defaultTheme with Material Design 3 colors and
	 * schemes from material-theme.json.
	 */
	Theme initializeDefaultTheme()
	{
		Theme theme(
			"Material Design 3 Theme",
			"A theme with Material Design 3 colors and multiple contrast "
			"levels");

		// Initialize all color schemes
		initializeLightScheme(theme.getLightScheme());
		initializeLightMediumContrastScheme(
			theme.getLightMediumContrastScheme());
		initializeLightHighContrastScheme(theme.getLightHighContrastScheme());
		initializeDarkScheme(theme.getDarkScheme());
		initializeDarkMediumContrastScheme(theme.getDarkMediumContrastScheme());
		initializeDarkHighContrastScheme(theme.getDarkHighContrastScheme());

		// Initialize all palettes
		initializePalettes(theme);

		return theme;
	}

	const Theme defaultTheme = initializeDefaultTheme();

}	 // namespace guillaume
