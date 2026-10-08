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

#include "guillaume/entities/side_sheet.hpp"
#include "guillaume/entities/overlay_helpers.hpp"

#include <vector>

#include "guillaume/entities/content_helpers.hpp"
#include "guillaume/entities/overlay_helpers.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/style_helpers.hpp"
#include "guillaume/entities/overlay_helpers.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::entities
{
	SideSheet::Builder::Builder(ecs::ComponentRegistry &componentRegistry,
								ecs::EntityRegistry &entityRegistry)
		: EntityBuilderBase<SideSheet>(componentRegistry, entityRegistry)
	{
		reset();
	}

	std::shared_ptr<SideSheet> SideSheet::Builder::buildEntity(void)
	{
		auto entity =
			std::make_shared<SideSheet>(this->getComponentRegistry(), _config,
										_variant, _side, _title, _content);
		return entity;
	}

	void SideSheet::Builder::reset(void)
	{
		_config	 = SurfaceConfig();
		_variant = Variant::Standard;
		_side	 = Side::Right;
		_title.clear();
		_content.clear();

		_config.axis = components::Layout::Axis::Vertical;
		_config.mainAxisAlignment =
			components::Layout::MainAxisAlignment::Start;
		_config.crossAxisAlignment =
			components::Layout::CrossAxisAlignment::Start;
		_config.padding = 16.0f;
		_config.spacing = 12.0f;
		_config.setFixedWidth(320.0f);
		_config.setFixedHeight(640.0f);
	}

	SideSheet::Builder &
		SideSheet::Builder::withPose(const utility::graphic::PoseF &pose)
	{
		_config.pose = pose;
		return *this;
	}

	SideSheet::Builder &SideSheet::Builder::withVariant(Variant variant)
	{
		_variant = variant;
		return *this;
	}

	SideSheet::Builder &SideSheet::Builder::withSide(Side side)
	{
		_side = side;
		return *this;
	}

	SideSheet::Builder &SideSheet::Builder::withTitle(const std::string &title)
	{
		_title = title;
		return *this;
	}

	SideSheet::Builder &
		SideSheet::Builder::withContent(const std::string &content)
	{
		_content = content;
		return *this;
	}

	std::shared_ptr<SideSheet> SideSheet::Director::makeSideSheet(
		Builder &builder, std::shared_ptr<ecs::Entity> parent, Variant variant,
		Side side, const std::string &title, const std::string &content)
	{
		return builder.withVariant(variant)
			.withSide(side)
			.withTitle(title)
			.withContent(content)
			.registerEntity(parent);
	}

	SideSheet::SideSheet(ecs::ComponentRegistry &registry,
						 const SurfaceConfig &config, Variant variant,
						 Side side, const std::string &title,
						 const std::string &content)
		: SurfaceBase<components::Overlay>(registry, config)
		, _variant(variant)
		, _side(side)
		, _title(title)
		, _content(content)
		, _titleEntity()
		, _contentEntity()
	{
		applyVariant();
		applyOverlay();
	}

	SideSheet::~SideSheet(void)
	{
	}

	void SideSheet::applyVariant(void)
	{
		auto config = getSurfaceConfig();

		config.axis				 = components::Layout::Axis::Vertical;
		config.mainAxisAlignment = components::Layout::MainAxisAlignment::Start;
		config.crossAxisAlignment =
			components::Layout::CrossAxisAlignment::Start;
		config.padding		  = 16.0f;
		config.spacing		  = 12.0f;
		config.hasFixedWidth  = true;
		config.fixedWidth	  = 320.0f;
		config.hasFixedHeight = true;
		config.fixedHeight	  = 640.0f;

		setSurfaceConfig(config);

		auto &borders =
			this->getComponentRegistry().getComponent<components::Borders>(
				this->getIdentifier());

		if (_side == Side::Left) {
			borders.setTopLeftRadius(0.0f)
				.setBottomLeftRadius(0.0f)
				.setTopRightRadius(16.0f)
				.setBottomRightRadius(16.0f);
		} else {
			borders.setTopRightRadius(0.0f)
				.setBottomRightRadius(0.0f)
				.setTopLeftRadius(16.0f)
				.setBottomLeftRadius(16.0f);
		}

		if (_variant == Variant::Modal) {
			setColor(schemeColor(SchemeColorRole::SurfaceContainerLow));
		} else {
			setColor(schemeColor(SchemeColorRole::Surface));
		}
	}

	void SideSheet::applyOverlay(void)
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

	void SideSheet::buildContent(void)
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
	}

	SideSheet &SideSheet::setVariant(Variant variant)
	{
		_variant = variant;
		applyVariant();
		applyOverlay();
		return *this;
	}

	SideSheet::Variant SideSheet::getVariant(void) const
	{
		return _variant;
	}

	SideSheet &SideSheet::setSide(Side side)
	{
		_side = side;
		applyVariant();
		return *this;
	}

	SideSheet::Side SideSheet::getSide(void) const
	{
		return _side;
	}

	SideSheet &SideSheet::setTitle(const std::string &title)
	{
		_title = title;

		if (_titleEntity != nullptr) {
			_titleEntity->setContent(title);
		}

		return *this;
	}

	SideSheet &SideSheet::setContent(const std::string &content)
	{
		_content = content;

		if (_contentEntity != nullptr) {
			_contentEntity->setContent(content);
		}

		return *this;
	}

	SideSheet &SideSheet::show(void)
	{
		setOverlayVisible(this->getComponentRegistry(), this->getIdentifier(),
						  true);
		return *this;
	}

	SideSheet &SideSheet::hide(void)
	{
		setOverlayVisible(this->getComponentRegistry(), this->getIdentifier(),
						  false);
		return *this;
	}

	bool SideSheet::isVisible(void) const
	{
		return isOverlayVisible(this->getComponentRegistry(),
								this->getIdentifier());
	}

	SideSheet &SideSheet::open(void)
	{
		return show();
	}

	SideSheet &SideSheet::close(void)
	{
		return hide();
	}

	bool SideSheet::isOpen(void) const
	{
		return isVisible();
	}

	void SideSheet::initialize(void)
	{
		SurfaceBase::initialize();
		applyVariant();
		applyOverlay();
		buildContent();
	}

	void SideSheet::update(void)
	{
		SurfaceBase::update();
		applyVariant();
		applyOverlay();
		buildContent();
	}

}	 // namespace guillaume::entities
