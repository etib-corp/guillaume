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

/**
 * @file
 * @brief Widget gallery for manual testing.
 *
 * Every widget of the library, on nine pages:
 * 1. Buttons: common buttons, icon buttons, FABs, FAB menu, segmented button.
 * 2. Selection: checkboxes, radio buttons, radio group, switches, chips.
 * 3. Text and input: type scale, icons, image, text fields, search bar.
 * 4. Surfaces and cards.
 * 5. Dividers, badges and lists.
 * 6. Feedback: progress and loading indicators, snackbar, tooltip, dialog,
 *    menu.
 * 7. App bar and navigation bar.
 * 8. Navigation drawers.
 * 9. A 3D mesh.
 *
 * The Previous and Next buttons at the bottom of each page switch pages. Set
 * GUILLAUME_GALLERY_PAGE to a page number (1 to 9) to open the gallery on
 * that page.
 * Every interaction logs a line on standard output, so a click that does
 * nothing visible can still be checked.
 */

#include <cstddef>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <evan/Engine.hpp>
#include <evan/IPlatform.hpp>

#if defined(__APPLE__)
	#include <evan/glfw/platform/MacOsDesktopPlatform.hpp>
#elif defined(__linux__)
	#include <evan/glfw/platform/LinuxDesktopPlatform.hpp>
#elif defined(_WIN32)
	#include <evan/glfw/platform/WindowsPlatform.hpp>
#endif

#include <guillaume/application.hpp>
#include <guillaume/scene.hpp>
#include <guillaume/theme.hpp>

#include <guillaume/entities/app_bar.hpp>
#include <guillaume/entities/badge.hpp>
#include <guillaume/entities/button.hpp>
#include <guillaume/entities/card.hpp>
#include <guillaume/entities/checkbox.hpp>
#include <guillaume/entities/chip.hpp>
#include <guillaume/entities/dialog.hpp>
#include <guillaume/entities/divider.hpp>
#include <guillaume/entities/extended_floating_action_button.hpp>
#include <guillaume/entities/floating_action_button.hpp>
#include <guillaume/entities/floating_action_button_menu.hpp>
#include <guillaume/entities/icon.hpp>
#include <guillaume/entities/icon_button.hpp>
#include <guillaume/entities/image.hpp>
#include <guillaume/entities/list.hpp>
#include <guillaume/entities/loading_indicator.hpp>
#include <guillaume/entities/menu.hpp>
#include <guillaume/entities/mesh.hpp>
#include <guillaume/entities/navigation_bar.hpp>
#include <guillaume/entities/navigation_drawer.hpp>
#include <guillaume/entities/progress_indicator.hpp>
#include <guillaume/entities/radio_button.hpp>
#include <guillaume/entities/radio_group.hpp>
#include <guillaume/entities/search.hpp>
#include <guillaume/entities/segmented_button.hpp>
#include <guillaume/entities/snackbar.hpp>
#include <guillaume/entities/surface.hpp>
#include <guillaume/entities/switch.hpp>
#include <guillaume/entities/text.hpp>
#include <guillaume/entities/text_field.hpp>
#include <guillaume/entities/tooltip.hpp>
#include <guillaume/entities/view.hpp>

#include <utility/graphic/color.hpp>
#include <utility/graphic/pose.hpp>
#include <utility/ressource_provider.hpp>
#include <utility/system_io/default_system_io.hpp>

namespace
{
	namespace entities = guillaume::entities;
	namespace theme	   = guillaume::theme;

	using guillaume::SchemeColorRole;
	using utility::graphic::Color32Bit;
	using utility::graphic::PoseF;

	using Entities = std::vector<std::shared_ptr<guillaume::ecs::Entity>>;

	/**
	 * @brief Log an interaction on standard output.
	 * @param message What happened.
	 * @return A callback that prints the message, for click handlers.
	 */
	std::function<void(void)> logger(const std::string &message)
	{
		return [message]() {
			std::cout << "[gallery] " << message << std::endl;
		};
	}

	/**
	 * @brief Color of a role in the active theme.
	 * @param role The scheme role.
	 * @return The color.
	 */
	Color32Bit color(SchemeColorRole role)
	{
		return guillaume::getActiveColor(role);
	}

	/**
	 * @brief Base of the gallery pages.
	 *
	 * A page is one root View: a column holding a title, rows of widgets and
	 * a navigation row. Every widget is owned by the row it sits in, so the
	 * scene has a single root to place in front of the camera. Leaving the
	 * page removes everything it built, so entering it again starts fresh.
	 */
	class Page: public guillaume::Scene
	{
		public:
		using guillaume::Scene::Scene;

