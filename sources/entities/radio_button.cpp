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

#include "guillaume/entities/radio_button.hpp"

#include <memory>
#include <vector>

#include "guillaume/ecs/entity_filler.hpp"

#include "guillaume/components/bound.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/ellipse.hpp"
#include "guillaume/components/transform.hpp"

#include "guillaume/entities/content_helpers.hpp"
#include "guillaume/entities/placement_helpers.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/style_helpers.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::entities
{
	namespace
	{
		/**
		 * @brief Inner filled dot of a selected radio button.
		 */
		class RadioDot:
			public ecs::EntityFiller<components::Transform, components::Bound,
									 components::Color, components::Ellipse>
		{
			public:
			/**
			 * @brief Construct a radio dot.
			 * @param registry The component registry.
			 * @param color The dot color.
			 */
			RadioDot(ecs::ComponentRegistry &registry,
					 const utility::graphic::Color32Bit &color)
				: ecs::EntityFiller<components::Transform, components::Bound,
									components::Color, components::Ellipse>(
					  registry)
			{
				getComponentRegistry()
					.getComponent<components::Bound>(getIdentifier())
					.setWidth(8.0f)
					.setHeight(8.0f);

				getComponentRegistry()
					.getComponent<components::Color>(getIdentifier())
					.setColor(color);
			}

			/**
			 * @brief Default destructor.
			 */
			~RadioDot(void) override = default;
		};
	}	 // namespace

	RadioButton::Builder::Builder(ecs::ComponentRegistry &componentRegistry,
								  ecs::EntityRegistry &entityRegistry)
		: EntityBuilderBase<RadioButton>(componentRegistry, entityRegistry)
	{
		reset();
	}

	std::shared_ptr<RadioButton> RadioButton::Builder::buildEntity(void)
	{
		auto entity = std::make_shared<RadioButton>(
			this->getComponentRegistry(), _config, _variant, _label);
		return entity;
	}

	void RadioButton::Builder::reset(void)
	{
		_config	 = SurfaceConfig();
		_variant = Variant::Enabled;
		_label.clear();

		_config.borderRadius = 10.0f;
		_config.axis		 = components::Layout::Axis::Horizontal;
		_config.mainAxisAlignment =
			components::Layout::MainAxisAlignment::Start;
		_config.crossAxisAlignment =
			components::Layout::CrossAxisAlignment::Center;
		_config.spacing = 8.0f;
		_config.padding = 0.0f;
		_config.setFixedWidth(20.0f);
		_config.setFixedHeight(20.0f);
	}

	RadioButton::Builder &
		RadioButton::Builder::withPose(const utility::graphic::PoseF &pose)
	{
		_config.pose = pose;
		return *this;
	}

	RadioButton::Builder &RadioButton::Builder::withVariant(Variant variant)
	{
		_variant = variant;
		return *this;
	}

	RadioButton::Builder &
		RadioButton::Builder::withLabel(const std::string &label)
	{
		_label = label;
		return *this;
	}

	std::shared_ptr<RadioButton> RadioButton::Director::makeRadioButton(
		Builder &builder, std::shared_ptr<ecs::Entity> parent, Variant variant,
		const std::string &label)
	{
		return builder.withVariant(variant).withLabel(label).registerEntity(
			parent);
	}

	RadioButton::RadioButton(ecs::ComponentRegistry &registry,
							 const SurfaceConfig &config, Variant variant,
							 const std::string &label)
		: SurfaceBase<components::Selectable, components::Ring>(registry,
																config)
		, _variant(variant)
		, _label(label)
		, _dot()
		, _labelEntity()
	{
		getComponentRegistry()
			.getComponent<components::Selectable>(getIdentifier())
			.setOnSelectionChangedHandler([this](bool) {
				applyState();
			});

		applyVariant();
	}

	RadioButton::~RadioButton(void)
	{
	}

	void RadioButton::applyVariant(void)
	{
		auto &ring = getComponentRegistry().getComponent<components::Ring>(
			getIdentifier());

		ring.setThickness(2.0f);

		const bool disabled = (_variant == Variant::Disabled);
		const bool errored	= (_variant == Variant::Error);
		const bool selected =
			getComponentRegistry()
				.getComponent<components::Selectable>(getIdentifier())
				.isSelected();

		if (disabled) {
			setColor(withAlpha(schemeColor(SchemeColorRole::OnSurface), 97U));
		} else if (errored) {
			setColor(schemeColor(SchemeColorRole::Error));
		} else if (selected) {
			setColor(schemeColor(SchemeColorRole::Primary));
		} else {
			setColor(schemeColor(SchemeColorRole::OnSurfaceVariant));
		}

		applyState();
	}

	void RadioButton::applyState(void)
	{
		const bool selected =
			getComponentRegistry()
				.getComponent<components::Selectable>(getIdentifier())
				.isSelected();

		const bool disabled = (_variant == Variant::Disabled);
		const bool errored	= (_variant == Variant::Error);

		const auto dotColor = disabled
			? withAlpha(schemeColor(SchemeColorRole::OnSurface), 97U)
			: (errored ? schemeColor(SchemeColorRole::Error)
					   : schemeColor(SchemeColorRole::Primary));

		const auto &pose =
			getComponentRegistry()
				.getComponent<components::Transform>(getIdentifier())
				.getPose();
		const auto &bound =
			getComponentRegistry().getComponent<components::Bound>(
				getIdentifier());

		if (_dot != nullptr) {
			getComponentRegistry()
				.getComponent<components::Color>(_dot->getIdentifier())
				.setColor(selected ? dotColor : transparentColor());

			if (selected) {
				centerChild(getComponentRegistry(), getIdentifier(),
							_dot->getIdentifier());
			}
		}

		if (_labelEntity != nullptr) {
			if (disabled) {
				_labelEntity->setColor(
					withAlpha(schemeColor(SchemeColorRole::OnSurface), 97U));
			} else if (errored) {
				_labelEntity->setColor(schemeColor(SchemeColorRole::Error));
			} else {
				_labelEntity->setColor(schemeColor(SchemeColorRole::OnSurface));
			}
		}
	}

	RadioButton &RadioButton::setVariant(Variant variant)
	{
		_variant = variant;
		applyVariant();
		return *this;
	}

	RadioButton::Variant RadioButton::getVariant(void) const
	{
		return _variant;
	}

	RadioButton &RadioButton::setLabel(const std::string &label)
	{
		_label = label;

		if (_labelEntity != nullptr) {
			_labelEntity->setContent(label);
		}

		applyState();

		return *this;
	}

	bool RadioButton::isSelected(void) const
	{
		return getComponentRegistry()
			.getComponent<components::Selectable>(getIdentifier())
			.isSelected();
	}

	RadioButton &RadioButton::select(bool selected)
	{
		getComponentRegistry()
			.getComponent<components::Selectable>(getIdentifier())
			.setSelected(selected);

		applyVariant();

		return *this;
	}

	void RadioButton::initialize(void)
	{
		SurfaceBase::initialize();

		_dot = std::make_shared<RadioDot>(
			getComponentRegistry(), schemeColor(SchemeColorRole::Primary));
		_dot->setParent(shared_from_this());
		this->addEntity(_dot);

		if (!_label.empty()) {
			_labelEntity = buildText(getComponentRegistry(), *this,
									 shared_from_this(), _label, 14.0f,
									 schemeColor(SchemeColorRole::OnSurface));
		}

		// The dot is positioned manually (not through the layout) so that it
		// stays centered; the label is arranged by the surface layout.
		std::vector<std::shared_ptr<ecs::Entity>> children;
		if (_labelEntity != nullptr) {
			children.push_back(_labelEntity);
		}
		setChildren(children);

		applyVariant();
	}

	void RadioButton::update(void)
	{
		SurfaceBase::update();
		applyVariant();
	}

}	 // namespace guillaume::entities
