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

#include "guillaume/entities/card.hpp"

#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/style_helpers.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::entities
{
	Card::Builder::Builder(ecs::ComponentRegistry &componentRegistry,
						   ecs::EntityRegistry &entityRegistry)
		: EntityBuilderBase<Card>(componentRegistry, entityRegistry)
	{
		reset();
	}

	std::shared_ptr<Card> Card::Builder::buildEntity(void)
	{
		auto entity = std::make_shared<Card>(this->getComponentRegistry(),
											 _config, _variant);
		return entity;
	}

	void Card::Builder::reset(void)
	{
		_config				 = SurfaceConfig();
		_variant			 = Variant::Elevated;
		_config.borderRadius = 12.0f;
		_config.padding		 = 16.0f;
		_config.spacing		 = 8.0f;
		_config.axis		 = components::Layout::Axis::Vertical;
		_config.setFixedWidth(240.0f);
	}

	Card::Builder &Card::Builder::withPose(const utility::graphic::PoseF &pose)
	{
		_config.pose = pose;
		return *this;
	}

	Card::Builder &Card::Builder::withVariant(Variant variant)
	{
		_variant = variant;
		return *this;
	}

	Card::Builder &Card::Builder::withWidth(float width)
	{
		_config.hasFixedWidth = true;
		_config.fixedWidth	  = width;
		return *this;
	}

	Card::Builder &Card::Builder::withHeight(float height)
	{
		_config.hasFixedHeight = true;
		_config.fixedHeight	   = height;
		return *this;
	}

	Card::Builder &Card::Builder::withBorderRadius(float radius)
	{
		_config.borderRadius = radius;
		return *this;
	}

	std::shared_ptr<Card> Card::Director::makeCard(
		Builder &builder, std::shared_ptr<ecs::Entity> parent, Variant variant)
	{
		return builder.withVariant(variant).registerEntity(parent);
	}

	Card::Card(ecs::ComponentRegistry &registry, const SurfaceConfig &config,
			   Variant variant)
		: SurfaceBase<components::Elevation>(registry, config)
		, _variant(variant)
	{
		applyVariant();
	}

	Card::~Card(void)
	{
	}

	void Card::applyVariant(void)
	{
		auto &borders =
			getComponentRegistry().getComponent<components::Borders>(
				getIdentifier());
		auto &elevation =
			getComponentRegistry().getComponent<components::Elevation>(
				getIdentifier());

		switch (_variant) {
			case Variant::Elevated: {
				setColor(schemeColor(SchemeColorRole::SurfaceContainerLow));
				borders.setColor(transparentColor());
				elevation.setLevel(1.0f).setSpread(6.0f);
				break;
			}
			case Variant::Filled: {
				setColor(schemeColor(SchemeColorRole::SurfaceContainerHighest));
				borders.setColor(transparentColor());
				elevation.setLevel(0.0f).setSpread(0.0f);
				break;
			}
			case Variant::Outlined:
			default: {
				setColor(transparentColor());
				borders.setColor(schemeColor(SchemeColorRole::OutlineVariant));
				elevation.setLevel(0.0f).setSpread(0.0f);
				break;
			}
		}
	}

	Card &Card::setVariant(Variant variant)
	{
		_variant = variant;
		applyVariant();
		return *this;
	}

	Card::Variant Card::getVariant(void) const
	{
		return _variant;
	}

	void Card::initialize(void)
	{
		SurfaceBase::initialize();
		applyVariant();
	}

	void Card::update(void)
	{
		SurfaceBase::update();
		applyVariant();
	}

}	 // namespace guillaume::entities