		/**
		 * @brief Remove the page's entities and their graphics.
		 */
		void onExit(void) override
		{
			getEntityRegistry().clearEntities();
			_rows.clear();
			_page.reset();
		}

		protected:
		/**
		 * @brief Start a page: create the root column and its title.
		 * @param title The page title.
		 */
		void beginPage(const std::string &title)
		{
			entities::View::Builder pageBuilder(getComponentRegistry(),
												getEntityRegistry());
			_page = pageBuilder.makeColorView(
				nullptr, PoseF(), color(SchemeColorRole::Surface), {});
			_page->setAxis(entities::View::Axis::Vertical)
				.setCrossAxisAlignment(
					entities::View::CrossAxisAlignment::Start)
				.setSpacing(20.0f)
				.setPadding(32.0f)
				.setBorderRadius(28.0f);
			_rows.clear();

			entities::Text::Builder textBuilder(getComponentRegistry(), *_page);
			_rows.push_back(textBuilder.makeText(
				_page, title, theme::TypeRole::HeadlineMedium,
				color(SchemeColorRole::OnSurface)));
		}

		/**
		 * @brief Create an empty row owned by the page.
		 * @return The row. Build widgets with it as owner, then call
		 * endRow().
		 */
		std::shared_ptr<entities::View> beginRow(void)
		{
			entities::View::Builder rowBuilder(getComponentRegistry(), *_page);
			auto row = rowBuilder.makeDefaultView(_page, PoseF(), {});
			row->setAxis(entities::View::Axis::Horizontal)
				.setCrossAxisAlignment(
					entities::View::CrossAxisAlignment::Center)
				.setSpacing(16.0f);
			return row;
		}

		/**
		 * @brief Lay out a row: a caption followed by its widgets.
		 * @param row The row from beginRow().
		 * @param caption Short text shown first in the row.
		 * @param widgets The widgets, in order.
		 */
		void endRow(const std::shared_ptr<entities::View> &row,
					const std::string &caption, const Entities &widgets)
		{
			entities::Text::Builder textBuilder(getComponentRegistry(), *row);
			Entities children { textBuilder.makeText(
				row, caption, theme::TypeRole::LabelLarge,
				color(SchemeColorRole::OnSurfaceVariant)) };

			children.insert(children.end(), widgets.begin(), widgets.end());
			row->setEntities(children);
			_rows.push_back(row);
		}

		/**
		 * @brief Add the Previous / Next row and lay out the page.
		 * @tparam Previous The page shown by Previous.
		 * @tparam Next The page shown by Next.
		 */
		template<typename Previous, typename Next> void endPage(void)
		{
			auto row = beginRow();
			entities::Button::Builder buttonBuilder(getComponentRegistry(),
													*row);
			auto previous = buttonBuilder.makeIconButton(
				row, "Previous", "arrow_back",
				guillaume::components::Glyph::Style::Outlined,
				[this]() {
					goToScene<Previous>();
				},
				entities::Button::Color::Tonal, entities::Button::Shape::Round,
				entities::Button::Size::Small, false);
			auto next = buttonBuilder.makeIconButton(
				row, "Next", "arrow_forward",
				guillaume::components::Glyph::Style::Outlined,
				[this]() {
					goToScene<Next>();
				},
				entities::Button::Color::Filled, entities::Button::Shape::Round,
				entities::Button::Size::Small, false);
			endRow(row, "Pages", { previous, next });

			_page->setEntities(_rows);
		}

		/**
		 * @brief Get the page column.
		 * @return The root View of the page.
		 */
		entities::View &page(void)
		{
			return *_page;
		}

		private:
		std::shared_ptr<entities::View> _page;	  ///< Root column.
		Entities _rows;							  ///< Title and rows.
	};

	/// Page 1: buttons.
	class ButtonsPage: public Page
	{
		public:
		using Page::Page;
		void onEnter(void) override;
	};

	/// Page 2: selection controls.
	class SelectionPage: public Page
	{
		public:
		using Page::Page;
		void onEnter(void) override;
	};

	/// Page 3: text and input.
	class TextPage: public Page
	{
		public:
		using Page::Page;
		void onEnter(void) override;
	};

	/// Page 4: surfaces and cards.
	class ContainersPage: public Page
	{
		public:
		using Page::Page;
		void onEnter(void) override;
	};

	/// Page 5: dividers, badges and lists.
	class ListsPage: public Page
	{
		public:
		using Page::Page;
		void onEnter(void) override;
	};

