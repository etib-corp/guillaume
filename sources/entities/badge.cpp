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

#include <algorithm>
#include <memory>
#include <stdexcept>

#include "guillaume/entities/badge.hpp"

#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/content_helpers.hpp"
#include "guillaume/entities/style_helpers.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::entities
{
	namespace
	{
		constexpr float DotSize			  = 6.0f;
		constexpr float SmallSize		  = 16.0f;
		constexpr float LargeSize		  = 20.0f;
		constexpr float HorizontalPadding = 3.0f;	 ///< Paper Badge padding.
		constexpr float LabelScale		  = 0.5f;	 ///< Paper font = size/2.
	}	 // namespace

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
		: SurfaceBase<components::Animation>(registry, config)
		, _variant(variant)
		, _label(label)
		, _labelEntity()
	{
		getComponentRegistry()
			.getComponent<components::Animation>(getIdentifier())
			.setDuration(getMotionDuration(theme::MotionDuration::Short3))
			.setEasing(theme::MotionScale::standard())
			.setOnSampleHandler([this](float progress) {
				const float target = _visible ? 1.0f : 0.0f;
				_opacity = _fadeFrom + (target - _fadeFrom) * progress;
				applyVariant();
			});
		applyVariant();
	}

	Badge::~Badge(void)
	{
	}

	float Badge::getVariantSize(void) const
	{
		switch (_variant) {
			case Variant::Dot:
				return DotSize;
			case Variant::Small:
				return SmallSize;
			case Variant::Large:
			default:
				return LargeSize;
		}
	}

	float Badge::getSize(void) const
	{
		return _size.value_or(getVariantSize());
	}

	void Badge::applyVariant(void)
	{
		const float size = getSize();

		auto config			= getSurfaceConfig();
		config.borderRadius = getShapeRadius(theme::ShapeRole::Full);
		config.padding		= 0.0f;
		config.spacing		= 0.0f;

		float width = size;
		if (_variant != Variant::Dot && _labelEntity != nullptr) {
			const float labelWidth = getComponentRegistry()
										 .getComponent<components::Bound>(
											 _labelEntity->getIdentifier())
										 .getWidth();
			width = std::max(size, labelWidth + 2.0f * HorizontalPadding);
		}
		config.hasFixedWidth  = true;
		config.fixedWidth	  = width;
		config.hasFixedHeight = true;
		config.fixedHeight	  = size;

		setSurfaceConfig(config);

		const auto fade = [this](const utility::graphic::Color32Bit &color) {
			return withAlpha(
				color,
				static_cast<std::uint8_t>(
					static_cast<float>(color.getAlpha()) * _opacity + 0.5f));
		};
		setColor(fade(
			_containerColor.value_or(schemeColor(SchemeColorRole::Error))));

		if (_labelEntity != nullptr) {
			_labelEntity->setFontSize(size * LabelScale);
			_labelEntity->setColor(fade(
				_contentColor.value_or(schemeColor(SchemeColorRole::OnError))));
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

	Badge &Badge::setSize(float size)
	{
		if (size <= 0.0f) {
			throw std::invalid_argument("Badge size must be positive");
		}
		_size = size;
		applyVariant();
		return *this;
	}

	Badge &Badge::clearSize(void)
	{
		_size.reset();
		applyVariant();
		return *this;
	}

	Badge &Badge::setContainerColor(const utility::graphic::Color32Bit &color)
	{
		_containerColor = color;
		applyVariant();
		return *this;
	}

	Badge &Badge::setContentColor(const utility::graphic::Color32Bit &color)
	{
		_contentColor = color;
		applyVariant();
		return *this;
	}

	Badge &Badge::setVisible(bool visible)
	{
		if (_visible == visible) {
			return *this;
		}
		_visible  = visible;
		_fadeFrom = _opacity;
		getComponentRegistry()
			.getComponent<components::Animation>(getIdentifier())
			.play();
		return *this;
	}

	bool Badge::isVisible(void) const
	{
		return _visible;
	}

	float Badge::getOpacity(void) const
	{
		return _opacity;
	}

	void Badge::initialize(void)
	{
		SurfaceBase::initialize();

		if (_variant != Variant::Dot && !_label.empty()) {
			_labelEntity =
				buildText(getComponentRegistry(), *this, shared_from_this(),
						  _label, theme::TypeRole::LabelSmall,
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
