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

#include <optional>

#include "guillaume/ecs/component.hpp"
#include "guillaume/ecs/entity.hpp"

namespace guillaume::components
{

	/**
	 * @brief Configuration of a Material 3 floating action button.
	 *
	 * systems::FloatingActionButton turns it, with components::Button, into the
	 * button's colors, shadow, size and the placement of its icon and label.
	 */
	class FloatingActionButton: public ecs::Component
	{
		public:
		/**
		 * @brief Color variant.
		 */
		enum class Variant { Surface, Primary, Secondary, Tertiary };

		/**
		 * @brief Shadow mode.
		 */
		enum class Mode { Elevated, Flat };

		/**
		 * @brief Size.
		 */
		enum class Size { Small, Medium, Large };

		private:
		Variant _variant { Variant::Primary };	  ///< Color variant
		Size _size { Size::Medium };			  ///< Size
		Mode _mode { Mode::Elevated };			  ///< Shadow mode
		std::optional<float> _customSize {};	  ///< Custom container size
		bool _lowered { false };	///< Whether the button is lowered

		public:
		/**
		 * @brief Default constructor.
		 */
		FloatingActionButton(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~FloatingActionButton(void) = default;

		/** @brief Get: color variant. @return The value. */
		Variant getVariant(void) const noexcept;

		/** @brief Set: color variant. @param variant The value. @return It. */
		FloatingActionButton &setVariant(Variant variant);

		/** @brief Get: size. @return The value. */
		Size getSize(void) const noexcept;

		/** @brief Set: size. @param size The value. @return It. */
		FloatingActionButton &setSize(Size size);

		/** @brief Get: shadow mode. @return The value. */
		Mode getMode(void) const noexcept;

		/** @brief Set: shadow mode. @param mode The value. @return It. */
		FloatingActionButton &setMode(Mode mode);

		/** @brief Get: custom container size. @return The value. */
		const std::optional<float> &getCustomSize(void) const noexcept;

		/** @brief Set: custom container size. @param size The value. @return
		 * It. */
		FloatingActionButton &setCustomSize(const std::optional<float> &size);

		/** @brief Get: whether the button is lowered. @return The value. */
		bool isLowered(void) const noexcept;

		/** @brief Set: whether the button is lowered. @param lowered The value.
		 * @return It. */
		FloatingActionButton &setLowered(bool lowered);
	};

}	 // namespace guillaume::components