	/// Page 6: feedback and overlays.
	class FeedbackPage: public Page
	{
		public:
		using Page::Page;
		void onEnter(void) override;
	};

	/// Page 7: app bar and navigation bar.
	class NavigationPage: public Page
	{
		public:
		using Page::Page;
		void onEnter(void) override;
	};

	/// Page 8: navigation drawers.
	class DrawerPage: public Page
	{
		public:
		using Page::Page;
		void onEnter(void) override;
	};

	/// Page 9: a 3D model.
	class MeshPage: public Page
	{
		public:
		using Page::Page;
		void onEnter(void) override;
	};

	/**
	 * @brief Jump to the page named by GUILLAUME_GALLERY_PAGE (1 to 9).
	 *
	 * Only on the first entry of the first page, so Previous / Next work
	 * normally afterwards.
	 * @param page The first page.
	 */
	void openRequestedPage(Page &page)
	{
		static bool done  = false;
		const char *value = std::getenv("GUILLAUME_GALLERY_PAGE");

		if (done || value == nullptr) {
			return;
		}
		done = true;
		switch (std::atoi(value)) {
			case 2:
				page.goToScene<SelectionPage>();
				break;
			case 3:
				page.goToScene<TextPage>();
				break;
			case 4:
				page.goToScene<ContainersPage>();
				break;
			case 5:
				page.goToScene<ListsPage>();
				break;
			case 6:
				page.goToScene<FeedbackPage>();
				break;
			case 7:
				page.goToScene<NavigationPage>();
				break;
			case 8:
				page.goToScene<DrawerPage>();
				break;
			case 9:
				page.goToScene<MeshPage>();
				break;
			default:
				break;
		}
	}

	void ButtonsPage::onEnter(void)
	{
		using Button   = entities::Button;
		auto &registry = getComponentRegistry();

		openRequestedPage(*this);
		beginPage("1 / 9  Buttons");

		// Common buttons: one per color style, plus a toggle and a disabled.
		{
			auto row = beginRow();
			Button::Builder builder(registry, *row);
			Entities widgets;
			const std::vector<std::pair<std::string, Button::Color>> styles {
				{ "Elevated", Button::Color::Elevated },
				{ "Filled", Button::Color::Filled },
				{ "Tonal", Button::Color::Tonal },
				{ "Outlined", Button::Color::Outlined },
				{ "Text", Button::Color::Text },
			};
			for (const auto &[label, style]: styles) {
				widgets.push_back(builder.makeButton(
					row, label, logger(label + " button clicked"), style,
					Button::Shape::Round, Button::Size::Small, false));
			}
			auto withIcon = builder.makeIconButton(
				row, "With icon", "add",
				guillaume::components::Glyph::Style::Outlined,
				logger("Icon + label button clicked"), Button::Color::Filled,
				Button::Shape::Square, Button::Size::Small, false);
			widgets.push_back(withIcon);
			auto disabled = builder.makeButton(
				row, "Disabled", logger("BUG"), Button::Color::Filled,
				Button::Shape::Round, Button::Size::Small, false);
			disabled->setDisabled(true);
			widgets.push_back(disabled);
			endRow(row, "Button", widgets);
		}

		// Icon buttons: the four variants, and a toggle.
		{
			using IconButton = entities::IconButton;
			auto row		 = beginRow();
			IconButton::Builder builder(registry, *row);
			Entities widgets;
			const std::vector<std::pair<std::string, IconButton::Variant>>
				variants {
					{ "Standard", IconButton::Variant::Standard },
					{ "Filled", IconButton::Variant::Filled },
					{ "Tonal", IconButton::Variant::Tonal },
					{ "Outlined", IconButton::Variant::Outlined },
				};
			for (const auto &[label, variant]: variants) {
				widgets.push_back(builder.makeIconButton(
					row, "favorite", logger(label + " icon button clicked"),
					variant, IconButton::Size::Medium));
			}
			auto toggle = builder.makeIconButton(
				row, "bookmark", logger("Toggle icon button"),
				IconButton::Variant::Filled, IconButton::Size::Medium);
			toggle->setIsToggle(true);
			toggle->setSelectedIconGlyphName("bookmark_added");
			widgets.push_back(toggle);
			endRow(row, "IconButton", widgets);
		}

		// Floating action buttons: sizes and variants, extended, menu.
		{
			using Fab = entities::FloatingActionButton;
			auto row  = beginRow();
			Fab::Builder builder(registry, *row);
			Entities widgets;
			widgets.push_back(builder.makeFloatingActionButton(
				row, "edit", logger("Small FAB clicked"), Fab::Variant::Primary,
				Fab::Size::Small));
			widgets.push_back(builder.makeFloatingActionButton(
				row, "edit", logger("Medium FAB clicked"),
				Fab::Variant::Secondary, Fab::Size::Medium));
			widgets.push_back(builder.makeFloatingActionButton(
				row, "edit", logger("Large FAB clicked"),
				Fab::Variant::Tertiary, Fab::Size::Large));
			widgets.push_back(builder.makeFloatingActionButton(
				row, "edit", logger("Surface FAB clicked"),
				Fab::Variant::Surface, Fab::Size::Medium));

			entities::ExtendedFloatingActionButton::Builder extendedBuilder(
				registry, *row);
			auto extended = extendedBuilder.makeExtendedFloatingActionButton(
				row, "Compose", {});
			// Each click collapses or expands the label.
			auto *extendedPointer = extended.get();
			extended->setOnClick([extendedPointer]() {
				extendedPointer->setExtended(!extendedPointer->isExtended());
				logger("Extended FAB toggled")();
			});
			widgets.push_back(extended);

			entities::FloatingActionButtonMenu::Builder menuBuilder(registry,
																	*row);
			widgets.push_back(
				menuBuilder.addAction("mail", "Mail", logger("FAB menu: mail"))
					.addAction("chat", "Chat", logger("FAB menu: chat"))
					.addAction("call", "Call", logger("FAB menu: call"))
					.makeFloatingActionButtonMenu(row, "add"));
			endRow(row, "FAB", widgets);
		}

		// Segmented buttons: single and multi selection.
		{
			using Segmented = entities::SegmentedButton;
			auto row		= beginRow();
			Segmented::Builder builder(registry, *row);
			Entities widgets;
			widgets.push_back(builder.makeSegmentedButton(
				row, { "Day", "Week", "Month" },
				Segmented::SelectionMode::Single, [](std::size_t index) {
					logger("Segment selected: " + std::to_string(index))();
				}));
			auto multi = builder.makeSegmentedButton(
				row, { "Bold", "Italic", "Underline" },
				Segmented::SelectionMode::Multi, [](std::size_t index) {
					logger("Segment toggled: " + std::to_string(index))();
				});
			multi->setSegmentDisabled(2, true);
			widgets.push_back(multi);
			endRow(row, "SegmentedButton", widgets);
		}

		endPage<MeshPage, SelectionPage>();
	}

