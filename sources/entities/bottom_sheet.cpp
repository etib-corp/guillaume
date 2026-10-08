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

#include "guillaume/entities/bottom_sheet.hpp"
#include "guillaume/entities/overlay_helpers.hpp"

#include <memory>
#include <vector>

#include "guillaume/ecs/entity_filler.hpp"

#include "guillaume/components/bound.hpp"
#include "guillaume/components/scrim.hpp"
#include "guillaume/components/transform.hpp"

#include "guillaume/entities/content_helpers.hpp"
#include "guillaume/entities/overlay_helpers.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/style_helpers.hpp"
#include "guillaume/entities/overlay_helpers.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::entities
{
	namespace
	{
		constexpr float DismissDragThreshold = 120.0f;

		/**
		 * @brief Internal full-screen scrim entity used by modal sheets.
		 */
		class SheetScrim:
			public ecs::EntityFiller<components::Transform, components::Bound,
									 components::Scrim>
		{
			public:
			/**
			 * @brief Construct a full-size dimming scrim.
			 * @param registry The component registry.
			 */
			explicit SheetScrim(ecs::ComponentRegistry &registry)
				: ecs::EntityFiller<components::Transform, components::Bound,
									components::Scrim>(registry)
			{
				getComponentRegistry()
					.getComponent<components::Bound>(getIdentifier())
					.setWidth(800.0f)
					.setHeight(600.0f);

				getComponentRegistry()
					.getComponent<components::Scrim>(getIdentifier())
					.setColor(utility::graphic::Color32Bit(0, 0, 0, 82));
			}

			/**
			 * @brief Default destructor.
			 */
			~SheetScrim(void) override = default;
		};
	}	 // namespace

	BottomSheet::Builder::Builder(ecs::ComponentRegistry &componentRegistry,
								  ecs::EntityRegistry &entityRegistry)
		: EntityBuilderBase<BottomSheet>(componentRegistry, entityRegistry)
	{
		reset();
	}

	std::shared_ptr<BottomSheet> BottomSheet::Builder::buildEntity(void)
	{
		auto entity =
			std::make_shared<BottomSheet>(this->getComponentRegistry(), _config,
										  _variant, _title, _content, _height);
		return entity;
	}

	void BottomSheet::Builder::reset(void)
	{
		_config	 = SurfaceConfig();
		_variant = Variant::Standard;
		_title.clear();
		_content.clear();
		_height = 320.0f;

		_config.axis = components::Layout::Axis::Vertical;
		_config.mainAxisAlignment =
			components::Layout::MainAxisAlignment::Start;
		_config.crossAxisAlignment =
			components::Layout::CrossAxisAlignment::Start;
		_config.padding = 16.0f;
		_config.spacing = 12.0f;
		_config.setFixedWidth(360.0f);
		_config.setFixedHeight(320.0f);
	}

	BottomSheet::Builder &
		BottomSheet::Builder::withPose(const utility::graphic::PoseF &pose)
	{
		_config.pose = pose;
		return *this;
	}

	BottomSheet::Builder &BottomSheet::Builder::withVariant(Variant variant)
	{
		_variant = variant;
		return *this;
	}

	BottomSheet::Builder &
		BottomSheet::Builder::withTitle(const std::string &title)
	{
		_title = title;
		return *this;
	}

	BottomSheet::Builder &
		BottomSheet::Builder::withContent(const std::string &content)
	{
		_content = content;
		return *this;
	}

	BottomSheet::Builder &BottomSheet::Builder::withHeight(float height)
	{
		_height				   = height;
		_config.hasFixedHeight = true;
		_config.fixedHeight	   = height;
		return *this;
	}

	std::shared_ptr<BottomSheet> BottomSheet::Director::makeBottomSheet(
		Builder &builder, std::shared_ptr<ecs::Entity> parent, Variant variant,
		const std::string &title, const std::string &content, float height)
	{
		return builder.withVariant(variant)
			.withTitle(title)
			.withContent(content)
			.withHeight(height)
			.registerEntity(parent);
	}

	BottomSheet::BottomSheet(ecs::ComponentRegistry &registry,
							 const SurfaceConfig &config, Variant variant,
							 const std::string &title,
							 const std::string &content, float height)
		: SurfaceBase<components::Overlay, components::DragInteraction>(
			  registry, config)
		, _variant(variant)
		, _title(title)
		, _content(content)
		, _height(height)
		, _titleEntity()
		, _contentEntity()
		, _scrimEntity()
	{
		applyVariant();
		applyOverlay();
		applyDrag();
	}

	BottomSheet::~BottomSheet(void)
	{
	}

	void BottomSheet::applyVariant(void)
	{
		auto config = getSurfaceConfig();

		config.axis				 = components::Layout::Axis::Vertical;
		config.mainAxisAlignment = components::Layout::MainAxisAlignment::Start;
		config.crossAxisAlignment =
			components::Layout::CrossAxisAlignment::Start;
		config.padding = 16.0f;
		config.spacing = 12.0f;

		config.hasFixedWidth  = true;
		config.fixedWidth	  = 360.0f;
		config.hasFixedHeight = true;
		config.fixedHeight	  = _height;

		setSurfaceConfig(config);

		auto &borders =
			this->getComponentRegistry().getComponent<components::Borders>(
				this->getIdentifier());
		borders.setTopLeftRadius(28.0f)
			.setTopRightRadius(28.0f)
			.setBottomLeftRadius(0.0f)
			.setBottomRightRadius(0.0f);

		if (_variant == Variant::Modal) {
			setColor(schemeColor(SchemeColorRole::SurfaceContainerHigh));
		} else {
			setColor(schemeColor(SchemeColorRole::SurfaceContainerLow));
		}
	}

	void BottomSheet::applyOverlay(void)
	{
		auto &overlay =
			this->getComponentRegistry().getComponent<components::Overlay>(
				this->getIdentifier());

		if (_variant == Variant::Modal) {
			overlay.setModal(true);
			overlay.setDismissOnOutsideClick(true);
		} else {
			overlay.setModal(false);
			overlay.setDismissOnOutsideClick(false);
		}

		overlay.setDismissOnEscape(true);
	}

	void BottomSheet::applyDrag(void)
	{
		auto &drag = this->getComponentRegistry()
						 .getComponent<components::DragInteraction>(
							 this->getIdentifier());

		drag.setOnDragHandler([this](const utility::math::Vector2F &delta) {
			if (delta.y > DismissDragThreshold) {
				this->hide();
			}
		});
	}

	void BottomSheet::buildContent(void)
	{
		if (_titleEntity == nullptr && !_title.empty()) {
			_titleEntity = buildText(this->getComponentRegistry(), *this,
									 this->shared_from_this(), _title, 22.0f,
									 schemeColor(SchemeColorRole::OnSurface));
		}

		if (_contentEntity == nullptr && !_content.empty()) {
			_contentEntity =
				buildText(this->getComponentRegistry(), *this,
						  this->shared_from_this(), _content, 14.0f,
						  schemeColor(SchemeColorRole::OnSurfaceVariant));
		}

		std::vector<std::shared_ptr<ecs::Entity>> children;

		if (_titleEntity != nullptr) {
			children.push_back(_titleEntity);
		}

		if (_contentEntity != nullptr) {
			children.push_back(_contentEntity);
		}

		setChildren(children);

		if (_variant == Variant::Modal && _scrimEntity == nullptr) {
			auto scrim =
				std::make_shared<SheetScrim>(this->getComponentRegistry());
			this->addEntity(scrim);
			_scrimEntity = scrim;
		}
	}

	BottomSheet &BottomSheet::setVariant(Variant variant)
	{
		_variant = variant;
		applyVariant();
		applyOverlay();
		return *this;
	}

	BottomSheet::Variant BottomSheet::getVariant(void) const
	{
		return _variant;
	}

	BottomSheet &BottomSheet::setTitle(const std::string &title)
	{
		_title = title;

		if (_titleEntity != nullptr) {
			_titleEntity->setContent(title);
		}

		return *this;
	}

	BottomSheet &BottomSheet::setContent(const std::string &content)
	{
		_content = content;

		if (_contentEntity != nullptr) {
			_contentEntity->setContent(content);
		}

		return *this;
	}

	BottomSheet &BottomSheet::show(void)
	{
		setOverlayVisible(this->getComponentRegistry(), this->getIdentifier(),
						  true);
		return *this;
	}

	BottomSheet &BottomSheet::hide(void)
	{
		setOverlayVisible(this->getComponentRegistry(), this->getIdentifier(),
						  false);
		return *this;
	}

	bool BottomSheet::isVisible(void) const
	{
		return isOverlayVisible(this->getComponentRegistry(),
								this->getIdentifier());
	}

	BottomSheet &BottomSheet::open(void)
	{
		return show();
	}

	BottomSheet &BottomSheet::close(void)
	{
		return hide();
	}

	bool BottomSheet::isOpen(void) const
	{
		return isVisible();
	}

	void BottomSheet::initialize(void)
	{
		SurfaceBase::initialize();
		applyVariant();
		applyOverlay();
		applyDrag();
		buildContent();
	}

	void BottomSheet::update(void)
	{
		SurfaceBase::update();
		applyVariant();
		applyOverlay();
		applyDrag();
		buildContent();
	}

}	 // namespace guillaume::entities
