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

#include "guillaume/entities/loading_indicator.hpp"

#include <stdexcept>

#include <memory>

#include "guillaume/ecs/entity_filler.hpp"

#include "guillaume/components/animation.hpp"
#include "guillaume/components/arc.hpp"
#include "guillaume/components/borders.hpp"
#include "guillaume/components/bound.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/transform.hpp"

#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/style_helpers.hpp"
#include "guillaume/systems/loading_indicator.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::entities
{
	namespace
	{
		constexpr float LinearHeight =
			components::LoadingIndicator::LinearHeight;
		constexpr float LinearWidth = components::LoadingIndicator::LinearWidth;
		const float LinearRadius	= getShapeRadius(theme::ShapeRole::Full);
		constexpr float CircularSize =
			components::LoadingIndicator::DefaultSize;

		/**
		 * @brief Linear sliding bar child of a loading indicator.
		 */
		class LinearBar:
			public ecs::EntityFiller<components::Transform, components::Bound,
									 components::Color, components::Borders,
									 components::Animation>
		{
			public:
			/**
			 * @brief Construct a linear bar.
			 * @param registry The component registry.
			 */
			explicit LinearBar(ecs::ComponentRegistry &registry)
				: ecs::EntityFiller<components::Transform, components::Bound,
									components::Color, components::Borders,
									components::Animation>(registry)
			{
			}

			/**
			 * @brief Default destructor.
			 */
			~LinearBar(void) override = default;
		};

		/**
		 * @brief Rotating arc child of a loading indicator.
		 */
		class RotatingArc:
			public ecs::EntityFiller<components::Transform, components::Bound,
									 components::Color, components::Arc,
									 components::Animation>
		{
			public:
			/**
			 * @brief Construct a rotating arc.
			 * @param registry The component registry.
			 */
			explicit RotatingArc(ecs::ComponentRegistry &registry)
				: ecs::EntityFiller<components::Transform, components::Bound,
									components::Color, components::Arc,
									components::Animation>(registry)
			{
			}

			/**
			 * @brief Default destructor.
			 */
			~RotatingArc(void) override = default;
		};
	}	 // namespace

	LoadingIndicator::Builder::Builder(
		ecs::ComponentRegistry &componentRegistry,
		ecs::EntityRegistry &entityRegistry)
		: EntityBuilderBase<LoadingIndicator>(componentRegistry, entityRegistry)
	{
		reset();
	}

	std::shared_ptr<LoadingIndicator>
		LoadingIndicator::Builder::buildEntity(void)
	{
		if (_variant == Variant::Circular) {
			_config.setFixedWidth(_size).setFixedHeight(_size);
			_config.borderRadius = _size * 0.5f;
		} else {
			_config.setFixedWidth(LinearWidth).setFixedHeight(LinearHeight);
			_config.borderRadius = LinearRadius;
		}

		auto entity = std::make_shared<LoadingIndicator>(
			this->getComponentRegistry(), _config, _variant);
		return entity;
	}

	void LoadingIndicator::Builder::reset(void)
	{
		_config	 = SurfaceConfig();
		_variant = Variant::Circular;
		_size	 = CircularSize;

		_config.borderRadius = LinearRadius;
		_config.axis		 = components::Layout::Axis::Horizontal;
		_config.mainAxisAlignment =
			components::Layout::MainAxisAlignment::Start;
		_config.crossAxisAlignment =
			components::Layout::CrossAxisAlignment::Center;
		_config.padding = 0.0f;
		_config.spacing = 0.0f;
		_config.setFixedWidth(LinearWidth);
		_config.setFixedHeight(LinearHeight);
	}

	LoadingIndicator::Builder &
		LoadingIndicator::Builder::withPose(const utility::graphic::PoseF &pose)
	{
		_config.pose = pose;
		return *this;
	}

	LoadingIndicator::Builder &
		LoadingIndicator::Builder::withVariant(Variant variant)
	{
		_variant = variant;
		return *this;
	}

	LoadingIndicator::Builder &LoadingIndicator::Builder::withSize(float size)
	{
		_size = size;
		return *this;
	}

	std::shared_ptr<LoadingIndicator>
		LoadingIndicator::Builder::makeLoadingIndicator(
			std::shared_ptr<ecs::Entity> parent, Variant variant)
	{
		return withVariant(variant).registerEntity(parent);
	}

	LoadingIndicator::LoadingIndicator(ecs::ComponentRegistry &registry,
									   const SurfaceConfig &config,
									   Variant variant)
		: SurfaceBase<components::Animation, components::Accessibility,
					  components::LoadingIndicator>(registry, config)
	{
		getComponentRegistry()
			.getComponent<components::LoadingIndicator>(getIdentifier())
			.setVariant(variant)
			.setSize(variant == Variant::Circular && config.hasFixedHeight
						 ? config.fixedHeight
						 : CircularSize);
		systems::LoadingIndicator::install(getComponentRegistry(),
										   getIdentifier());
		systems::LoadingIndicator::apply(getComponentRegistry(),
										 getIdentifier());
	}

	LoadingIndicator &LoadingIndicator::setHidesWhenStopped(bool hides)
	{
		getComponentRegistry()
			.getComponent<components::LoadingIndicator>(getIdentifier())
			.setHidesWhenStopped(hides);
		systems::LoadingIndicator::startFade(getComponentRegistry(),
											 getIdentifier());
		return *this;
	}

	bool LoadingIndicator::hidesWhenStopped(void) const
	{
		return getComponentRegistry()
			.getComponent<components::LoadingIndicator>(getIdentifier())
			.hidesWhenStopped();
	}

	float LoadingIndicator::getOpacity(void) const
	{
		return getComponentRegistry()
			.getComponent<components::LoadingIndicator>(getIdentifier())
			.getOpacity();
	}

	LoadingIndicator &LoadingIndicator::setIndicatorColor(
		const utility::graphic::Color32Bit &color)
	{
		getComponentRegistry()
			.getComponent<components::LoadingIndicator>(getIdentifier())
			.setIndicatorColor(color);
		systems::LoadingIndicator::apply(getComponentRegistry(),
										 getIdentifier());
		return *this;
	}

	utility::graphic::Color32Bit LoadingIndicator::getIndicatorColor(void) const
	{
		return systems::LoadingIndicator::indicatorColor(getComponentRegistry(),
														 getIdentifier());
	}

	LoadingIndicator &LoadingIndicator::setSize(float size)
	{
		if (size <= 0.0f) {
			throw std::invalid_argument(
				"Loading indicator size must be positive");
		}

		auto &state =
			getComponentRegistry().getComponent<components::LoadingIndicator>(
				getIdentifier());

		state.setSize(size);
		if (state.getVariant() == Variant::Circular) {
			state.setBuilt(false);
			buildChild();
			systems::LoadingIndicator::apply(getComponentRegistry(),
											 getIdentifier());
		}
		return *this;
	}

	float LoadingIndicator::getSize(void) const
	{
		return getComponentRegistry()
			.getComponent<components::LoadingIndicator>(getIdentifier())
			.getSize();
	}

	float LoadingIndicator::getStrokeWidth(void) const
	{
		return systems::LoadingIndicator::strokeWidth(getComponentRegistry(),
													  getIdentifier());
	}

	LoadingIndicator::~LoadingIndicator(void)
	{
	}

	void LoadingIndicator::buildChild(void)
	{
		auto &registry = getComponentRegistry();
		auto &state	   = registry.getComponent<components::LoadingIndicator>(
			   getIdentifier());

		if (state.isBuilt()) {
			return;
		}

		// Remove any child left over from a previous variant with its
		// components, and silence its animation first so it no longer
		// drives this indicator.
		if (registry.hasComponent<components::Animation>(
				state.getIndicator())) {
			registry.getComponent<components::Animation>(state.getIndicator())
				.setOnSampleHandler(components::Animation::SampleHandler {})
				.stop();
		}
		this->clearEntities();

		std::shared_ptr<ecs::Entity> child;
		if (state.getVariant() == Variant::Circular) {
			child = std::make_shared<RotatingArc>(registry);
		} else {
			child = std::make_shared<LinearBar>(registry);
		}
		child->setParent(shared_from_this());
		this->addEntity(child);
		state.setIndicator(child->getIdentifier()).setBuilt(true);
		systems::LoadingIndicator::installChildren(registry, getIdentifier());

		// The animated child is positioned by systems::LoadingIndicator, so
		// it is not registered as a layout child of the surface.
		setChildren({});
	}

	LoadingIndicator &LoadingIndicator::setVariant(Variant variant)
	{
		auto &state =
			getComponentRegistry().getComponent<components::LoadingIndicator>(
				getIdentifier());

		if (state.getVariant() == variant) {
			return *this;
		}

		// The child is built from the variant: rebuild it so the new shape
		// (bar vs. arc) is adopted.
		state.setVariant(variant).setBuilt(false);
		buildChild();
		systems::LoadingIndicator::apply(getComponentRegistry(),
										 getIdentifier());

		return *this;
	}

	LoadingIndicator::Variant LoadingIndicator::getVariant(void) const
	{
		return getComponentRegistry()
			.getComponent<components::LoadingIndicator>(getIdentifier())
			.getVariant();
	}

	LoadingIndicator &LoadingIndicator::start(void)
	{
		systems::LoadingIndicator::start(getComponentRegistry(),
										 getIdentifier());
		return *this;
	}

	LoadingIndicator &LoadingIndicator::stop(void)
	{
		systems::LoadingIndicator::stop(getComponentRegistry(),
										getIdentifier());
		return *this;
	}

	bool LoadingIndicator::isRunning(void) const
	{
		return systems::LoadingIndicator::isRunning(getComponentRegistry(),
													getIdentifier());
	}

	void LoadingIndicator::initialize(void)
	{
		buildChild();
		systems::LoadingIndicator::apply(getComponentRegistry(),
										 getIdentifier());
	}

}	 // namespace guillaume::entities