	void SelectionPage::onEnter(void)
	{
		auto &registry = getComponentRegistry();

		beginPage("2 / 9  Selection");

		// Checkboxes: the three states and the three variants.
		{
			using Checkbox = entities::Checkbox;
			auto row	   = beginRow();
			Checkbox::Builder builder(registry, *row);
			Entities widgets;
			auto onChange = [](Checkbox::State state) {
				logger("Checkbox state: "
					   + std::to_string(static_cast<int>(state)))();
			};
			auto unchecked =
				builder.makeCheckbox(row, Checkbox::State::Unchecked,
									 Checkbox::Variant::Enabled, "Unchecked");
			unchecked->setOnValueChange(onChange);
			auto checked =
				builder.makeCheckbox(row, Checkbox::State::Checked,
									 Checkbox::Variant::Enabled, "Checked");
			checked->setOnValueChange(onChange);
			auto mixed = builder.makeCheckbox(
				row, Checkbox::State::Indeterminate, Checkbox::Variant::Enabled,
				"Indeterminate");
			mixed->setOnValueChange(onChange);
			widgets.push_back(unchecked);
			widgets.push_back(checked);
			widgets.push_back(mixed);
			widgets.push_back(
				builder.makeCheckbox(row, Checkbox::State::Checked,
									 Checkbox::Variant::Error, "Error"));
			widgets.push_back(
				builder.makeCheckbox(row, Checkbox::State::Checked,
									 Checkbox::Variant::Disabled, "Disabled"));
			endRow(row, "Checkbox", widgets);
		}

		// Radio buttons, and a radio group that keeps one selection.
		{
			using Radio = entities::RadioButton;
			auto row	= beginRow();
			Radio::Builder builder(registry, *row);
			Entities widgets;
			auto enabled = builder.makeRadioButton(row, Radio::Variant::Enabled,
												   "Enabled");
			enabled->setOnPress(logger("Radio pressed"));
			widgets.push_back(enabled);
			widgets.push_back(
				builder.makeRadioButton(row, Radio::Variant::Error, "Error"));
			widgets.push_back(builder.makeRadioButton(
				row, Radio::Variant::Disabled, "Disabled"));

			entities::RadioGroup::Builder groupBuilder(registry, *row);
			auto group = groupBuilder.makeRadioGroup(
				row, { "Small", "Medium", "Large" });
			widgets.push_back(group);
			endRow(row, "Radio", widgets);
		}

		// Switches.
		{
			using Switch = entities::Switch;
			auto row	 = beginRow();
			Switch::Builder builder(registry, *row);
			Entities widgets;
			auto off = builder.makeSwitch(row, Switch::Variant::Enabled, false,
										  "", "Off");
			off->setOnValueChange([](bool on) {
				logger(std::string("Switch: ") + (on ? "on" : "off"))();
			});
			widgets.push_back(off);
			widgets.push_back(builder.makeSwitch(row, Switch::Variant::Enabled,
												 true, "", "On"));
			widgets.push_back(builder.makeSwitch(row, Switch::Variant::WithIcon,
												 true, "check", "With icon"));
			widgets.push_back(builder.makeSwitch(row, Switch::Variant::Disabled,
												 true, "", "Disabled"));
			endRow(row, "Switch", widgets);
		}

		// Chips: the four variants.
		{
			using Chip = entities::Chip;
			auto row   = beginRow();
			Chip::Builder builder(registry, *row);
			Entities widgets;
			auto assist = builder.makeChip(row, Chip::Variant::Assist, "Assist",
										   "event", false);
			assist->setOnPress(logger("Assist chip pressed"));
			widgets.push_back(assist);
			widgets.push_back(builder.makeChip(row, Chip::Variant::Filter,
											   "Filter", "", true));
			auto input =
				builder.makeChip(row, Chip::Variant::Input, "Input", "", false);
			input->setOnClose(logger("Input chip closed"));
			widgets.push_back(input);
			widgets.push_back(builder.makeChip(row, Chip::Variant::Suggestion,
											   "Suggestion", "", false));
			auto outlined = builder.makeChip(row, Chip::Variant::Filter,
											 "Outlined", "", false);
			outlined->setMode(Chip::Mode::Outlined);
			widgets.push_back(outlined);
			endRow(row, "Chip", widgets);
		}

		endPage<ButtonsPage, TextPage>();
	}

