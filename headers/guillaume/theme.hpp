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

#include <utility/graphic/color.hpp>

#include "guillaume/theme/tokens.hpp"
#include "guillaume/theme/typescale.hpp"

namespace guillaume
{

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
		SurfaceDisabled,	  ///< Disabled container (onSurface at 12%)
		OnSurfaceDisabled,	  ///< Disabled content (onSurface at 38%)
		Backdrop,			  ///< Modal backdrop behind dialogs and drawers
		Count
	};

	/**
	 * @brief Theme scheme variants (light/dark and contrast levels).
	 */
	enum ThemeSchemeRole : size_t {
		Light,
		LightMediumContrast,
		LightHighContrast,
		Dark,
		DarkMediumContrast,
		DarkHighContrast,
		ThemeSchemeCount
	};

	/**
	 * @brief Theme palette categories used by Material Design.
	 */
	enum ThemePaletteRole : size_t {
		Primary,
		Secondary,
		Tertiary,
		Neutral,
		NeutralVariant,
		ThemePaletteCount
	};

	/**
	 * @brief Extended color class that includes additional metadata for a
	 * color. Based on the Material Design color system.
	 */
	class ExtendedColor
	{
		private:
		utility::graphic::Color32Bit _color;	///< RGBA color value
		std::string _name;			 ///< Human-readable name for the color
		std::string _description;	 ///< Optional description of the color's
									 ///< purpose or usage
		bool _harmonized;	 ///< True if the color was harmonized from a source
							 ///< color, false if it is an original color in the
							 ///< palette

		public:
		/**
		 * @brief Construct an empty extended color.
		 */
		ExtendedColor(void);

		/**
		 * @brief Construct an extended color with metadata.
		 * @param color RGBA color value.
		 * @param name Human-readable name.
		 * @param description Optional description.
		 * @param harmonized True if harmonized from a source color.
		 */
		ExtendedColor(const utility::graphic::Color32Bit &color,
					  const std::string &name, const std::string &description,
					  bool harmonized);

		/**
		 * @brief Default destructor.
		 */
		~ExtendedColor(void) = default;

		/**
		 * @brief Get the RGBA color value.
		 * @return Immutable color reference.
		 */
		const utility::graphic::Color32Bit &getColor(void) const;

		/**
		 * @brief Get the color name.
		 * @return Immutable name reference.
		 */
		const std::string &getName(void) const;

		/**
		 * @brief Get the color description.
		 * @return Immutable description reference.
		 */
		const std::string &getDescription(void) const;

		/**
		 * @brief Get harmonization state.
		 * @return True when color is harmonized.
		 */
		bool isHarmonized(void) const;

		/**
		 * @brief Set the RGBA color value.
		 * @param color New RGBA color.
		 */
		void setColor(const utility::graphic::Color32Bit &color);

		/**
		 * @brief Set the color name.
		 * @param name New color name.
		 */
		void setName(const std::string &name);

		/**
		 * @brief Set the color description.
		 * @param description New color description.
		 */
		void setDescription(const std::string &description);

		/**
		 * @brief Set harmonization state.
		 * @param harmonized True if color is harmonized.
		 */
		void setHarmonized(bool harmonized);
	};

	/**
	 * @brief Color scheme for a theme, containing all the defined colors. Based
	 * on the Material Design color system.
	 */
	class Scheme
	{
		private:
		std::array<ExtendedColor,
				   static_cast<std::size_t>(SchemeColorRole::Count)>
			_colors;

		static constexpr std::size_t toIndex(SchemeColorRole role);

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
			const std::array<ExtendedColor,
							 static_cast<std::size_t>(SchemeColorRole::Count)>
				&colors);

		/**
		 * @brief Default destructor.
		 */
		~Scheme(void) = default;

		/**
		 * @brief Get a scheme color by role.
		 * @param role Target role.
		 * @return Immutable extended color reference.
		 */
		const ExtendedColor &getColor(SchemeColorRole role) const;

		/**
		 * @brief Get a mutable scheme color by role.
		 * @param role Target role.
		 * @return Mutable extended color reference.
		 */
		ExtendedColor &getColor(SchemeColorRole role);

		/**
		 * @brief Set a scheme color by role.
		 * @param role Target role.
		 * @param color Extended color value.
		 */
		void setColor(SchemeColorRole role, const ExtendedColor &color);

		/**
		 * @brief Set a scheme color by role from raw value and metadata.
		 * @param role Target role.
		 * @param color Raw RGBA color value.
		 * @param name Optional color name.
		 * @param description Optional color description.
		 * @param harmonized True if harmonized from source color.
		 */
		void setColor(SchemeColorRole role,
					  const utility::graphic::Color32Bit &color,
					  const std::string &name		 = "",
					  const std::string &description = "",
					  bool harmonized				 = false);

		/**
		 * @brief Get all colors in role index order.
		 * @return Immutable color array reference.
		 */
		const auto &getColors(void) const;
	};

	/**
	 * @brief Tonal palette storing tone-to-color mapping for a color family.
	 */
	class Palette
	{
		public:
		/**
		 * @brief Enumerate the available tones in the palette.
		 */
		enum class Tone : std::uint8_t {
			T0	 = 0,
			T10	 = 10,
			T20	 = 20,
			T30	 = 30,
			T40	 = 40,
			T50	 = 50,
			T60	 = 60,
			T70	 = 70,
			T80	 = 80,
			T90	 = 90,
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
		Palette(void);

		/**
		 * @brief Construct a named palette.
		 * @param name Palette name.
		 * @param description Palette description.
		 */
		Palette(const std::string &name, const std::string &description);

		/**
		 * @brief Default destructor.
		 */
		~Palette(void) = default;

		/**
		 * @brief Get palette name.
		 * @return Immutable name reference.
		 */
		const std::string &getName(void) const;

		/**
		 * @brief Get palette description.
		 * @return Immutable description reference.
		 */
		const std::string &getDescription(void) const;

		/**
		 * @brief Get the full tone map.
		 * @return Immutable map from tone to extended color.
		 */
		const std::map<Tone, ExtendedColor> &getTones(void) const;

		/**
		 * @brief Set palette name.
		 * @param name New palette name.
		 */
		void setName(const std::string &name);

		/**
		 * @brief Set palette description.
		 * @param description New palette description.
		 */
		void setDescription(const std::string &description);

		/**
		 * @brief Set tone using an extended color value.
		 * @param tone Tone key.
		 * @param color Extended color value.
		 */
		void setTone(Tone tone, const ExtendedColor &color);

		/**
		 * @brief Set tone using raw color and metadata.
		 * @param tone Tone key.
		 * @param color Raw RGBA color value.
		 * @param name Optional color name.
		 * @param description Optional color description.
		 * @param harmonized True if harmonized from source color.
		 */
		void setTone(Tone tone, const utility::graphic::Color32Bit &color,
					 const std::string &name		= "",
					 const std::string &description = "",
					 bool harmonized				= false);

		/**
		 * @brief Check if a tone exists in the palette.
		 * @param tone Tone key.
		 * @return True when tone exists.
		 */
		bool hasTone(Tone tone) const;

		/**
		 * @brief Get a tone from the palette.
		 * @param tone Tone key.
		 * @return Immutable extended color reference for the tone.
		 * @throws std::out_of_range When tone does not exist.
		 */
		const ExtendedColor &getTone(Tone tone) const;

		/**
		 * @brief Remove a tone from the palette.
		 * @param tone Tone key.
		 */
		void removeTone(Tone tone);

		/**
		 * @brief Remove all tones from the palette.
		 */
		void clearTones(void);
	};

	/**
	 * @brief Base class for application themes, based on the Material Design
	 * guidelines.
	 */
	class Theme
	{
		private:
		std::string _name;	  ///< Theme name for identification
		std::string
			_description;	 ///< Optional theme description for documentation
		std::array<Scheme, ThemeSchemeRole::ThemeSchemeCount>
			_schemes;	 ///< Schemes indexed by ThemeSchemeRole
		std::array<Palette, ThemePaletteRole::ThemePaletteCount>
			_palettes;	  ///< Palettes indexed by ThemePaletteRole
		theme::Typescale _typescale;			///< Material 3 typescale
		theme::ShapeScale _shapes;				///< Material 3 shape scale
		theme::ElevationScale _elevations;		///< Material 3 elevation scale
		theme::StateLayerScale _stateLayers;	///< Material 3 state layers
		theme::MotionScale _motion;				///< Material 3 motion scale

		public:
		/**
		 * @brief Construct an empty theme.
		 */
		Theme(void);

		/**
		 * @brief Construct a named theme.
		 * @param name Theme name.
		 * @param description Theme description.
		 */
		Theme(const std::string &name, const std::string &description);

		/**
		 * @brief Default destructor.
		 */
		virtual ~Theme(void) = default;

		/**
		 * @brief Get theme name.
		 * @return Immutable name reference.
		 */
		const std::string &getName(void) const;

		/**
		 * @brief Get theme description.
		 * @return Immutable description reference.
		 */
		const std::string &getDescription(void) const;

		/**
		 * @brief Set theme name.
		 * @param name New theme name.
		 */
		void setName(const std::string &name);

		/**
		 * @brief Set theme description.
		 * @param description New theme description.
		 */
		void setDescription(const std::string &description);

		/**
		 * @brief Get an immutable scheme by role.
		 * @param role Scheme role selector.
		 * @return Immutable scheme reference.
		 */
		const Scheme &getScheme(ThemeSchemeRole role) const;

		/**
		 * @brief Get a mutable scheme by role.
		 * @param role Scheme role selector.
		 * @return Mutable scheme reference.
		 */
		Scheme &getScheme(ThemeSchemeRole role);

		/**
		 * @brief Get an immutable palette by role.
		 * @param role Palette role selector.
		 * @return Immutable palette reference.
		 */
		const Palette &getPalette(ThemePaletteRole role) const;

		/**
		 * @brief Get a mutable palette by role.
		 * @param role Palette role selector.
		 * @return Mutable palette reference.
		 */
		Palette &getPalette(ThemePaletteRole role);

		/**
		 * @brief Set a scheme for a given role.
		 * @param role Scheme role selector.
		 * @param scheme Scheme to set for the role.
		 */
		void setScheme(ThemeSchemeRole role, const Scheme &scheme);

		/**
		 * @brief Set a palette for a given role.
		 * @param role Palette role selector.
		 * @param palette Palette to set for the role.
		 */
		void setPalette(ThemePaletteRole role, const Palette &palette);

		/**
		 * @brief Get the theme typescale.
		 * @return Immutable typescale reference.
		 */
		const theme::Typescale &getTypescale(void) const;

		/**
		 * @brief Get the mutable theme typescale.
		 * @return Mutable typescale reference.
		 */
		theme::Typescale &getTypescale(void);

		/**
		 * @brief Replace the theme typescale.
		 * @param typescale New typescale.
		 */
		void setTypescale(const theme::Typescale &typescale);

		/** @brief Get the shape scale. @return Immutable reference. */
		const theme::ShapeScale &getShapes(void) const;

		/** @brief Get the mutable shape scale. @return Mutable reference. */
		theme::ShapeScale &getShapes(void);

		/** @brief Get the elevation scale. @return Immutable reference. */
		const theme::ElevationScale &getElevations(void) const;

		/** @brief Get the mutable elevation scale. @return Mutable reference.
		 */
		theme::ElevationScale &getElevations(void);

		/** @brief Get the state-layer scale. @return Immutable reference. */
		const theme::StateLayerScale &getStateLayers(void) const;

		/** @brief Get the mutable state-layer scale. @return Mutable
		 * reference. */
		theme::StateLayerScale &getStateLayers(void);

		/** @brief Get the motion scale. @return Immutable reference. */
		const theme::MotionScale &getMotion(void) const;

		/** @brief Get the mutable motion scale. @return Mutable reference. */
		theme::MotionScale &getMotion(void);
	};

	extern const Theme defaultTheme;

	/**
	 * @brief Get the currently active color scheme.
	 * @return Immutable reference to the active scheme.
	 */
	const Scheme &getActiveScheme(void);

	/**
	 * @brief Get a color role from the active scheme.
	 * @param role The scheme color role.
	 * @return The role color.
	 */
	utility::graphic::Color32Bit getActiveColor(SchemeColorRole role);

	/**
	 * @brief Set the currently active color scheme.
	 * @param role The scheme role to activate.
	 */
	void setActiveScheme(ThemeSchemeRole role);

	/**
	 * @brief Get the typescale of the active theme.
	 * @return Immutable typescale reference.
	 */
	const theme::Typescale &getActiveTypescale(void);

	/**
	 * @brief Get the active theme token of a typescale role.
	 * @param role Typescale role.
	 * @return Immutable token reference.
	 */
	const theme::TypeToken &getTypeToken(theme::TypeRole role);

	/**
	 * @brief Resolve a shape role against the active theme.
	 * @param role Shape role.
	 * @return The corner radius, in dp.
	 */
	float getShapeRadius(theme::ShapeRole role);

	/**
	 * @brief Get a state-layer alpha of the active theme.
	 * @param layer State layer.
	 * @return The alpha in [0, 255].
	 */
	std::uint8_t getStateLayerAlpha(theme::StateLayer layer);

	/**
	 * @brief Get an elevation token of the active theme.
	 * @param level Elevation level.
	 * @return Immutable token reference.
	 */
	const theme::ElevationToken &getElevationToken(theme::ElevationLevel level);

	/**
	 * @brief Get the active Surface color tinted for an elevation level.
	 * @param level Elevation level.
	 * @return The tinted surface color.
	 */
	utility::graphic::Color32Bit
		getElevatedSurfaceColor(theme::ElevationLevel level);

	/**
	 * @brief Get a motion duration of the active theme.
	 * @param duration Duration token.
	 * @return The duration, in seconds.
	 */
	float getMotionDuration(theme::MotionDuration duration);

}	 // namespace guillaume
