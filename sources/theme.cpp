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
#include "guillaume/theme/default_theme.hpp"

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

	constexpr std::size_t Scheme::toIndex(SchemeColorRole role)
	{
		return static_cast<std::size_t>(role);
	}

	Scheme::Scheme(
		const std::array<ExtendedColor,
						 static_cast<std::size_t>(SchemeColorRole::Count)>
			&colors)
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
						  const std::string &description, bool harmonized)
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

	void Palette::setTone(Tone tone, const utility::graphic::Color32Bit &color,
						  const std::string &name,
						  const std::string &description, bool harmonized)
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

	const Scheme &Theme::getScheme(ThemeSchemeRole role) const
	{
		return _schemes[role];
	}

	Scheme &Theme::getScheme(ThemeSchemeRole role)
	{
		return _schemes[role];
	}

	const Palette &Theme::getPalette(ThemePaletteRole role) const
	{
		return _palettes[role];
	}

	Palette &Theme::getPalette(ThemePaletteRole role)
	{
		return _palettes[role];
	}

	void Theme::setScheme(ThemeSchemeRole role, const Scheme &scheme)
	{
		_schemes[role] = scheme;
	}

	void Theme::setPalette(ThemePaletteRole role, const Palette &palette)
	{
		_palettes[role] = palette;
	}

	const theme::Typescale &Theme::getTypescale(void) const
	{
		return _typescale;
	}

	theme::Typescale &Theme::getTypescale(void)
	{
		return _typescale;
	}

	void Theme::setTypescale(const theme::Typescale &typescale)
	{
		_typescale = typescale;
	}

	const theme::ShapeScale &Theme::getShapes(void) const
	{
		return _shapes;
	}

	theme::ShapeScale &Theme::getShapes(void)
	{
		return _shapes;
	}

	const theme::ElevationScale &Theme::getElevations(void) const
	{
		return _elevations;
	}

	theme::ElevationScale &Theme::getElevations(void)
	{
		return _elevations;
	}

	const theme::StateLayerScale &Theme::getStateLayers(void) const
	{
		return _stateLayers;
	}

	theme::StateLayerScale &Theme::getStateLayers(void)
	{
		return _stateLayers;
	}

	const theme::MotionScale &Theme::getMotion(void) const
	{
		return _motion;
	}

	theme::MotionScale &Theme::getMotion(void)
	{
		return _motion;
	}

	const Theme defaultTheme = initializeDefaultTheme();

	namespace
	{
		ThemeSchemeRole activeSchemeRole = ThemeSchemeRole::Light;
	}

	const Scheme &getActiveScheme(void)
	{
		return defaultTheme.getScheme(activeSchemeRole);
	}

	void setActiveScheme(ThemeSchemeRole role)
	{
		activeSchemeRole = role;
	}

	const theme::Typescale &getActiveTypescale(void)
	{
		return defaultTheme.getTypescale();
	}

	utility::graphic::Color32Bit getActiveColor(SchemeColorRole role)
	{
		return getActiveScheme().getColor(role).getColor();
	}

	const theme::TypeToken &getTypeToken(theme::TypeRole role)
	{
		return getActiveTypescale().get(role);
	}

	float getShapeRadius(theme::ShapeRole role)
	{
		return defaultTheme.getShapes().getRadius(role);
	}

	std::uint8_t getStateLayerAlpha(theme::StateLayer layer)
	{
		return defaultTheme.getStateLayers().getAlpha(layer);
	}

	const theme::ElevationToken &getElevationToken(theme::ElevationLevel level)
	{
		return defaultTheme.getElevations().get(level);
	}

	utility::graphic::Color32Bit
		getElevatedSurfaceColor(theme::ElevationLevel level)
	{
		const auto &scheme = getActiveScheme();
		return defaultTheme.getElevations().tint(
			scheme.getColor(SchemeColorRole::Surface).getColor(),
			scheme.getColor(SchemeColorRole::SurfaceTint).getColor(), level);
	}

	float getMotionDuration(theme::MotionDuration duration)
	{
		return defaultTheme.getMotion().getDuration(duration);
	}

}	 // namespace guillaume