	void TextPage::onEnter(void)
	{
		auto &registry = getComponentRegistry();

		beginPage("3 / 9  Text and input");

		// The type scale, one sample per group.
		{
			auto row = beginRow();
			entities::Text::Builder builder(registry, *row);
			Entities widgets;
			const std::vector<std::pair<std::string, theme::TypeRole>> roles {
				{ "Display", theme::TypeRole::DisplaySmall },
				{ "Headline", theme::TypeRole::HeadlineSmall },
				{ "Title", theme::TypeRole::TitleMedium },
				{ "Body", theme::TypeRole::BodyMedium },
				{ "Label", theme::TypeRole::LabelSmall },
			};
			for (const auto &[label, role]: roles) {
				widgets.push_back(builder.makeText(
					row, label, role, color(SchemeColorRole::OnSurface)));
			}
			endRow(row, "Text", widgets);
		}

		// Icons and an image.
		{
			auto row = beginRow();
			entities::Icon::Builder builder(registry, *row);
			Entities widgets;
			for (const std::string glyph:
				 { "home", "search", "settings", "favorite", "delete" }) {
				widgets.push_back(builder.makeIcon(
					row, glyph, 32.0f, color(SchemeColorRole::Primary),
					guillaume::components::Glyph::Style::Outlined));
			}
			entities::Image::Builder imageBuilder(registry, *row);
			auto image =
				imageBuilder.makeImage(row, "textures/default_texture.png");
			image->setSize(64.0f, 64.0f);
			widgets.push_back(image);
			endRow(row, "Icon, Image", widgets);
		}

		// Text fields: filled, outlined, with an error.
		{
			using TextField = entities::TextField;
			auto row		= beginRow();
			TextField::Builder builder(registry, *row);
			Entities widgets;
			auto filled =
				builder.makeTextField(row, TextField::Variant::Filled, "Name",
									  "person", "", "Click, then type", false);
			filled->setOnChange([](const std::string &text) {
				logger("Text field: " + text)();
			});
			widgets.push_back(filled);
			widgets.push_back(builder.makeTextField(
				row, TextField::Variant::Outlined, "Email", "", "mail", "",
				false, "someone@example.com"));
			auto error =
				builder.makeTextField(row, TextField::Variant::Filled, "Age",
									  "", "error", "", true, "abc");
			error->setErrorText("Enter a number");
			widgets.push_back(error);
			endRow(row, "TextField", widgets);
		}

		// Search bar.
		{
			auto row = beginRow();
			entities::Search::Builder builder(registry, *row);
			auto search =
				builder.makeSearch(row, entities::Search::Variant::SearchBar,
								   "Search the gallery");
			search->setOnIconPress(logger("Search icon pressed"));
			endRow(row, "Search", { search });
		}

		endPage<SelectionPage, ContainersPage>();
	}

