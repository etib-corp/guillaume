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

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity.hpp"

#include "guillaume/components/overlay.hpp"

namespace guillaume::entities
{

	/**
	 * @brief Set the visibility of an entity's overlay component.
	 * @param registry The component registry.
	 * @param identifier The owning entity identifier.
	 * @param visible The new visibility.
	 */
	inline void setOverlayVisible(ecs::ComponentRegistry &registry,
								  const ecs::Entity::Identifier &identifier,
								  bool visible)
	{
		registry.getComponent<components::Overlay>(identifier)
			.setVisible(visible);
	}

	/**
	 * @brief Get the visibility of an entity's overlay component.
	 * @param registry The component registry.
	 * @param identifier The owning entity identifier.
	 * @return True when the overlay is visible.
	 */
	inline bool isOverlayVisible(const ecs::ComponentRegistry &registry,
								 const ecs::Entity::Identifier &identifier)
	{
		return registry.getComponent<components::Overlay>(identifier)
			.isVisible();
	}

}	 // namespace guillaume::entities
