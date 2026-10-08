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

#include "guillaume/entities/carousel.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <memory>
#include <vector>

#include <utility/math/vector.hpp>

#include "guillaume/ecs/entity_filler.hpp"

#include "guillaume/components/borders.hpp"
#include "guillaume/components/bound.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/transform.hpp"

#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/style_helpers.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::entities
{
	namespace
	{
		constexpr float ItemRadius	= 12.0f;
		constexpr float ItemSpacing = 8.0f;

		/**
		 * @brief Placeholder item child of a carousel.
		 */
		class CarouselItem:
			public ecs::EntityFiller<components::Transform, components::Bound,
									 components::Color, components::Borders>
		{
			public:
			/**
			 * @brief Construct a carousel item.
			 * @param registry The component registry.
			 * @param width The item width.
			 * @param height The item height.
			 */
			CarouselItem(ecs::ComponentRegistry &registry, float width,
						 float height)
				: ecs::EntityFiller<components::Transform, components::Bound,
									components::Color, components::Borders>(
					  registry)
			{
				getComponentRegistry()
					.getComponent<components::Bound>(getIdentifier())
					.setWidth(width)
					.setHeight(height);
				getComponentRegistry()
					.getComponent<components::Borders>(getIdentifier())
					.setBorderRadius(ItemRadius);
			}

			/**
			 * @brief Default destructor.
			 */
			~CarouselItem(void) override = default;
		};
	}	 // namespace

	Carousel::Builder::Builder(ecs::ComponentRegistry &componentRegistry,
							   ecs::EntityRegistry &entityRegistry)
		: EntityBuilderBase<Carousel>(componentRegistry, entityRegistry)
	{
		reset();
	}

	std::shared_ptr<Carousel> Carousel::Builder::buildEntity(void)
	{
		auto entity = std::make_shared<Carousel>(
			this->getComponentRegistry(), _config, _variant, _itemCount,
			_itemWidth, _itemHeight, _value, _items, _onChanged);
		return entity;
	}

	void Carousel::Builder::reset(void)
	{
		_config		= SurfaceConfig();
		_variant	= Variant::Basic;
		_itemCount	= 3;
		_itemWidth	= 120.0f;
		_itemHeight = 160.0f;
		_value		= 0;
		_items.clear();

		_config.borderRadius = 0.0f;
		_config.axis		 = components::Layout::Axis::Horizontal;
		_config.mainAxisAlignment =
			components::Layout::MainAxisAlignment::Start;
		_config.crossAxisAlignment =
			components::Layout::CrossAxisAlignment::Center;
		_config.padding = ItemSpacing;
		_config.spacing = ItemSpacing;
		_config.setFixedWidth(400.0f);
		_config.setFixedHeight(_itemHeight + 2.0f * ItemSpacing);
	}

	Carousel::Builder &
		Carousel::Builder::withPose(const utility::graphic::PoseF &pose)
	{
		_config.pose = pose;
		return *this;
	}

	Carousel::Builder &Carousel::Builder::withVariant(Variant variant)
	{
		_variant = variant;
		return *this;
	}

	Carousel::Builder &Carousel::Builder::withItemCount(std::size_t itemCount)
	{
		_itemCount = itemCount;
		return *this;
	}

	Carousel::Builder &Carousel::Builder::withItemSize(float width,
													   float height)
	{
		_itemWidth	= width;
		_itemHeight = height;
		_config.setFixedHeight(height + 2.0f * ItemSpacing);
		return *this;
	}

	Carousel::Builder &Carousel::Builder::withValue(std::size_t index)
	{
		_value = index;
		return *this;
	}

	Carousel::Builder &Carousel::Builder::withOnChanged(
		const std::function<void(std::size_t)> &onChanged)
	{
		_onChanged = onChanged;
		return *this;
	}

	Carousel::Builder &Carousel::Builder::withItems(
		const std::vector<std::shared_ptr<ecs::Entity>> &items)
	{
		_items = items;
		return *this;
	}

	std::shared_ptr<Carousel>
		Carousel::Director::makeCarousel(Builder &builder,
										 std::shared_ptr<ecs::Entity> parent,
										 Variant variant, std::size_t itemCount)
	{
		return builder.withVariant(variant)
			.withItemCount(itemCount)
			.registerEntity(parent);
	}

	Carousel::Carousel(ecs::ComponentRegistry &registry,
					   const SurfaceConfig &config, Variant variant,
					   std::size_t itemCount, float itemWidth, float itemHeight,
					   std::size_t value,
					   const std::vector<std::shared_ptr<ecs::Entity>> &items,
					   const std::function<void(std::size_t)> &onChanged)
		: SurfaceBase<components::Value, components::Scrollable,
					  components::DragInteraction>(registry, config)
		, _variant(variant)
		, _itemCount(items.empty() ? itemCount : items.size())
		, _itemWidth(itemWidth)
		, _itemHeight(itemHeight)
		, _items(items)
		, _onChanged(onChanged)
		, _built(false)
	{
		auto &valueComponent =
			getComponentRegistry().getComponent<components::Value>(
				getIdentifier());
		valueComponent.setMin(0.0f)
			.setMax(static_cast<float>(_itemCount > 0 ? _itemCount - 1 : 0))
			.setStep(1.0f);
		valueComponent.setOnChangedHandler([this](float) {
			applyState();
		});
		valueComponent.setValue(static_cast<float>(value));

		auto &scrollable =
			getComponentRegistry().getComponent<components::Scrollable>(
				getIdentifier());
		scrollable.setContentSize(utility::math::Vector2F(
			{ static_cast<float>(_itemCount) * (itemWidth + ItemSpacing),
			  itemHeight }));

		applyVariant();
	}

	Carousel::~Carousel(void)
	{
	}

	void Carousel::applyVariant(void)
	{
		auto config = getSurfaceConfig();

		config.axis			= components::Layout::Axis::Horizontal;
		config.spacing		= ItemSpacing;
		config.borderRadius = 0.0f;

		switch (_variant) {
			case Variant::Centered:
				config.mainAxisAlignment =
					components::Layout::MainAxisAlignment::Center;
				config.crossAxisAlignment =
					components::Layout::CrossAxisAlignment::Center;
				config.padding = ItemSpacing;
				break;
			case Variant::Uncontained:
				config.mainAxisAlignment =
					components::Layout::MainAxisAlignment::Start;
				config.crossAxisAlignment =
					components::Layout::CrossAxisAlignment::Center;
				config.padding = 0.0f;
				break;
			case Variant::Basic:
			default:
				config.mainAxisAlignment =
					components::Layout::MainAxisAlignment::Start;
				config.crossAxisAlignment =
					components::Layout::CrossAxisAlignment::Center;
				config.padding = ItemSpacing;
				break;
		}

		setSurfaceConfig(config);
		setColor(transparentColor());
	}

	void Carousel::buildItems(void)
	{
		if (_built) {
			return;
		}

		// Remove any items left over from a previous build.
		this->accessDirectEntities().clear();

		if (_items.empty()) {
			_items.reserve(_itemCount);
			for (std::size_t index = 0; index < _itemCount; ++index) {
				auto item = std::make_shared<CarouselItem>(
					getComponentRegistry(), _itemWidth, _itemHeight);
				getComponentRegistry()
					.getComponent<components::Color>(item->getIdentifier())
					.setColor(
						schemeColor(SchemeColorRole::SurfaceContainerHighest));
				_items.push_back(item);
			}
		}

		for (const auto &item: _items) {
			if (item == nullptr) {
				continue;
			}
			item->setParent(shared_from_this());

			// Register the item so the hierarchy and layout system can see it.
			this->addEntity(item);
		}

		// The items are arranged by `systems::Layout` along the horizontal
		// axis; the scroll offset is tracked separately.
		setChildren(_items);

		_built = true;
	}

	void Carousel::applyState(void)
	{
		if (!_built) {
			return;
		}

		auto &scrollable =
			getComponentRegistry().getComponent<components::Scrollable>(
				getIdentifier());

		scrollable.setContentSize(utility::math::Vector2F(
			{ static_cast<float>(_itemCount) * (_itemWidth + ItemSpacing),
			  _itemHeight }));

		const std::size_t index = static_cast<std::size_t>(
			std::round(getComponentRegistry()
						   .getComponent<components::Value>(getIdentifier())
						   .getValue()));
		const float offset =
			static_cast<float>(index) * (_itemWidth + ItemSpacing);

		scrollable.setContentOffset(utility::math::Vector2F({ offset, 0.0f }));
	}

	void Carousel::setIndex(std::size_t index)
	{
		const std::size_t clamped =
			_itemCount > 0 ? std::min(index, _itemCount - 1) : 0;

		auto &value = getComponentRegistry().getComponent<components::Value>(
			getIdentifier());
		if (static_cast<std::size_t>(value.getValue()) == clamped) {
			return;
		}

		value.setValue(static_cast<float>(clamped));

		if (_onChanged) {
			_onChanged(clamped);
		}
	}

	Carousel &Carousel::setVariant(Variant variant)
	{
		_variant = variant;
		applyVariant();
		applyState();
		return *this;
	}

	Carousel::Variant Carousel::getVariant(void) const
	{
		return _variant;
	}

	Carousel &Carousel::setItemCount(std::size_t itemCount)
	{
		_itemCount = itemCount;
		_built	   = false;
		_items.clear();

		auto &value = getComponentRegistry().getComponent<components::Value>(
			getIdentifier());
		value.setMax(static_cast<float>(_itemCount > 0 ? _itemCount - 1 : 0));
		value.setMin(0.0f);

		getComponentRegistry()
			.getComponent<components::Scrollable>(getIdentifier())
			.setContentSize(utility::math::Vector2F(
				{ static_cast<float>(_itemCount) * (_itemWidth + ItemSpacing),
				  _itemHeight }));

		buildItems();
		applyState();

		return *this;
	}

	std::size_t Carousel::getItemCount(void) const
	{
		return _itemCount;
	}

	ecs::Entity::Identifier Carousel::getItemIdentifier(std::size_t index) const
	{
		if (index >= _items.size() || _items[index] == nullptr) {
			return ecs::Entity::InvalidIdentifier;
		}
		return _items[index]->getIdentifier();
	}

	Carousel &Carousel::setValue(std::size_t index)
	{
		setIndex(index);
		return *this;
	}

	std::size_t Carousel::getValue(void) const
	{
		return static_cast<std::size_t>(
			std::round(getComponentRegistry()
						   .getComponent<components::Value>(getIdentifier())
						   .getValue()));
	}

	void Carousel::initialize(void)
	{
		SurfaceBase::initialize();
		applyVariant();
		buildItems();

		getComponentRegistry()
			.getComponent<components::DragInteraction>(getIdentifier())
			.setOnDragHandler([this](const utility::math::Vector2F &delta) {
				const float unit = _itemWidth + ItemSpacing;
				if (unit <= 0.0f) {
					return;
				}
				const float moved  = delta.x / unit;
				const long current = static_cast<long>(std::round(
					getComponentRegistry()
						.getComponent<components::Value>(getIdentifier())
						.getValue()));
				setIndex(static_cast<std::size_t>(std::max(
					0L, current + static_cast<long>(std::round(moved)))));
			});

		applyState();
	}

	void Carousel::update(void)
	{
		SurfaceBase::update();
		applyVariant();
		buildItems();
		applyState();
	}

}	 // namespace guillaume::entities