	void ContainersPage::onEnter(void)
	{
		auto &registry = getComponentRegistry();

		beginPage("4 / 9  Surfaces and cards");

		// Surfaces at three elevation levels, flat and elevated.
		{
			using Surface = entities::Surface;
			auto row	  = beginRow();
			Surface::Builder builder(registry, *row);
			Entities widgets;
			const std::vector<theme::ElevationLevel> levels {
				theme::ElevationLevel::Level0, theme::ElevationLevel::Level1,
				theme::ElevationLevel::Level3, theme::ElevationLevel::Level5
			};
			for (const auto level: levels) {
				auto surface = builder.withShape(theme::ShapeRole::Medium)
								   .makeSurface(row, level);
				surface->setSurfaceConfig(surface->getSurfaceConfig()
											  .setFixedWidth(72.0f)
											  .setFixedHeight(48.0f));
				widgets.push_back(surface);
			}
			auto flat = builder.makeSurface(row, theme::ElevationLevel::Level3,
											Surface::Mode::Flat);
			flat->setSurfaceConfig(
				flat->getSurfaceConfig().setFixedWidth(72.0f).setFixedHeight(
					48.0f));
			widgets.push_back(flat);
			endRow(row, "Surface", widgets);
		}

		// Cards: the three variants, the last one with every section.
		{
			using Card = entities::Card;
			auto row   = beginRow();
			Card::Builder builder(registry, *row);
			Entities widgets;
			auto elevated = builder.makeCard(row, Card::Variant::Elevated);
			elevated->setTitle("Elevated", "Card");
			elevated->setOnPress(logger("Elevated card pressed"));
			widgets.push_back(elevated);
			auto filled = builder.makeCard(row, Card::Variant::Filled);
			filled->setTitle("Filled", "Card");
			widgets.push_back(filled);
			auto outlined = builder.makeCard(row, Card::Variant::Outlined);
			outlined->setCover("textures/default_texture.png");
			outlined->setTitle("Outlined", "With every section");
			outlined->setContent("Cover, title, content and actions.");
			outlined->addAction("Share", logger("Card action: share"));
			outlined->addAction("Open", logger("Card action: open"));
			widgets.push_back(outlined);
			endRow(row, "Card", widgets);
		}

		endPage<TextPage, ListsPage>();
	}

	void ListsPage::onEnter(void)
	{
		auto &registry = getComponentRegistry();

		beginPage("5 / 9  Dividers, badges and lists");

		// Dividers and badges.
		{
			using Divider = entities::Divider;
			using Badge	  = entities::Badge;
			auto row	  = beginRow();
			Divider::Builder dividerBuilder(registry, *row);
			Badge::Builder badgeBuilder(registry, *row);
			Entities widgets;
			widgets.push_back(dividerBuilder.makeDivider(
				row, 120.0f, Divider::Variant::FullWidth));
			widgets.push_back(dividerBuilder.makeDivider(
				row, 120.0f, Divider::Variant::Inset));
			widgets.push_back(
				badgeBuilder.makeBadge(row, "", Badge::Variant::Dot));
			widgets.push_back(
				badgeBuilder.makeBadge(row, "3", Badge::Variant::Small));
			widgets.push_back(
				badgeBuilder.makeBadge(row, "999+", Badge::Variant::Large));
			endRow(row, "Divider, Badge", widgets);
		}

		// List with a subheader, rows, a divider and an accordion.
		{
			using List = entities::List;
			auto row   = beginRow();
			List::Builder builder(registry, *row);

			List::Item inbox;
			inbox.headline	  = "Inbox";
			inbox.supporting  = "3 unread";
			inbox.leadingIcon = "inbox";
			inbox.onPress	  = logger("List: inbox");
			List::Item sent;
			sent.headline	 = "Sent";
			sent.leadingIcon = "send";
			sent.onPress	 = logger("List: sent");
			List::Item folders;
			folders.headline	= "Folders (click to expand)";
			folders.leadingIcon = "folder";
			List::Item work;
			work.headline	  = "Work";
			work.onPress	  = logger("List: work");
			List::Item travel = work;
			travel.headline	  = "Travel (disabled)";
			travel.disabled	  = true;
			folders.children  = { work, travel };

			auto list = builder.makeList(row, List::Variant::TwoLine,
										 { List::subheader("Mail"), inbox, sent,
										   List::divider(), folders });
			endRow(row, "List", { list });
		}

		endPage<ContainersPage, FeedbackPage>();
	}

