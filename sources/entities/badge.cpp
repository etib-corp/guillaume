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

#include "guillaume/entities/badge.hpp"

#include "guillaume/entities/content_helpers.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/style_helpers.hpp"
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
		auto entity = std::make_shared<Badge>(this->getComponentRegistry(),
											  _config, _variant, _label);
		return entity;
	}

	void Badge::Builder::reset(void)
	{
		_config		 = SurfaceConfig();
		_variant	 = Variant::Dot;
		_label		 = "1";
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
		Badge::Director::makeBadge(Builder &builder,
								   std::shared_ptr<ecs::Entity> parent,
								   const std::string &label, Variant variant)
	{
		return builder.withLabel(label).withVariant(variant).registerEntity(
			parent);
	}

	Badge::Badge(ecs::ComponentRegistry &registry, const SurfaceConfig &config,
				 Variant variant, const std::string &label)
		: SurfaceBase<>(registry, config)
		, _variant(variant)
		, _label(label)
		, _labelEntity()
	{
		applyVariant();
	}

	Badge::~Badge(void)
	{
	}

	float Badge::getVariantSize(void) const
	{
		switch (_variant) {
			case Variant::Dot:
				return 6.0f;
			case Variant::Small:
				return 16.0f;
			case Variant::Large:
				return 24.0f;
			default:
				return 6.0f;
		}
	}

	void Badge::applyVariant(void)
	{
		const float size = getVariantSize();

		auto config			= getSurfaceConfig();
		config.borderRadius = size * 0.5f;
		config.padding		= 0.0f;
		config.spacing		= 0.0f;

		if (_variant == Variant::Dot) {
			config.hasFixedWidth  = true;
			config.fixedWidth	  = size;
			config.hasFixedHeight = true;
			config.fixedHeight	  = size;
		} else {
			const float labelWidth = _labelEntity != nullptr
				? getComponentRegistry()
					  .getComponent<components::Bound>(
						  _labelEntity->getIdentifier())
					  .getWidth()
				: 0.0f;
			const float width =
				size + (_label.empty() ? 0.0f : labelWidth + size * 0.5f);

			config.hasFixedWidth  = true;
			config.fixedWidth	  = width;
			config.hasFixedHeight = true;
			config.fixedHeight	  = size;
		}

		setSurfaceConfig(config);

		setColor(schemeColor(SchemeColorRole::Error));

		if (_labelEntity != nullptr) {
			_labelEntity->setColor(schemeColor(SchemeColorRole::OnError));
		}
	}

	Badge &Badge::setVariant(Variant variant)
	{
		_variant = variant;
		applyVariant();
		return *this;
	}

	Badge::Variant Badge::getVariant(void) const
	{
		return _variant;
	}

	Badge &Badge::setLabel(const std::string &label)
	{
		_label = label;

		if (_labelEntity != nullptr) {
			_labelEntity->setContent(label);
		}

		applyVariant();

		return *this;
	}

	void Badge::initialize(void)
	{
		SurfaceBase::initialize();

		if (_variant != Variant::Dot && !_label.empty()) {
			_labelEntity =
				buildText(getComponentRegistry(), *this, shared_from_this(),
						  _label, getVariantSize() * 0.75f,
						  schemeColor(SchemeColorRole::OnError));

			if (_labelEntity != nullptr) {
				setChildren({ _labelEntity });
			}
		}

		applyVariant();
	}

	void Badge::update(void)
	{
		SurfaceBase::update();
		applyVariant();
	}

}	 // namespace guillaume::entities
