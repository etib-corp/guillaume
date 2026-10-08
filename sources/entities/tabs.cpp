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

#include "guillaume/entities/tabs.hpp"

#include <cstdint>
#include <memory>
#include <vector>

#include "guillaume/ecs/parent_entity_filler.hpp"

#include "guillaume/components/borders.hpp"
#include "guillaume/components/bound.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/layout.hpp"
#include "guillaume/components/selection.hpp"
#include "guillaume/components/transform.hpp"

#include "guillaume/entities/content_helpers.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/style_helpers.hpp"
#include "guillaume/systems/layout.hpp"
#include "guillaume/systems/selection.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::entities
{
	namespace
	{
		constexpr float TabHeight = 48.0f;

		/**
		 * @brief Internal tab row: a selectable layout holding a label.
		 */
		class Tab:
			public std::enable_shared_from_this<Tab>,
			public ecs::ParentEntityFiller<
				components::Layout, components::Transform, components::Bound,
				components::Color, components::Borders, components::Selectable>
		{
			private:
			std::string _content {};	///< Label content.
			utility::graphic::Color32Bit _contentColor {};	  ///< Label color.
			std::shared_ptr<Text> _label {};				  ///< Tab label.

			public:
			/**
			 * @brief Construct a tab row.
			 * @param registry The component registry.
			 * @param content The label content.
			 * @param color The label color.
			 */
			Tab(ecs::ComponentRegistry &registry, const std::string &content,
				const utility::graphic::Color32Bit &color)
				: ecs::ParentEntityFiller<
					  components::Layout, components::Transform,
					  components::Bound, components::Color, components::Borders,
					  components::Selectable>(registry)
				, _content(content)
				, _contentColor(color)
				, _label()
			{
				getComponentRegistry()
					.getComponent<components::Layout>(getIdentifier())
					.setAxis(components::Layout::Axis::Horizontal)
					.setMainAxisAlignment(
						components::Layout::MainAxisAlignment::Center)
					.setCrossAxisAlignment(
						components::Layout::CrossAxisAlignment::Center)
					.setSpacing(8.0f)
					.setPadding(16.0f)
					.setFixedHeight(TabHeight);

				getComponentRegistry()
					.getComponent<components::Color>(getIdentifier())
					.setColor(transparentColor());

				getComponentRegistry()
					.getComponent<components::Borders>(getIdentifier())
					.setBorderRadius(0.0f)
					.setColor(transparentColor());
			}

			/**
			 * @brief Default destructor.
			 */
			~Tab(void) override = default;

			/**
			 * @brief Build the label child now that the entity is owned.
			 */
			void initialize(void) override
			{
				_label =
					buildText(getComponentRegistry(), *this, shared_from_this(),
							  _content, 14.0f, _contentColor);

				applyGeometry();
			}

			/**
			 * @brief Set the tab label color.
			 * @param color The new label color.
			 */
			void setContentColor(const utility::graphic::Color32Bit &color)
			{
				_contentColor = color;
				if (_label != nullptr) {
					_label->setColor(color);
				}
			}

			/**
			 * @brief Set the tab selected state.
			 * @param selected The new selected state.
			 */
			void setSelected(bool selected)
			{
				getComponentRegistry()
					.getComponent<components::Selectable>(getIdentifier())
					.setSelected(selected);
			}

			/**
			 * @brief Whether the tab is selected.
			 * @return True when selected.
			 */
			bool isSelected(void)
			{
				return getComponentRegistry()
					.getComponent<components::Selectable>(getIdentifier())
					.isSelected();
			}

			/**
			 * @brief Set the selection change handler.
			 * @param handler The handler to invoke on selection change.
			 */
			void setOnSelectionChanged(
				const components::Selectable::Handler &handler)
			{
				getComponentRegistry()
					.getComponent<components::Selectable>(getIdentifier())
					.setOnSelectionChangedHandler(handler);
			}

			/**
			 * @brief Set the bottom indicator (border) color.
			 * @param color The new indicator color.
			 */
			void setIndicatorColor(const utility::graphic::Color32Bit &color)
			{
				getComponentRegistry()
					.getComponent<components::Borders>(getIdentifier())
					.setColor(color);
			}

			/**
			 * @brief Get the label child identifier.
			 * @return The label identifier, or InvalidIdentifier.
			 */
			ecs::Entity::Identifier getLabelIdentifier(void) const
			{
				return _label != nullptr ? _label->getIdentifier()
										 : ecs::Entity::InvalidIdentifier;
			}

			/**
			 * @brief Arrange the tab content.
			 */
			void applyGeometry(void)
			{
				std::vector<ecs::Entity::Identifier> children;
				if (_label != nullptr) {
					children.push_back(_label->getIdentifier());
				}

				systems::Layout::apply(getComponentRegistry(), getIdentifier(),
									   children,
									   static_cast<std::uint32_t>(getLayer()));
			}
		};
	}	 // namespace

	Tabs::Builder::Builder(ecs::ComponentRegistry &componentRegistry,
						   ecs::EntityRegistry &entityRegistry)
		: EntityBuilderBase<Tabs>(componentRegistry, entityRegistry)
	{
		reset();
	}

	std::shared_ptr<Tabs> Tabs::Builder::buildEntity(void)
	{
		auto entity =
			std::make_shared<Tabs>(this->getComponentRegistry(), _config,
								   _variant, _labels, _selectedIndex);
		return entity;
	}

	void Tabs::Builder::reset(void)
	{
		_config	 = SurfaceConfig();
		_variant = Variant::Primary;
		_labels.clear();
		_selectedIndex = 0;

		_config.borderRadius = 0.0f;
		_config.axis		 = components::Layout::Axis::Horizontal;
		_config.mainAxisAlignment =
			components::Layout::MainAxisAlignment::Start;
		_config.crossAxisAlignment =
			components::Layout::CrossAxisAlignment::Center;
		_config.spacing = 0.0f;
		_config.padding = 0.0f;
	}

	Tabs::Builder &Tabs::Builder::withPose(const utility::graphic::PoseF &pose)
	{
		_config.pose = pose;
		return *this;
	}

	Tabs::Builder &Tabs::Builder::withVariant(Variant variant)
	{
		_variant = variant;
		return *this;
	}

	Tabs::Builder &
		Tabs::Builder::withLabels(const std::vector<std::string> &labels)
	{
		_labels = labels;
		return *this;
	}

	Tabs::Builder &Tabs::Builder::withSelectedIndex(std::size_t selectedIndex)
	{
		_selectedIndex = selectedIndex;
		return *this;
	}

	std::shared_ptr<Tabs> Tabs::Director::makeTabs(
		Builder &builder, std::shared_ptr<ecs::Entity> parent, Variant variant,
		const std::vector<std::string> &labels, std::size_t selectedIndex)
	{
		return builder.withVariant(variant)
			.withLabels(labels)
			.withSelectedIndex(selectedIndex)
			.registerEntity(parent);
	}

	Tabs::Tabs(ecs::ComponentRegistry &registry, const SurfaceConfig &config,
			   Variant variant, const std::vector<std::string> &labels,
			   std::size_t selectedIndex)
		: SurfaceBase<components::SelectionGroup>(registry, config)
		, _variant(variant)
		, _labels(labels)
		, _tabs()
		, _selectedIndex(selectedIndex)
		, _updatingSelection(false)
	{
		getComponentRegistry()
			.getComponent<components::SelectionGroup>(getIdentifier())
			.setAllowEmpty(false);
	}

	Tabs::~Tabs(void)
	{
	}

	void Tabs::buildTabs(void)
	{
		_tabs.clear();

		const auto contentColor =
			schemeColor(SchemeColorRole::OnSurfaceVariant);

		for (const auto &label: _labels) {
			auto tab = std::make_shared<Tab>(getComponentRegistry(), label,
											 contentColor);
			tab->setParent(shared_from_this());
			this->addEntity(tab);
			_tabs.push_back(tab);
		}

		for (std::size_t i = 0; i < _tabs.size(); ++i) {
			auto tab = std::dynamic_pointer_cast<Tab>(_tabs[i]);
			if (tab == nullptr) {
				continue;
			}

			tab->setOnSelectionChanged([this, i](bool selected) {
				if (this->_updatingSelection || !selected) {
					return;
				}

				this->_updatingSelection = true;
				this->_selectedIndex	 = i;
				this->selectEntity(this->_tabs[i]->getIdentifier());
				this->_updatingSelection = false;

				this->applyVariant();
			});
		}

		setChildren(_tabs);
	}

	void Tabs::selectEntity(ecs::Entity::Identifier identifier)
	{
		std::vector<ecs::Entity::Identifier> childIdentifiers;
		childIdentifiers.reserve(_tabs.size());

		for (const auto &tab: _tabs) {
			childIdentifiers.push_back(tab->getIdentifier());
		}

		systems::Selection::select(getComponentRegistry(), getIdentifier(),
								   childIdentifiers, identifier);
	}

	void Tabs::applyVariant(void)
	{
		const auto selectedColor = (_variant == Variant::Secondary)
			? schemeColor(SchemeColorRole::Secondary)
			: schemeColor(SchemeColorRole::Primary);
		const auto unselectedColor =
			schemeColor(SchemeColorRole::OnSurfaceVariant);

		for (auto &entity: _tabs) {
			auto tab = std::dynamic_pointer_cast<Tab>(entity);
			if (tab == nullptr) {
				continue;
			}

			const bool selected = tab->isSelected();

			tab->setContentColor(selected ? selectedColor : unselectedColor);
			tab->setIndicatorColor(selected ? selectedColor
											: transparentColor());
		}
	}

	void Tabs::applySelection(void)
	{
		if (_tabs.empty()) {
			return;
		}

		if (_selectedIndex >= _tabs.size()) {
			_selectedIndex = _tabs.size() - 1;
		}

		_updatingSelection = true;
		selectEntity(_tabs[_selectedIndex]->getIdentifier());
		_updatingSelection = false;

		applyVariant();
	}

	Tabs &Tabs::setVariant(Variant variant)
	{
		_variant = variant;
		applyVariant();
		return *this;
	}

	Tabs::Variant Tabs::getVariant(void) const
	{
		return _variant;
	}

	Tabs &Tabs::setLabels(const std::vector<std::string> &labels)
	{
		_labels = labels;
		buildTabs();
		applySelection();
		return *this;
	}

	Tabs &Tabs::select(std::size_t index)
	{
		if (index >= _tabs.size()) {
			return *this;
		}

		_selectedIndex = index;
		applySelection();

		return *this;
	}

	std::size_t Tabs::getSelectedIndex(void) const
	{
		for (std::size_t i = 0; i < _tabs.size(); ++i) {
			auto tab = std::dynamic_pointer_cast<Tab>(_tabs[i]);
			if (tab != nullptr && tab->isSelected()) {
				return i;
			}
		}

		return _selectedIndex;
	}

	std::size_t Tabs::getTabCount(void) const
	{
		return _tabs.size();
	}
	ecs::Entity::Identifier Tabs::getTabIdentifier(std::size_t index) const
	{
		if (index >= _tabs.size()) {
			return ecs::Entity::InvalidIdentifier;
		}

		return _tabs[index]->getIdentifier();
	}

	void Tabs::initialize(void)
	{
		SurfaceBase::initialize();

		if (_tabs.empty()) {
			buildTabs();
		}

		applySelection();
	}

	void Tabs::update(void)
	{
		SurfaceBase::update();

		for (auto &entity: _tabs) {
			auto tab = std::dynamic_pointer_cast<Tab>(entity);
			if (tab != nullptr) {
				tab->applyGeometry();
			}
		}

		applyVariant();
	}

}	 // namespace guillaume::entities
