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
#include <utility>

#include "guillaume/entities/menu.hpp"

#include "guillaume/components/borders.hpp"
#include "guillaume/components/bound.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/layout.hpp"
#include "guillaume/components/transform.hpp"
#include "guillaume/ecs/parent_entity_filler.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/content_helpers.hpp"
#include "guillaume/entities/style_helpers.hpp"
#include "guillaume/systems/menu.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::entities
{
	namespace
	{
		/// M3 menu item icon size, in dp.
		constexpr float MenuIconSize = 24.0f;

		/**
		 * @brief Container of one menu item.
		 *
		 * A bare layout surface without state or behaviour: systems::Menu
		 * sizes, colors and arranges it through its components.
		 */
		class MenuRow final:
			public ecs::ParentEntityFiller<
				components::Layout, components::Transform, components::Bound,
				components::Color, components::Borders>
		{
			public:
			/**
			 * @brief Construct a row.
			 * @param registry The component registry.
			 */
			explicit MenuRow(ecs::ComponentRegistry &registry)
				: ecs::ParentEntityFiller<components::Layout,
										  components::Transform,
										  components::Bound, components::Color,
										  components::Borders>(registry)
			{
			}

			/**
			 * @brief Default destructor.
			 */
			~MenuRow(void) override = default;
		};
	}	 // namespace

	Menu::Builder::Builder(ecs::ComponentRegistry &componentRegistry,
						   ecs::EntityRegistry &entityRegistry)
		: EntityBuilderBase<Menu>(componentRegistry, entityRegistry)
	{
		reset();
	}

	std::shared_ptr<Menu> Menu::Builder::buildEntity(void)
	{
		auto entity = std::make_shared<Menu>(this->getComponentRegistry(),
											 _config, _variant, _items);
		return entity;
	}

	void Menu::Builder::reset(void)
	{
		_config	 = SurfaceConfig();
		_variant = Variant::Dropdown;
		_items.clear();

		_config.axis = components::Layout::Axis::Vertical;
		_config.mainAxisAlignment =
			components::Layout::MainAxisAlignment::Start;
		_config.crossAxisAlignment =
			components::Layout::CrossAxisAlignment::Start;
		_config.padding		 = 0.0f;
		_config.spacing		 = 0.0f;
		_config.borderRadius = getShapeRadius(theme::ShapeRole::ExtraSmall);
		_config.setFixedWidth(MinWidth);
	}

	Menu::Builder &Menu::Builder::withPose(const utility::graphic::PoseF &pose)
	{
		_config.pose = pose;
		return *this;
	}

	Menu::Builder &Menu::Builder::withVariant(Variant variant)
	{
		_variant = variant;
		return *this;
	}

	Menu::Builder &Menu::Builder::addItem(const std::string &label,
										  const std::string &leadingIcon)
	{
		Item item;
		item.label		 = label;
		item.leadingIcon = leadingIcon;
		_items.push_back(item);
		return *this;
	}

	Menu::Builder &Menu::Builder::addItem(const Item &item)
	{
		_items.push_back(item);
		return *this;
	}

	std::shared_ptr<Menu>
		Menu::Builder::makeMenu(std::shared_ptr<ecs::Entity> parent,
								Variant variant, const std::vector<Item> &items)
	{
		withVariant(variant);

		for (const auto &item: items) {
			addItem(item);
		}

		return registerEntity(parent);
	}

	Menu::Menu(ecs::ComponentRegistry &registry, const SurfaceConfig &config,
			   Variant variant, const std::vector<Item> &items)
		: SurfaceBase<components::Overlay, components::Elevation,
					  components::Accessibility, components::Menu>(registry,
																   config)
	{
		getComponentRegistry()
			.getComponent<components::Menu>(getIdentifier())
			.setVariant(variant)
			.setItems(items);
		systems::Menu::install(getComponentRegistry(), getIdentifier());
		systems::Menu::apply(getComponentRegistry(), getIdentifier());
	}

	Menu &Menu::setVariant(Variant variant)
	{
		getComponentRegistry()
			.getComponent<components::Menu>(getIdentifier())
			.setVariant(variant);
		systems::Menu::apply(getComponentRegistry(), getIdentifier());
		return *this;
	}

	Menu::Variant Menu::getVariant(void) const
	{
		return getComponentRegistry()
			.getComponent<components::Menu>(getIdentifier())
			.getVariant();
	}

	Menu &Menu::setMode(Mode mode)
	{
		getComponentRegistry()
			.getComponent<components::Menu>(getIdentifier())
			.setMode(mode);
		systems::Menu::apply(getComponentRegistry(), getIdentifier());
		return *this;
	}

	Menu::Mode Menu::getMode(void) const
	{
		return getComponentRegistry()
			.getComponent<components::Menu>(getIdentifier())
			.getMode();
	}

	Menu &Menu::setItems(const std::vector<Item> &items)
	{
		getComponentRegistry()
			.getComponent<components::Menu>(getIdentifier())
			.setItems(items)
			.setRowsDirty(true);
		buildItems();
		systems::Menu::apply(getComponentRegistry(), getIdentifier());
		return *this;
	}

	std::size_t Menu::getItemCount(void) const
	{
		return getComponentRegistry()
			.getComponent<components::Menu>(getIdentifier())
			.getItems()
			.size();
	}

	ecs::Entity::Identifier Menu::getItemIdentifier(std::size_t index) const
	{
		const auto &rows = getComponentRegistry()
							   .getComponent<components::Menu>(getIdentifier())
							   .getRows();

		return index < rows.size() ? rows[index].view
								   : ecs::Entity::InvalidIdentifier;
	}

	Menu &Menu::setItemDisabled(std::size_t index, bool disabled)
	{
		systems::Menu::setItemDisabled(getComponentRegistry(), getIdentifier(),
									   index, disabled);
		return *this;
	}

	Menu &Menu::setAnchor(ecs::Entity::Identifier anchor)
	{
		getComponentRegistry()
			.getComponent<components::Menu>(getIdentifier())
			.setAnchor(anchor);
		systems::Menu::apply(getComponentRegistry(), getIdentifier());
		return *this;
	}

	ecs::Entity::Identifier Menu::getAnchor(void) const
	{
		return getComponentRegistry()
			.getComponent<components::Menu>(getIdentifier())
			.getAnchor();
	}

	Menu &Menu::setAnchorPosition(AnchorPosition position)
	{
		getComponentRegistry()
			.getComponent<components::Menu>(getIdentifier())
			.setAnchorPosition(position);
		systems::Menu::apply(getComponentRegistry(), getIdentifier());
		return *this;
	}

	Menu::AnchorPosition Menu::getAnchorPosition(void) const
	{
		return getComponentRegistry()
			.getComponent<components::Menu>(getIdentifier())
			.getAnchorPosition();
	}

	Menu &Menu::setOnDismiss(std::function<void(void)> onDismiss)
	{
		getComponentRegistry()
			.getComponent<components::Overlay>(getIdentifier())
			.setOnDismissHandler(std::move(onDismiss));
		return *this;
	}

	Menu &Menu::dismiss(void)
	{
		systems::Menu::dismiss(getComponentRegistry(), getIdentifier());
		return *this;
	}

	Menu &Menu::open(void)
	{
		return show();
	}

	Menu &Menu::close(void)
	{
		return hide();
	}

	bool Menu::isOpen(void) const
	{
		return isVisible();
	}

	Menu &Menu::show(void)
	{
		systems::Menu::show(getComponentRegistry(), getIdentifier());
		return *this;
	}

	Menu &Menu::hide(void)
	{
		systems::Menu::hide(getComponentRegistry(), getIdentifier());
		return *this;
	}

	bool Menu::isVisible(void) const
	{
		return getComponentRegistry()
			.getComponent<components::Overlay>(getIdentifier())
			.isVisible();
	}

	void Menu::initialize(void)
	{
		buildItems();
		systems::Menu::apply(getComponentRegistry(), getIdentifier());
	}

	void Menu::buildItems(void)
	{
		// Children need the owning handle as their parent.
		if (weak_from_this().expired()) {
			return;
		}

		auto &registry = getComponentRegistry();
		auto &menu = registry.getComponent<components::Menu>(getIdentifier());

		if (!menu.isRowsDirty()) {
			return;
		}

		// The rows are rebuilt from scratch: remove the previous generation
		// with its components. Everything the menu owns is a row.
		clearEntities();

		std::vector<components::Menu::Row> rows;
		Children children;

		for (const auto &item: menu.getItems()) {
			const auto view = std::make_shared<MenuRow>(registry);
			components::Menu::Row row;

			view->setParent(shared_from_this());
			addEntity(view);
			row.view = view->getIdentifier();

			if (const auto icon = buildIcon(
					registry, *this, view, item.leadingIcon, MenuIconSize,
					schemeColor(SchemeColorRole::OnSurfaceVariant))) {
				row.content.push_back(icon->getIdentifier());
				row.icons.push_back(icon->getIdentifier());
			}
			if (const auto label =
					buildText(registry, *this, view, item.label,
							  theme::TypeRole::BodyLarge,
							  schemeColor(SchemeColorRole::OnSurface))) {
				row.content.push_back(label->getIdentifier());
				row.label = label->getIdentifier();
			}
			if (const auto icon = buildIcon(
					registry, *this, view, item.trailingIcon, MenuIconSize,
					schemeColor(SchemeColorRole::OnSurfaceVariant))) {
				row.content.push_back(icon->getIdentifier());
				row.icons.push_back(icon->getIdentifier());
			}

			rows.push_back(row);
			children.push_back(view);
		}

		menu.setRows(rows).setRowsDirty(false);
		for (std::size_t index = 0; index < rows.size(); ++index) {
			systems::Menu::installRow(registry, getIdentifier(), index);
		}

		// Hand the layout children to the surface.
		setChildren(children);
	}

}	 // namespace guillaume::entities