	void FeedbackPage::onEnter(void)
	{
		auto &registry = getComponentRegistry();

		beginPage("6 / 9  Feedback");

		// Progress and loading indicators.
		{
			using Progress = entities::ProgressIndicator;
			using Loading  = entities::LoadingIndicator;
			auto row	   = beginRow();
			Progress::Builder progressBuilder(registry, *row);
			Loading::Builder loadingBuilder(registry, *row);
			Entities widgets;
			widgets.push_back(progressBuilder.makeProgressIndicator(
				row, Progress::Variant::LinearDeterminate, 0.6f));
			widgets.push_back(progressBuilder.makeProgressIndicator(
				row, Progress::Variant::LinearIndeterminate, 0.0f));
			widgets.push_back(progressBuilder.makeProgressIndicator(
				row, Progress::Variant::CircularDeterminate, 0.3f));
			widgets.push_back(progressBuilder.makeProgressIndicator(
				row, Progress::Variant::CircularIndeterminate, 0.0f));
			widgets.push_back(loadingBuilder.makeLoadingIndicator(
				row, Loading::Variant::Circular));
			widgets.push_back(loadingBuilder.makeLoadingIndicator(
				row, Loading::Variant::Linear));
			endRow(row, "Progress, Loading", widgets);
		}

		// Overlays, each opened by a button. The snackbar sits in the row;
		// the dialog, the menu and the tooltip place themselves.
		{
			using Button = entities::Button;
			auto row	 = beginRow();
			Button::Builder buttonBuilder(registry, *row);
			Entities widgets;

			entities::Snackbar::Builder snackbarBuilder(registry, *row);
			auto snackbar = snackbarBuilder.makeSnackbar(
				row, entities::Snackbar::Variant::WithAction, "Message sent",
				"Undo");
			snackbar->setOnAction(logger("Snackbar action"));
			snackbar->setOnDismiss(logger("Snackbar dismissed"));
			snackbar->setVisible(false);
			auto *snackbarPointer = snackbar.get();
			widgets.push_back(buttonBuilder.makeButton(
				row, "Snackbar",
				[snackbarPointer]() {
					snackbarPointer->show();
				},
				Button::Color::Tonal, Button::Shape::Round, Button::Size::Small,
				false));

			entities::Dialog::Builder dialogBuilder(registry, *row);
			auto dialog = dialogBuilder.makeDialog(
				row, entities::Dialog::Variant::Alert, "Discard draft?",
				"This cannot be undone.", { "Cancel", "Discard" });
			dialog->setOnDismiss(logger("Dialog dismissed"));
			auto *dialogPointer = dialog.get();
			widgets.push_back(buttonBuilder.makeButton(
				row, "Dialog",
				[dialogPointer]() {
					dialogPointer->show();
				},
				Button::Color::Tonal, Button::Shape::Round, Button::Size::Small,
				false));

			auto menuTrigger = buttonBuilder.makeButton(
				row, "Menu", {}, Button::Color::Tonal, Button::Shape::Round,
				Button::Size::Small, false);
			entities::Menu::Item cut;
			cut.label		= "Cut";
			cut.leadingIcon = "content_cut";
			cut.onPress		= logger("Menu: cut");
			entities::Menu::Item copy;
			copy.label		 = "Copy";
			copy.leadingIcon = "content_copy";
			copy.onPress	 = logger("Menu: copy");
			entities::Menu::Item paste;
			paste.label		  = "Paste (disabled)";
			paste.leadingIcon = "content_paste";
			paste.disabled	  = true;
			entities::Menu::Builder menuBuilder(registry, *row);
			auto menu = menuBuilder.makeMenu(
				row, entities::Menu::Variant::Dropdown, { cut, copy, paste });
			menu->setAnchor(menuTrigger->getIdentifier());
			auto *menuPointer = menu.get();
			menuTrigger->setOnClick([menuPointer]() {
				menuPointer->show();
			});
			widgets.push_back(menuTrigger);

			auto tooltipAnchor = buttonBuilder.makeButton(
				row, "Hover me", logger("Tooltip anchor clicked"),
				Button::Color::Outlined, Button::Shape::Round,
				Button::Size::Small, false);
			entities::Tooltip::Builder tooltipBuilder(registry, *row);
			auto tooltip = tooltipBuilder.makeTooltip(
				row, entities::Tooltip::Variant::Plain, "A plain tooltip");
			tooltip->setAnchor(tooltipAnchor);
			widgets.push_back(tooltipAnchor);

			widgets.push_back(snackbar);
			endRow(row, "Overlays", widgets);
		}

		endPage<ListsPage, NavigationPage>();
	}

