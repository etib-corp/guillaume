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
	 * @brief The named colors of a scheme: what a color is used for, not
	 * what it looks like.
	 *
	 * Widgets never hard-code a color; they ask for a role
	 * (`getActiveColor(SchemeColorRole::Primary)`) and get the color the
	 * active scheme gives it. The roles are those of Material Design 3 and
	 * follow its naming:
	 * - `Primary`, `Secondary`, `Tertiary`, `Error`: accent fills.
	 * - `XContainer`: a quieter fill of the same family.
	 * - `OnX`: the color of text and icons drawn on top of `X`.
	 * - `Surface...`: backgrounds, from `SurfaceContainerLowest` to
	 *   `SurfaceContainerHighest` by increasing emphasis.
	 * - `...Fixed`, `...FixedDim`: fills that are the same in light and
	 *   dark schemes.
	 *
	 * @see Scheme
	 * @see getActiveColor
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
		Count				  ///< Number of roles; not a role
	};

	/**
	 * @brief The six color schemes a Theme holds: light and dark, each in
	 * standard, medium and high contrast.
	 *
	 * setActiveScheme() chooses which one the widgets read; it starts as
	 * `Light`.
	 * @note The enumeration is unscoped: `Dark` and `ThemeSchemeRole::Dark`
	 * both name the same value.
	 */
	enum ThemeSchemeRole : size_t {
		Light,					///< Light scheme, standard contrast
		LightMediumContrast,	///< Light scheme, medium contrast
		LightHighContrast,		///< Light scheme, high contrast
		Dark,					///< Dark scheme, standard contrast
		DarkMediumContrast,		///< Dark scheme, medium contrast
		DarkHighContrast,		///< Dark scheme, high contrast
		ThemeSchemeCount		///< Number of schemes; not a scheme
	};

	/**
	 * @brief The five tonal palettes a Theme holds, one per color family.
	 *
	 * The palettes are the raw material the scheme colors are picked from.
	 * @note The enumeration is unscoped: `Neutral` and
	 * `ThemePaletteRole::Neutral` both name the same value.
	 */
	enum ThemePaletteRole : size_t {
		Primary,			 ///< Tones of the primary accent color
		Secondary,			 ///< Tones of the secondary accent color
		Tertiary,			 ///< Tones of the tertiary accent color
		Neutral,			 ///< Greys used for surfaces and text
		NeutralVariant,		 ///< Tinted greys used for outlines and variants
		ThemePaletteCount	 ///< Number of palettes; not a palette
	};

	/**
	 * @brief A color with a name and a description, as stored in a Scheme or
	 * a Palette.
	 *
	 * Only the color value is used for drawing. The name and the
	 * description document the theme (the default theme names each color
	 * after its role, "primary", "onPrimary", ...).
	 */
	class ExtendedColor
	{
		private:
		utility::graphic::Color32Bit
			_color;					 ///< The color, 8 bits per RGBA channel
		std::string _name;			 ///< Name of the color, for documentation
		std::string _description;	 ///< What the color is for; may be empty
		bool _harmonized;	 ///< True if the color was harmonized from a source
							 ///< color, false if it is an original color in the
							 ///< palette

		public:
		/**
		 * @brief Create a default-constructed color with an empty name and
		 * description, not harmonized.
		 */
		ExtendedColor(void);

		/**
		 * @brief Create a color with its documentation.
		 * @param color The color, 8 bits per RGBA channel.
		 * @param name The name of the color, such as "primary".
		 * @param description What the color is for; may be empty.
		 * @param harmonized True when the color was shifted toward the
		 * theme's source color, false when it is used as designed.
		 */
		ExtendedColor(const utility::graphic::Color32Bit &color,
					  const std::string &name, const std::string &description,
					  bool harmonized);

		/**
		 * @brief Default destructor.
		 */
		~ExtendedColor(void) = default;

		/**
		 * @brief Get the color to draw with.
		 * @return The color, 8 bits per RGBA channel.
		 */
		const utility::graphic::Color32Bit &getColor(void) const;

		/**
		 * @brief Get the name of the color.
		 * @return The name; empty when none was given.
		 */
		const std::string &getName(void) const;

		/**
		 * @brief Get the text describing what the color is for.
		 * @return The description; empty when none was given.
		 */
		const std::string &getDescription(void) const;

		/**
		 * @brief Tell whether the color was shifted toward the theme's
		 * source color.
		 * @return True when the color is marked as harmonized. The flag is
		 * informative; nothing in the library computes from it.
		 */
		bool isHarmonized(void) const;

		/**
		 * @brief Replace the color value; the name and the description are
		 * kept.
		 * @param color The new color, 8 bits per RGBA channel.
		 */
		void setColor(const utility::graphic::Color32Bit &color);

		/**
		 * @brief Replace the name of the color.
		 * @param name The new name.
		 */
		void setName(const std::string &name);

		/**
		 * @brief Replace the text describing what the color is for.
		 * @param description The new description; may be empty.
		 */
		void setDescription(const std::string &description);

		/**
		 * @brief Mark the color as harmonized or not.
		 * @param harmonized True when the color was shifted toward the
		 * theme's source color.
		 */
		void setHarmonized(bool harmonized);
	};

	/**
	 * @brief One complete set of colors: a color for every SchemeColorRole.
	 *
	 * A Theme holds six schemes (ThemeSchemeRole). Widgets read the active
	 * one through getActiveColor(). To change a color, edit the scheme in a
	 * copy of the theme and activate the copy:
	 * @code
	 * guillaume::Theme theme = guillaume::getActiveTheme();
	 * theme.getScheme(guillaume::ThemeSchemeRole::Light)
	 * 	.setColor(guillaume::SchemeColorRole::Primary,
	 * 			  utility::graphic::Color32Bit(0, 106, 106, 255));
	 * guillaume::setActiveTheme(theme);
	 * @endcode
	 *
	 * @see Theme
	 * @see getActiveScheme
	 */
	class Scheme
	{
		private:
		std::array<ExtendedColor,
				   static_cast<std::size_t>(SchemeColorRole::Count)>
			_colors;	///< One color per role, indexed by SchemeColorRole

		/**
		 * @brief Convert a role to its index in the color array.
		 * @param role The role to convert.
		 * @return The position of the role in the enumeration.
		 */
		static constexpr std::size_t toIndex(SchemeColorRole role);

		public:
		/**
		 * @brief Create a scheme in which every role has a
		 * default-constructed, unnamed color.
		 */
		Scheme(void) = default;

		/**
		 * @brief Create a scheme from a full set of colors.
		 * @param colors One color per role, in the order of the
		 * SchemeColorRole enumerators.
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
		 * @brief Get the color of a role, with its name and description.
		 * @param role The role to look up. Must not be
		 * `SchemeColorRole::Count`.
		 * @return The color of that role; call `getColor()` on it for the
		 * value to draw with.
		 */
		const ExtendedColor &getColor(SchemeColorRole role) const;

		/**
		 * @brief Get the color of a role, to modify it in place.
		 * @param role The role to look up. Must not be
		 * `SchemeColorRole::Count`.
		 * @return Mutable reference to the color of that role.
		 */
		ExtendedColor &getColor(SchemeColorRole role);

		/**
		 * @brief Replace the color of a role, with its name and
		 * description.
		 * @param role The role to change. Must not be
		 * `SchemeColorRole::Count`.
		 * @param color The new color and its documentation.
		 */
		void setColor(SchemeColorRole role, const ExtendedColor &color);

		/**
		 * @brief Replace the color of a role from a plain color value.
		 *
		 * The name, the description and the harmonized flag of the role are
		 * replaced too, by the arguments or their defaults.
		 * @param role The role to change. Must not be
		 * `SchemeColorRole::Count`.
		 * @param color The new color, 8 bits per RGBA channel.
		 * @param name The name of the color; empty by default.
		 * @param description What the color is for; empty by default.
		 * @param harmonized True when the color was shifted toward the
		 * theme's source color.
		 */
		void setColor(SchemeColorRole role,
					  const utility::graphic::Color32Bit &color,
					  const std::string &name		 = "",
					  const std::string &description = "",
					  bool harmonized				 = false);

		/**
		 * @brief Get every color of the scheme.
		 * @return The array of colors, indexed by SchemeColorRole.
		 * @note The return type is deduced and the function is defined in
		 * `theme.cpp`, so it can only be called from that file. Use
		 * getColor() elsewhere.
		 */
		const auto &getColors(void) const;
	};

	/**
	 * @brief The shades of one color family, from black (tone 0) to white
	 * (tone 100).
	 *
	 * A Theme holds five palettes (ThemePaletteRole). They are reference
	 * data: widgets read scheme colors, not tones. A palette starts empty;
	 * tones are added with setTone().
	 *
	 * @see Theme::getPalette
	 */
	class Palette
	{
		public:
		/**
		 * @brief The tones a palette can hold. The number is the lightness:
		 * 0 is black, 100 is white.
		 */
		enum class Tone : std::uint8_t {
			T0	 = 0,	  ///< Black
			T10	 = 10,	  ///< Lightness 10
			T20	 = 20,	  ///< Lightness 20
			T30	 = 30,	  ///< Lightness 30
			T40	 = 40,	  ///< Lightness 40
			T50	 = 50,	  ///< Lightness 50
			T60	 = 60,	  ///< Lightness 60
			T70	 = 70,	  ///< Lightness 70
			T80	 = 80,	  ///< Lightness 80
			T90	 = 90,	  ///< Lightness 90
			T100 = 100	  ///< White
		};

		private:
		std::string _name;			 ///< Name of the palette ("Neutral", ...)
		std::string _description;	 ///< What the palette is for; may be empty
		std::map<Tone, ExtendedColor> _tones;	 ///< The tones defined so far

		public:
		/**
		 * @brief Create a palette with no name and no tone.
		 */
		Palette(void);

		/**
		 * @brief Create a palette with a name and no tone.
		 * @param name The name of the palette, such as "Neutral".
		 * @param description What the palette is for; may be empty.
		 */
		Palette(const std::string &name, const std::string &description);

		/**
		 * @brief Default destructor.
		 */
		~Palette(void) = default;

		/**
		 * @brief Get the name of the palette.
		 * @return The name; empty when none was given.
		 */
		const std::string &getName(void) const;

		/**
		 * @brief Get the text describing what the palette is for.
		 * @return The description; empty when none was given.
		 */
		const std::string &getDescription(void) const;

		/**
		 * @brief Get every tone defined in the palette.
		 * @return The tones, ordered from the darkest to the lightest;
		 * empty when none was set.
		 */
		const std::map<Tone, ExtendedColor> &getTones(void) const;

		/**
		 * @brief Replace the name of the palette.
		 * @param name The new name.
		 */
		void setName(const std::string &name);

		/**
		 * @brief Replace the text describing what the palette is for.
		 * @param description The new description; may be empty.
		 */
		void setDescription(const std::string &description);

		/**
		 * @brief Define a tone, replacing the color it already has, if any.
		 * @param tone The tone to define.
		 * @param color The color of that tone and its documentation.
		 */
		void setTone(Tone tone, const ExtendedColor &color);

		/**
		 * @brief Define a tone from a plain color value, replacing the
		 * color it already has, if any.
		 * @param tone The tone to define.
		 * @param color The color of that tone, 8 bits per RGBA channel.
		 * @param name The name of the color; empty by default.
		 * @param description What the color is for; empty by default.
		 * @param harmonized True when the color was shifted toward the
		 * theme's source color.
		 */
		void setTone(Tone tone, const utility::graphic::Color32Bit &color,
					 const std::string &name		= "",
					 const std::string &description = "",
					 bool harmonized				= false);

		/**
		 * @brief Tell whether a tone is defined.
		 * @param tone The tone to look for.
		 * @return True when setTone() was called for that tone and it was
		 * not removed since.
		 */
		bool hasTone(Tone tone) const;

		/**
		 * @brief Get the color of a tone.
		 * @param tone The tone to look up.
		 * @return The color of that tone.
		 * @throws std::out_of_range When the tone is not defined. Check
		 * with hasTone() first when unsure.
		 */
		const ExtendedColor &getTone(Tone tone) const;

		/**
		 * @brief Undefine a tone. Does nothing when it is not defined.
		 * @param tone The tone to remove.
		 */
		void removeTone(Tone tone);

		/**
		 * @brief Undefine every tone of the palette.
		 */
		void clearTones(void);
	};

	/**
	 * @brief Every design token of the UI in one object: colors, type
	 * scale, corner shapes, elevation, state layers and motion.
	 *
	 * A token is a named design value (the "primary" color, the "title
	 * large" text style, the "medium" corner radius). Widgets and systems
	 * never hard-code such values: they ask the **active theme** through
	 * the free functions at the end of this header.
	 *
	 * Reading tokens, from anywhere:
	 * @code
	 * const auto color = guillaume::getActiveColor(
	 * 	guillaume::SchemeColorRole::Primary);
	 * const auto &title = guillaume::getTypeToken(
	 * 	guillaume::theme::TypeRole::TitleLarge);		// title.fontSize, in dp
	 * const float radius = guillaume::getShapeRadius(
	 * 	guillaume::theme::ShapeRole::Medium);			// in dp
	 * const float duration = guillaume::getMotionDuration(
	 * 	guillaume::theme::MotionDuration::Short4);		// in seconds
	 * @endcode
	 *
	 * Overriding tokens: copy the active theme, change the copy, activate
	 * it. Do it before the Application is created, so the scenes are built
	 * with the new values.
	 * @code
	 * guillaume::Theme theme = guillaume::getActiveTheme();
	 *
	 * theme.getScheme(guillaume::ThemeSchemeRole::Dark)
	 * 	.setColor(guillaume::SchemeColorRole::Primary,
	 * 			  utility::graphic::Color32Bit(128, 216, 208, 255));
	 * theme.getShapes().setRoundness(2.0f);		// halves the corner radii
	 *
	 * guillaume::setActiveTheme(theme);
	 * guillaume::setActiveScheme(guillaume::ThemeSchemeRole::Dark);
	 * @endcode
	 *
	 * A default-constructed Theme has the Material Design 3 baseline type
	 * scale, shapes, elevations, state layers and motion, but empty color
	 * schemes and palettes. The complete default theme, colors included, is
	 * `defaultTheme`, which is also what the active theme starts as.
	 *
	 * @see getActiveTheme
	 * @see setActiveTheme
	 * @see theme::Typescale
	 */
	class Theme
	{
		private:
		std::string _name;			 ///< Name of the theme, for documentation
		std::string _description;	 ///< What the theme is; may be empty
		std::array<Scheme, ThemeSchemeRole::ThemeSchemeCount>
			_schemes;	 ///< The six color schemes, indexed by ThemeSchemeRole
		std::array<Palette, ThemePaletteRole::ThemePaletteCount>
			_palettes;	  ///< The five palettes, indexed by ThemePaletteRole
		theme::Typescale _typescale;	///< Text styles, one per TypeRole
		theme::ShapeScale _shapes;		///< Corner radii, one per ShapeRole
		theme::ElevationScale
			_elevations;	///< Shadow and tint, one per ElevationLevel
		theme::StateLayerScale
			_stateLayers;	 ///< Overlay opacities, one per StateLayer
		theme::MotionScale _motion;	   ///< Animation durations

		public:
		/**
		 * @brief Create an unnamed theme with the baseline scales and empty
		 * color schemes and palettes.
		 */
		Theme(void);

		/**
		 * @brief Create a named theme with the baseline scales and empty
		 * color schemes and palettes.
		 * @param name The name of the theme.
		 * @param description What the theme is; may be empty.
		 */
		Theme(const std::string &name, const std::string &description);

		/**
		 * @brief Virtual destructor.
		 */
		virtual ~Theme(void) = default;

		/**
		 * @brief Get the name of the theme.
		 * @return The name; empty when none was given.
		 */
		const std::string &getName(void) const;

		/**
		 * @brief Get the text describing the theme.
		 * @return The description; empty when none was given.
		 */
		const std::string &getDescription(void) const;

		/**
		 * @brief Replace the name of the theme.
		 * @param name The new name.
		 */
		void setName(const std::string &name);

		/**
		 * @brief Replace the text describing the theme.
		 * @param description The new description; may be empty.
		 */
		void setDescription(const std::string &description);

		/**
		 * @brief Get one of the six color schemes.
		 * @param role Which scheme. Must not be `ThemeSchemeCount`.
		 * @return The scheme, read-only.
		 */
		const Scheme &getScheme(ThemeSchemeRole role) const;

		/**
		 * @brief Get one of the six color schemes, to change its colors in
		 * place.
		 * @param role Which scheme. Must not be `ThemeSchemeCount`.
		 * @return Mutable reference to the scheme.
		 */
		Scheme &getScheme(ThemeSchemeRole role);

		/**
		 * @brief Get one of the five tonal palettes.
		 * @param role Which palette. Must not be `ThemePaletteCount`.
		 * @return The palette, read-only.
		 */
		const Palette &getPalette(ThemePaletteRole role) const;

		/**
		 * @brief Get one of the five tonal palettes, to change its tones in
		 * place.
		 * @param role Which palette. Must not be `ThemePaletteCount`.
		 * @return Mutable reference to the palette.
		 */
		Palette &getPalette(ThemePaletteRole role);

		/**
		 * @brief Replace one of the six color schemes as a whole.
		 * @param role Which scheme to replace. Must not be
		 * `ThemeSchemeCount`.
		 * @param scheme The scheme to copy in.
		 */
		void setScheme(ThemeSchemeRole role, const Scheme &scheme);

		/**
		 * @brief Replace one of the five tonal palettes as a whole.
		 * @param role Which palette to replace. Must not be
		 * `ThemePaletteCount`.
		 * @param palette The palette to copy in.
		 */
		void setPalette(ThemePaletteRole role, const Palette &palette);

		/**
		 * @brief Get the text styles of the theme.
		 * @return The type scale, read-only: one theme::TypeToken per
		 * theme::TypeRole.
		 */
		const theme::Typescale &getTypescale(void) const;

		/**
		 * @brief Get the text styles of the theme, to change a role in
		 * place with theme::Typescale::set.
		 * @return Mutable reference to the type scale.
		 */
		theme::Typescale &getTypescale(void);

		/**
		 * @brief Replace every text style of the theme at once.
		 * @param typescale The type scale to copy in.
		 */
		void setTypescale(const theme::Typescale &typescale);

		/**
		 * @brief Get the corner radii of the theme.
		 * @return The shape scale, read-only.
		 */
		const theme::ShapeScale &getShapes(void) const;

		/**
		 * @brief Get the corner radii of the theme, to change the roundness
		 * in place.
		 * @return Mutable reference to the shape scale.
		 */
		theme::ShapeScale &getShapes(void);

		/**
		 * @brief Get the elevation levels of the theme (shadow and surface
		 * tint).
		 * @return The elevation scale, read-only.
		 */
		const theme::ElevationScale &getElevations(void) const;

		/**
		 * @brief Get the elevation levels of the theme, to change a level
		 * in place.
		 * @return Mutable reference to the elevation scale.
		 */
		theme::ElevationScale &getElevations(void);

		/**
		 * @brief Get the state-layer opacities of the theme (hovered,
		 * focused, pressed, ...).
		 * @return The state-layer scale, read-only.
		 */
		const theme::StateLayerScale &getStateLayers(void) const;

		/**
		 * @brief Get the state-layer opacities of the theme, to change one
		 * in place.
		 * @return Mutable reference to the state-layer scale.
		 */
		theme::StateLayerScale &getStateLayers(void);

		/**
		 * @brief Get the animation durations of the theme.
		 * @return The motion scale, read-only.
		 */
		const theme::MotionScale &getMotion(void) const;

		/**
		 * @brief Get the animation durations of the theme, to change one in
		 * place.
		 * @return Mutable reference to the motion scale.
		 */
		theme::MotionScale &getMotion(void);
	};

	/**
	 * @brief The theme of the library: Material Design 3 baseline colors
	 * (six schemes, five palettes) and scales.
	 *
	 * It is a constant. The active theme starts with the same content; copy
	 * either one to make a custom theme.
	 * @see initializeDefaultTheme
	 */
	extern const Theme defaultTheme;

	/**
	 * @brief Get the theme every widget and system reads its tokens from.
	 *
	 * There is one active theme for the whole program. Until
	 * setActiveTheme() is called it has the content of defaultTheme.
	 * @return The active theme, read-only. The reference stays valid;
	 * setActiveTheme() changes the object it designates.
	 */
	const Theme &getActiveTheme(void);

	/**
	 * @brief Replace the active theme by a copy of another theme.
	 *
	 * This is how tokens are overridden: copy getActiveTheme(), modify the
	 * copy, pass it here (see the example in Theme).
	 *
	 * Call it before building a scene: widgets resolve their tokens when
	 * they are built or restyled, so a widget that already exists keeps
	 * the values it resolved until it is next restyled. Nothing is marked
	 * changed by this call.
	 * @param theme The theme to copy and activate. Later changes to
	 * `theme` have no effect on the active one.
	 */
	void setActiveTheme(const Theme &theme);

	/**
	 * @brief Get the color scheme in use: the scheme of the active theme
	 * selected by setActiveScheme().
	 * @return The active scheme, read-only.
	 */
	const Scheme &getActiveScheme(void);

	/**
	 * @brief Get the color the active scheme gives to a role.
	 *
	 * This is the call widgets and systems use for every color:
	 * `getActiveColor(SchemeColorRole::OnSurface)`.
	 * @param role The role to resolve. Must not be
	 * `SchemeColorRole::Count`.
	 * @return The color, 8 bits per RGBA channel, by value.
	 */
	utility::graphic::Color32Bit getActiveColor(SchemeColorRole role);

	/**
	 * @brief Choose which of the six schemes of the active theme is in use,
	 * for instance to switch between light and dark.
	 *
	 * The choice is program-wide, starts as `Light`, and is kept when the
	 * active theme is replaced. As with setActiveTheme(), widgets that
	 * already exist keep their colors until they are restyled.
	 * @param role The scheme to use. Must not be `ThemeSchemeCount`.
	 */
	void setActiveScheme(ThemeSchemeRole role);

	/**
	 * @brief Get the text styles of the active theme.
	 * @return The active type scale, read-only.
	 */
	const theme::Typescale &getActiveTypescale(void);

	/**
	 * @brief Get the text style the active theme gives to a role: font
	 * family, weight, size, line height and letter spacing.
	 *
	 * @code
	 * const auto &body = getTypeToken(theme::TypeRole::BodyMedium);
	 * text.setFontSize(body.fontSize);	   // 14 dp in the default theme
	 * @endcode
	 * @param role The typescale role to resolve.
	 * @return The token of that role, read-only; sizes are in dp.
	 * @throws std::out_of_range When `role` is `theme::TypeRole::Count`.
	 */
	const theme::TypeToken &getTypeToken(theme::TypeRole role);

	/**
	 * @brief Get the corner radius the active theme gives to a shape role.
	 * @param role The shape role to resolve.
	 * @return The corner radius, in dp. `theme::ShapeRole::Full` returns
	 * theme::ShapeScale::FullRadius, a value large enough to be clamped to
	 * a pill or a circle by the renderers.
	 * @throws std::out_of_range When `role` is `theme::ShapeRole::Count`.
	 */
	float getShapeRadius(theme::ShapeRole role);

	/**
	 * @brief Get the opacity the active theme gives to a state layer, as
	 * an alpha channel value.
	 *
	 * A state layer is the translucent overlay drawn on a widget that is
	 * hovered, focused, pressed or dragged.
	 * @param layer The state layer to resolve.
	 * @return The alpha, 0 (transparent) to 255 (opaque).
	 * @throws std::out_of_range When `layer` is `theme::StateLayer::Count`.
	 */
	std::uint8_t getStateLayerAlpha(theme::StateLayer layer);

	/**
	 * @brief Get the shadow and tint the active theme gives to an
	 * elevation level.
	 * @param level The elevation level to resolve.
	 * @return The token of that level, read-only.
	 * @throws std::out_of_range When `level` is
	 * `theme::ElevationLevel::Count`.
	 */
	const theme::ElevationToken &getElevationToken(theme::ElevationLevel level);

	/**
	 * @brief Get the background color of a surface raised to an elevation
	 * level.
	 *
	 * It is the active scheme's `Surface` color with its `SurfaceTint`
	 * blended over it at the tint opacity of the level: the higher the
	 * level, the stronger the tint.
	 * @param level The elevation level of the surface.
	 * @return The tinted surface color; the plain `Surface` color for
	 * `Level0` in the default theme.
	 * @throws std::out_of_range When `level` is
	 * `theme::ElevationLevel::Count`.
	 */
	utility::graphic::Color32Bit
		getElevatedSurfaceColor(theme::ElevationLevel level);

	/**
	 * @brief Get the length the active theme gives to a duration token.
	 * @param duration The duration token to resolve.
	 * @return The duration, in seconds (`Short4` is 0.2 in the default
	 * theme).
	 * @throws std::out_of_range When `duration` is
	 * `theme::MotionDuration::Count`.
	 */
	float getMotionDuration(theme::MotionDuration duration);

}	 // namespace guillaume
