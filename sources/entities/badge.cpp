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

#include <memory>
#include <stdexcept>

#include "guillaume/entities/badge.hpp"

#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/content_helpers.hpp"
#include "guillaume/entities/style_helpers.hpp"
#include "guillaume/systems/badge.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::entities
{
	Badge::Builder::Builder(ecs::ComponentRegistry &componentRegistry,
							ecs::EntityRegistry &entityRegistry)
		: EntityBuilderBase<Badge>(componentRegistry, entityRegistry)
	{
		reset();
	}

	std::shared_ptr<Badge> Badge::Builder::buildEntity(void)
	{
		return std::make_shared<Badge>(this->getComponentRegistry(), _config,
									   _variant, _label);
	}

	void Badge::Builder::reset(void)
	{
		_config		 = SurfaceConfig();
		_variant	 = Variant::Large;
		_label		 = "";
		_config.axis = components::Layout::Axis::Horizontal;
		_config.mainAxisAlignment =
			components::Layout::MainAxisAlignment::Center;
		_config.crossAxisAlignment =
			components::Layout::CrossAxisAlignment::Center;
	}

	Badge::Builder &
		Badge::Builder::withPose(const utility::graphic::PoseF &pose)
	{
		_config.pose = pose;
		return *this;
	}

	Badge::Builder &Badge::Builder::withVariant(Variant variant)
	{
		_variant = variant;
		return *this;
	}

	Badge::Builder &Badge::Builder::withLabel(const std::string &label)
	{
		_label = label;
		return *this;
	}

	std::shared_ptr<Badge>
		Badge::Builder::makeBadge(std::shared_ptr<ecs::Entity> parent,
								  const std::string &label, Variant variant)
	{
		return withLabel(label).withVariant(variant).registerEntity(parent);
	}

	Badge::Badge(ecs::ComponentRegistry &registry, const SurfaceConfig &config,
				 Variant variant, const std::string &label)
		: SurfaceBase<components::Animation, components::Badge>(registry,
																config)
	{
		getComponentRegistry()
			.getComponent<components::Badge>(getIdentifier())
			.setVariant(variant)
			.setLabel(label);
		systems::Badge::install(getComponentRegistry(), getIdentifier());
		systems::Badge::apply(getComponentRegistry(), getIdentifier());
	}

	Badge::~Badge(void)
	{
	}

	float Badge::getSize(void) const
	{
		return systems::Badge::sizeOf(getComponentRegistry(), getIdentifier());
	}

	Badge &Badge::setVariant(Variant variant)
	{
		getComponentRegistry()
			.getComponent<components::Badge>(getIdentifier())
			.setVariant(variant);
		systems::Badge::apply(getComponentRegistry(), getIdentifier());
		return *this;
	}

	Badge::Variant Badge::getVariant(void) const
	{
		return getComponentRegistry()
			.getComponent<components::Badge>(getIdentifier())
			.getVariant();
	}

	Badge &Badge::setLabel(const std::string &label)
	{
		getComponentRegistry()
			.getComponent<components::Badge>(getIdentifier())
			.setLabel(label);
		systems::Badge::apply(getComponentRegistry(), getIdentifier());
		return *this;
	}

	Badge &Badge::setSize(float size)
	{
		if (size <= 0.0f) {
			throw std::invalid_argument("Badge size must be positive");
		}
		getComponentRegistry()
			.getComponent<components::Badge>(getIdentifier())
			.setSize(size);
		systems::Badge::apply(getComponentRegistry(), getIdentifier());
		return *this;
	}

	Badge &Badge::clearSize(void)
	{
		getComponentRegistry()
			.getComponent<components::Badge>(getIdentifier())
			.setSize(std::nullopt);
		systems::Badge::apply(getComponentRegistry(), getIdentifier());
		return *this;
	}

	Badge &Badge::setContainerColor(const utility::graphic::Color32Bit &color)
	{
		getComponentRegistry()
			.getComponent<components::Badge>(getIdentifier())
			.setContainerColor(color);
		systems::Badge::apply(getComponentRegistry(), getIdentifier());
		return *this;
	}

	Badge &Badge::setContentColor(const utility::graphic::Color32Bit &color)
	{
		getComponentRegistry()
			.getComponent<components::Badge>(getIdentifier())
			.setContentColor(color);
		systems::Badge::apply(getComponentRegistry(), getIdentifier());
		return *this;
	}

	Badge &Badge::setVisible(bool visible)
	{
		systems::Badge::setVisible(getComponentRegistry(), getIdentifier(),
								   visible);
		return *this;
	}

	bool Badge::isVisible(void) const
	{
		return getComponentRegistry()
			.getComponent<components::Badge>(getIdentifier())
			.isVisible();
	}

	float Badge::getOpacity(void) const
	{
		return getComponentRegistry()
			.getComponent<components::Badge>(getIdentifier())
			.getOpacity();
	}

	void Badge::initialize(void)
	{
		auto &registry = getComponentRegistry();
		auto &badge = registry.getComponent<components::Badge>(getIdentifier());

		if (systems::Badge::needsLabel(registry, getIdentifier())) {
			const auto label =
				buildText(registry, *this, shared_from_this(), badge.getLabel(),
						  theme::TypeRole::LabelSmall,
						  schemeColor(SchemeColorRole::OnError));

			if (label != nullptr) {
				badge.setLabelEntity(label->getIdentifier());
				setChildren({ label });
			}
		}

		systems::Badge::apply(registry, getIdentifier());
	}

}	 // namespace guillaume::entities
