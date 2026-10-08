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

#include "guillaume/entities/date_picker.hpp"
#include "guillaume/entities/overlay_helpers.hpp"

#include <cstddef>
#include <cstdio>
#include <memory>
#include <string>
#include <vector>

#include "guillaume/ecs/entity_filler.hpp"

#include "guillaume/components/borders.hpp"
#include "guillaume/components/bound.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/layout.hpp"
#include "guillaume/components/selection.hpp"
#include "guillaume/components/transform.hpp"
#include "guillaume/systems/layout.hpp"
#include "guillaume/systems/selection.hpp"

#include "guillaume/entities/content_helpers.hpp"
#include "guillaume/entities/overlay_helpers.hpp"
#include "guillaume/entities/layout.hpp"
#include "guillaume/entities/overlay_helpers.hpp"
#include "guillaume/entities/placement_helpers.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/style_helpers.hpp"
#include "guillaume/entities/overlay_helpers.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::entities
{
	namespace
	{
		constexpr float DayCellSize	  = 40.0f;
		constexpr float DayCellRadius = 20.0f;
		constexpr int DaysPerWeek	  = 7;
		constexpr int WeekRows		  = 6;

		/**
		 * @brief Get the number of days in a month.
		 * @param year The year.
		 * @param month The month (1-12).
		 * @return The number of days.
		 */
		int daysInMonth(int year, int month)
		{
			static const int lengths[12] = { 31, 28, 31, 30, 31, 30,
											 31, 31, 30, 31, 30, 31 };

			if (month < 1 || month > 12) {
				return 31;
			}

			if (month == 2
				&& ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)) {
				return 29;
			}

			return lengths[month - 1];
		}

		/**
		 * @brief Get the weekday index (Sunday = 0) of the first of a month.
		 * @param year The year.
		 * @param month The month (1-12).
		 * @return The weekday index.
		 */
		int firstWeekday(int year, int month)
		{
			int y = year;
			int m = month;

			if (m < 3) {
				m += 12;
				y -= 1;
			}

			const int k = y % 100;
			const int j = y / 100;
			const int h =
				(1 + (13 * (m + 1)) / 5 + k + k / 4 + j / 4 + 5 * j) % 7;

			return (h + 6) % 7;
		}
	}	 // namespace

	/**
	 * @brief Internal selectable day cell of a `DatePicker`.
	 */
	class DatePicker::DayCell:
		public std::enable_shared_from_this<DatePicker::DayCell>,
		public ecs::ParentEntityFiller<
			components::Transform, components::Bound, components::Color,
			components::Borders, components::Selectable, components::Layout>
	{
		private:
		int _day { 1 };	   ///< Day number represented by the cell.
		std::shared_ptr<Text> _label {};	///< Day number label child.

		public:
		/**
		 * @brief Construct a day cell.
		 * @param registry The component registry.
		 * @param day The day number (0 when the cell is a padding cell).
		 */
		DayCell(ecs::ComponentRegistry &registry, int day)
			: ecs::ParentEntityFiller<components::Transform, components::Bound,
									  components::Color, components::Borders,
									  components::Selectable,
									  components::Layout>(registry)
			, _day(day)
			, _label()
		{
			getComponentRegistry()
				.getComponent<components::Bound>(getIdentifier())
				.setWidth(DayCellSize)
				.setHeight(DayCellSize);

			getComponentRegistry()
				.getComponent<components::Borders>(getIdentifier())
				.setBorderRadius(DayCellRadius);

			getComponentRegistry()
				.getComponent<components::Color>(getIdentifier())
				.setColor(transparentColor());

			getComponentRegistry()
				.getComponent<components::Layout>(getIdentifier())
				.setAxis(components::Layout::Axis::Horizontal)
				.setMainAxisAlignment(
					components::Layout::MainAxisAlignment::Center)
				.setCrossAxisAlignment(
					components::Layout::CrossAxisAlignment::Center)
				.setSpacing(0.0f)
				.setPadding(0.0f)
				.setFixedWidth(DayCellSize)
				.setFixedHeight(DayCellSize);
		}

		/**
		 * @brief Default destructor.
		 */
		~DayCell(void) override = default;

		/**
		 * @brief Build the day-number label child.
		 */
		void initialize(void) override
		{
			if (_day <= 0) {
				return;
			}

			auto onSurface = schemeColor(SchemeColorRole::OnSurface);

			_label =
				buildText(getComponentRegistry(), *this, shared_from_this(),
						  std::to_string(_day), 14.0f, onSurface);

			update();
		}

		/**
		 * @brief Position the day-number label inside the cell.
		 */
		void update(void) override
		{
			std::vector<ecs::Entity::Identifier> childIdentifiers;

			if (_label != nullptr) {
				childIdentifiers.push_back(_label->getIdentifier());
			}

			systems::Layout::apply(getComponentRegistry(), getIdentifier(),
								   childIdentifiers,
								   static_cast<std::uint32_t>(getLayer()));
		}

		/**
		 * @brief Get the represented day number.
		 * @return The day number.
		 */
		int getDay(void) const
		{
			return _day;
		}

		/**
		 * @brief Set the cell selected state.
		 * @param selected The new selected state.
		 */
		void setSelected(bool selected)
		{
			getComponentRegistry()
				.getComponent<components::Selectable>(getIdentifier())
				.setSelected(selected);
		}

		/**
		 * @brief Whether the cell is selected.
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
	};

	DatePicker::Builder::Builder(ecs::ComponentRegistry &componentRegistry,
								 ecs::EntityRegistry &entityRegistry)
		: EntityBuilderBase<DatePicker>(componentRegistry, entityRegistry)
	{
		reset();
	}

	std::shared_ptr<DatePicker> DatePicker::Builder::buildEntity(void)
	{
		auto entity = std::make_shared<DatePicker>(this->getComponentRegistry(),
												   _config, _variant, _year,
												   _month, _onSelect, _onRange);
		return entity;
	}

	void DatePicker::Builder::reset(void)
	{
		_config	  = SurfaceConfig();
		_variant  = Variant::SingleDate;
		_year	  = 2026;
		_month	  = 1;
		_onSelect = nullptr;
		_onRange  = nullptr;

		_config.axis = components::Layout::Axis::Vertical;
		_config.mainAxisAlignment =
			components::Layout::MainAxisAlignment::Center;
		_config.crossAxisAlignment =
			components::Layout::CrossAxisAlignment::Center;
		_config.spacing		 = 8.0f;
		_config.padding		 = 24.0f;
		_config.borderRadius = 28.0f;
		_config.setFixedWidth(328.0f);
	}

	DatePicker::Builder &
		DatePicker::Builder::withPose(const utility::graphic::PoseF &pose)
	{
		_config.pose = pose;
		return *this;
	}

	DatePicker::Builder &DatePicker::Builder::withVariant(Variant variant)
	{
		_variant = variant;
		return *this;
	}

	DatePicker::Builder &DatePicker::Builder::withYear(int year)
	{
		_year = year;
		return *this;
	}

	DatePicker::Builder &DatePicker::Builder::withMonth(int month)
	{
		_month = month;
		return *this;
	}

	DatePicker::Builder &DatePicker::Builder::withOnSelect(
		const std::function<void(int)> &onSelect)
	{
		_onSelect = onSelect;
		return *this;
	}

	DatePicker::Builder &DatePicker::Builder::withOnRange(
		const std::function<void(int, int)> &onRange)
	{
		_onRange = onRange;
		return *this;
	}

	std::shared_ptr<DatePicker> DatePicker::Director::makeDatePicker(
		Builder &builder, std::shared_ptr<ecs::Entity> parent, Variant variant,
		int year, int month)
	{
		return builder.withVariant(variant)
			.withYear(year)
			.withMonth(month)
			.registerEntity(parent);
	}

	DatePicker::DatePicker(ecs::ComponentRegistry &registry,
						   const SurfaceConfig &config, Variant variant,
						   int year, int month,
						   const std::function<void(int)> &onSelect,
						   const std::function<void(int, int)> &onRange)
		: SurfaceBase<components::Overlay, components::SelectionGroup>(registry,
																	   config)
		, _variant(variant)
		, _start(Date { year, month, 1 })
		, _end(Date { year, month, 1 })
		, _year(year)
		, _month(month)
		, _onSelect(onSelect)
		, _onRange(onRange)
		, _dayCells()
		, _header()
		, _rows()
		, _built(false)
		, _rangeStarted(false)
	{
		getComponentRegistry()
			.getComponent<components::SelectionGroup>(getIdentifier())
			.setAllowEmpty(true);

		applyVariant();
		applyOverlay();
	}

	DatePicker::~DatePicker(void)
	{
	}

	void DatePicker::applyVariant(void)
	{
		setSurfaceConfig(SurfaceConfig::panel(
			328.0f, components::Layout::Axis::Vertical, 8.0f));
		setColor(schemeColor(SchemeColorRole::SurfaceContainerHigh));
	}

	void DatePicker::applyOverlay(void)
	{
		auto &overlay =
			this->getComponentRegistry().getComponent<components::Overlay>(
				this->getIdentifier());

		overlay.setModal(true);
		overlay.setDismissOnOutsideClick(true);
		overlay.setDismissOnEscape(true);
	}

	void DatePicker::refreshHeader(void)
	{
		if (_header == nullptr) {
			return;
		}

		char buffer[16];
		std::snprintf(buffer, sizeof(buffer), "%04d-%02d", _year, _month);
		_header->setContent(buffer);
	}

	void DatePicker::buildCalendar(void)
	{
		if (_built) {
			return;
		}

		this->accessDirectEntities().clear();
		_dayCells.clear();
		_rows.clear();

		const auto onSurface = schemeColor(SchemeColorRole::OnSurface);

		std::vector<std::shared_ptr<ecs::Entity>> children;

		char headerBuffer[16];
		std::snprintf(headerBuffer, sizeof(headerBuffer), "%04d-%02d", _year,
					  _month);

		_header =
			buildText(this->getComponentRegistry(), *this,
					  this->shared_from_this(), headerBuffer, 16.0f, onSurface);

		children.push_back(_header);

		const int offset = firstWeekday(_year, _month);
		const int total	 = daysInMonth(_year, _month);
		int index		 = 0;

		for (int row = 0; row < WeekRows; ++row) {
			std::vector<std::shared_ptr<ecs::Entity>> rowCells;
			rowCells.reserve(DaysPerWeek);

			for (int column = 0; column < DaysPerWeek; ++column) {
				const int dayNumber = index - offset + 1;
				const bool valid	= dayNumber >= 1 && dayNumber <= total;

				auto cell = std::make_shared<DayCell>(
					this->getComponentRegistry(), valid ? dayNumber : 0);

				if (valid && dayNumber == _start.day) {
					cell->setSelected(true);
				}

				DayCell *cellPtr = cell.get();

				cell->setOnSelectionChanged(
					[this, cellPtr, dayNumber](bool selected) {
						if (!selected || dayNumber <= 0) {
							return;
						}

						if (this->_variant == Variant::SingleDate) {
							this->_start.day = dayNumber;
							this->selectCell(cellPtr->getIdentifier());

							if (this->_onSelect) {
								this->_onSelect(dayNumber);
							}
						} else {
							if (!this->_rangeStarted) {
								this->_start.day	= dayNumber;
								this->_rangeStarted = true;
							} else {
								this->_end.day		= dayNumber;
								this->_rangeStarted = false;
								if (this->_onRange) {
									this->_onRange(this->_start.day,
												   this->_end.day);
								}
							}
						}
					});

				rowCells.push_back(cell);
				_dayCells.push_back(cell);
				++index;
			}

			Layout::Builder rowBuilder(this->getComponentRegistry(), *this);
			rowBuilder.withAxis(components::Layout::Axis::Horizontal)
				.withSpacing(4.0f)
				.withPadding(0.0f)
				.withFixedHeight(DayCellSize)
				.withColor(transparentColor())
				.withBorderRadius(0.0f);

			auto rowEntity = rowBuilder.withEntities(rowCells).registerEntity(
				this->shared_from_this());

			for (const auto &rowCell: rowCells) {
				rowEntity->addEntity(rowCell);
			}

			_rows.push_back(rowEntity);
			children.push_back(rowEntity);
		}

		setChildren(children);
		refreshHeader();

		_built = true;
	}

	DatePicker &DatePicker::setVariant(Variant variant)
	{
		_variant = variant;
		_built	 = false;
		buildCalendar();
		return *this;
	}

	DatePicker::Variant DatePicker::getVariant(void) const
	{
		return _variant;
	}

	DatePicker &DatePicker::setYear(int year)
	{
		_year  = year;
		_built = false;
		buildCalendar();
		return *this;
	}

	int DatePicker::getYear(void) const
	{
		return _year;
	}

	DatePicker &DatePicker::setMonth(int month)
	{
		_month = month;
		_built = false;
		buildCalendar();
		return *this;
	}

	int DatePicker::getMonth(void) const
	{
		return _month;
	}

	int DatePicker::getSelectedDay(void) const
	{
		return _start.day;
	}

	int DatePicker::getSelectedStartDay(void) const
	{
		return _start.day;
	}

	int DatePicker::getSelectedEndDay(void) const
	{
		return _end.day;
	}

	std::size_t DatePicker::getDayCellCount(void) const
	{
		return _dayCells.size();
	}

	ecs::Entity::Identifier
		DatePicker::getDayCellIdentifier(std::size_t index) const
	{
		if (index >= _dayCells.size() || _dayCells[index] == nullptr) {
			return ecs::Entity::InvalidIdentifier;
		}

		return _dayCells[index]->getIdentifier();
	}

	DatePicker &DatePicker::select(int day)
	{
		for (auto &entity: _dayCells) {
			auto cell = std::dynamic_pointer_cast<DayCell>(entity);
			if (cell != nullptr && cell->getDay() == day) {
				cell->setSelected(true);
				break;
			}
		}

		return *this;
	}

	void DatePicker::selectCell(ecs::Entity::Identifier identifier)
	{
		std::vector<ecs::Entity::Identifier> childIdentifiers;
		childIdentifiers.reserve(_dayCells.size());

		for (const auto &cell: _dayCells) {
			if (cell != nullptr) {
				childIdentifiers.push_back(cell->getIdentifier());
			}
		}

		systems::Selection::select(getComponentRegistry(), getIdentifier(),
								   childIdentifiers, identifier);
	}

	DatePicker &DatePicker::show(void)
	{
		setOverlayVisible(this->getComponentRegistry(), this->getIdentifier(),
						  true);
		return *this;
	}

	DatePicker &DatePicker::hide(void)
	{
		setOverlayVisible(this->getComponentRegistry(), this->getIdentifier(),
						  false);
		return *this;
	}

	bool DatePicker::isVisible(void) const
	{
		return isOverlayVisible(this->getComponentRegistry(),
								this->getIdentifier());
	}

	DatePicker &DatePicker::open(void)
	{
		return show();
	}

	DatePicker &DatePicker::close(void)
	{
		return hide();
	}

	bool DatePicker::isOpen(void) const
	{
		return isVisible();
	}

	void DatePicker::initialize(void)
	{
		SurfaceBase::initialize();
		applyVariant();
		applyOverlay();
		buildCalendar();
	}

	void DatePicker::update(void)
	{
		SurfaceBase::update();
		applyOverlay();
		buildCalendar();
		refreshHeader();
	}

}	 // namespace guillaume::entities