	void NavigationPage::onEnter(void)
	{
		auto &registry = getComponentRegistry();

		beginPage("7 / 9  App bar and navigation bar");

		// App bar.
		{
			using AppBar = entities::AppBar;
			auto row	 = beginRow();
			AppBar::Builder builder(registry, *row);
			auto appBar =
				builder.withTitle("Gallery")
					.withNavigationIcon("menu")
					.withOnNavigation(logger("App bar navigation"), "Menu")
					.addAction("search", logger("App bar: search"), "Search")
					.addAction("more_vert", logger("App bar: more"), "More")
					.withFixedWidth(480.0f)
					.makeAppBar(row, AppBar::Variant::Small);
			endRow(row, "AppBar", { appBar });
		}

		// Navigation bar.
		{
			using Bar = entities::NavigationBar;
			auto row  = beginRow();
			Bar::Builder builder(registry, *row);
			auto bar = builder.makeNavigationBar(row, Bar::Variant::WithLabels,
												 { { "home", "Home" },
												   { "explore", "Explore" },
												   { "person", "Profile" } });
			bar->setOnIndexChange([](std::size_t index) {
				logger("Navigation bar: " + std::to_string(index))();
			});
			bar->setBadge(1, "3");
			endRow(row, "NavigationBar", { bar });
		}

		endPage<FeedbackPage, DrawerPage>();
	}

	void DrawerPage::onEnter(void)
	{
		auto &registry = getComponentRegistry();

		beginPage("8 / 9  Navigation drawers");

		// Navigation drawer, standard and collapsed.
		{
			using Drawer = entities::NavigationDrawer;
			auto row	 = beginRow();
			Drawer::Builder builder(registry, *row);
			Entities widgets;
			auto drawer =
				builder.makeNavigationDrawer(row, Drawer::Variant::Standard,
											 { { "inbox", "Inbox" },
											   { "send", "Sent" },
											   { "delete", "Trash" } });
			drawer->setOnIndexChange([](std::size_t index) {
				logger("Drawer: " + std::to_string(index))();
			});
			widgets.push_back(drawer);
			widgets.push_back(builder.makeNavigationDrawer(
				row, Drawer::Variant::Collapsed,
				{ { "inbox", "Inbox" }, { "send", "Sent" } }));

			endRow(row, "NavigationDrawer", widgets);
		}

		endPage<NavigationPage, MeshPage>();
	}

	void MeshPage::onEnter(void)
	{
		beginPage("9 / 9  3D mesh");

		auto row = beginRow();
		entities::Mesh::Builder meshBuilder(getComponentRegistry(), *row);
		// No texture: the engine draws an untextured model flat, which
		// shows its silhouette on the light page.
		auto teapot = meshBuilder.makeMesh(row, "models/teapot.obj", "");
		teapot->setFitSize(160.0f);
		endRow(row, "Mesh", { teapot });

		endPage<DrawerPage, ButtonsPage>();
	}

	/**
	 * @brief Create a desktop platform appropriate for the current OS.
	 * @return The platform instance, or nullptr on unsupported systems.
	 */
	std::shared_ptr<evan::IPlatform> makePlatform(void)
	{
#if defined(__APPLE__)
		return std::make_shared<evan::MacOsDesktopPlatform>(
			"Guillaume widget gallery", 1600, 1000);
#elif defined(__linux__)
		return std::make_shared<evan::LinuxDesktopPlatform>(
			"Guillaume widget gallery", 1600, 1000);
#elif defined(_WIN32)
		return std::make_shared<evan::WindowsDesktopPlatform>(
			"Guillaume widget gallery", 1600, 1000);
#else
		return nullptr;
#endif
	}

}	 // namespace

int main(void)
{
	auto platform = makePlatform();

	utility::DefaultSystemIO systemIo;
	auto resources = std::make_shared<utility::RessourceProvider>(systemIo);

	auto engine =
		std::make_unique<evan::Engine>(resources, std::move(platform));

	guillaume::Application<ButtonsPage, SelectionPage, TextPage, ContainersPage,
						   ListsPage, FeedbackPage, NavigationPage, DrawerPage,
						   MeshPage>
		app(resources, std::move(engine));

	return app.run();
}
