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

#include "guillaume/entities/time_picker.hpp"
#include "guillaume/entities/overlay_helpers.hpp"

#include <cmath>
#include <cstddef>
#include <cstdio>
#include <memory>
#include <string>
#include <vector>

#include <utility/graphic/pose.hpp>

#include "guillaume/components/bound.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/transform.hpp"

#include "guillaume/entities/content_helpers.hpp"
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
		constexpr float ClockRadius	  = 100.0f;
		constexpr int MarkersPerClock = 12;
		constexpr int MinutesPerHour  = 60;
		constexpr int MinutesPerDay	  = 24 * 60;
		constexpr float Pi			  = 3.14159265358979323846f;
	}	 // namespace

	TimePicker::Builder::Builder(ecs::ComponentRegistry &componentRegistry,
								 ecs::EntityRegistry &entityRegistry)
		: EntityBuilderBase<TimePicker>(componentRegistry, entityRegistry)
	{
		reset();
	}

	std::shared_ptr<TimePicker> TimePicker::Builder::buildEntity(void)
	{
		auto entity =
			std::make_shared<TimePicker>(this->getComponentRegistry(), _config,
										 _variant, _hour, _minute, _onChange);
		return entity;
	}

	void TimePicker::Builder::reset(void)
	{
		_config	  = SurfaceConfig();
		_variant  = Variant::Input;
		_hour	  = 0;
		_minute	  = 0;
		_onChange = nullptr;

		_config.axis = components::Layout::Axis::Horizontal;
		_config.mainAxisAlignment =
			components::Layout::MainAxisAlignment::Center;
		_config.crossAxisAlignment =
			components::Layout::CrossAxisAlignment::Center;
		_config.spacing		 = 8.0f;
		_config.padding		 = 24.0f;
		_config.borderRadius = 28.0f;
		_config.setFixedWidth(328.0f);
	}

	TimePicker::Builder &
		TimePicker::Builder::withPose(const utility::graphic::PoseF &pose)
	{
		_config.pose = pose;
		return *this;
	}

	TimePicker::Builder &TimePicker::Builder::withVariant(Variant variant)
	{
		_variant = variant;
		return *this;
	}

	TimePicker::Builder &TimePicker::Builder::withHour(int hour)
	{
		_hour = hour;
		return *this;
	}

	TimePicker::Builder &TimePicker::Builder::withMinute(int minute)
	{
		_minute = minute;
		return *this;
	}

	TimePicker::Builder &TimePicker::Builder::withOnChange(
		const std::function<void(int, int)> &onChange)
	{
		_onChange = onChange;
		return *this;
	}

	std::shared_ptr<TimePicker> TimePicker::Director::makeTimePicker(
		Builder &builder, std::shared_ptr<ecs::Entity> parent, Variant variant,
		int hour, int minute)
	{
		return builder.withVariant(variant)
			.withHour(hour)
			.withMinute(minute)
			.registerEntity(parent);
	}

	TimePicker::TimePicker(ecs::ComponentRegistry &registry,
						   const SurfaceConfig &config, Variant variant,
						   int hour, int minute,
						   const std::function<void(int, int)> &onChange)
		: SurfaceBase<components::Overlay, components::Value>(registry, config)
		, _variant(variant)
		, _hour(hour)
		, _minute(minute)
		, _onChange(onChange)
		, _markers()
		, _hourText()
		, _minuteText()
		, _colonText()
		, _built(false)
		, _updating(false)
	{
		applyVariant();
		applyOverlay();
		applyValue();
	}

	TimePicker::~TimePicker(void)
	{
	}

	void TimePicker::applyVariant(void)
	{
		setSurfaceConfig(SurfaceConfig::panel(
			328.0f, components::Layout::Axis::Horizontal, 8.0f));
		setColor(schemeColor(SchemeColorRole::SurfaceContainerHigh));
	}

	void TimePicker::applyOverlay(void)
	{
		auto &overlay =
			this->getComponentRegistry().getComponent<components::Overlay>(
				this->getIdentifier());

		overlay.setModal(true);
		overlay.setDismissOnOutsideClick(true);
		overlay.setDismissOnEscape(true);
	}

	void TimePicker::applyValue(void)
	{
		auto &value =
			this->getComponentRegistry().getComponent<components::Value>(
				this->getIdentifier());

		value.setMin(0.0f)
			.setMax(static_cast<float>(MinutesPerDay - 1))
			.setStep(1.0f);

		value.setOnChangedHandler([this](float raw) {
			if (this->_updating) {
				return;
			}

			int total = static_cast<int>(std::round(raw));
			if (total < 0) {
				total = 0;
			}
			if (total >= MinutesPerDay) {
				total = MinutesPerDay - 1;
			}

			this->_hour	  = total / MinutesPerHour;
			this->_minute = total % MinutesPerHour;

			this->refreshTexts();

			if (this->_onChange) {
				this->_onChange(this->_hour, this->_minute);
			}
		});

		_updating = true;
		value.setValue(static_cast<float>(_hour * MinutesPerHour + _minute));
		_updating = false;
	}

	void TimePicker::refreshTexts(void)
	{
		if (_hourText != nullptr) {
			char buffer[8];
			std::snprintf(buffer, sizeof(buffer), "%02d", _hour);
			_hourText->setContent(buffer);
		}

		if (_minuteText != nullptr) {
			char buffer[8];
			std::snprintf(buffer, sizeof(buffer), "%02d", _minute);
			_minuteText->setContent(buffer);
		}
	}

	void TimePicker::buildContent(void)
	{
		if (_built) {
			return;
		}

		this->accessDirectEntities().clear();
		_markers.clear();
		_hourText  = nullptr;
		_colonText = nullptr;

		const auto onSurface = schemeColor(SchemeColorRole::OnSurface);

		std::vector<std::shared_ptr<ecs::Entity>> children;

		if (_variant == Variant::Clock) {
			for (int index = 0; index < MarkersPerClock; ++index) {
				const int hourLabel = index == 0 ? 12 : index;

				auto marker =
					buildText(this->getComponentRegistry(), *this,
							  this->shared_from_this(),
							  std::to_string(hourLabel), 18.0f, onSurface);

				if (marker == nullptr) {
					continue;
				}

				_markers.push_back(marker);
				children.push_back(marker);
			}
		} else {
			_hourText =
				buildText(this->getComponentRegistry(), *this,
						  this->shared_from_this(), "00", 45.0f, onSurface);

			_colonText =
				buildText(this->getComponentRegistry(), *this,
						  this->shared_from_this(), ":", 45.0f, onSurface);

			_minuteText =
				buildText(this->getComponentRegistry(), *this,
						  this->shared_from_this(), "00", 45.0f, onSurface);

			if (_hourText != nullptr) {
				children.push_back(_hourText);
			}
			if (_colonText != nullptr) {
				children.push_back(_colonText);
			}
			if (_minuteText != nullptr) {
				children.push_back(_minuteText);
			}

			refreshTexts();
		}

		setChildren(children);

		if (_variant == Variant::Clock) {
			applyMarkerPlacement();
		}

		_built = true;
	}

	void TimePicker::applyMarkerPlacement(void)
	{
		for (std::size_t index = 0; index < _markers.size(); ++index) {
			if (_markers[index] == nullptr) {
				continue;
			}

			const float angle =
				90.0f - static_cast<float>(index) * (360.0f / 12.0f);
			const float radians = angle * Pi / 180.0f;

			const float x = ClockRadius + ClockRadius * std::cos(radians);
			const float y = ClockRadius + ClockRadius * std::sin(radians);

			placeChild(this->getComponentRegistry(), this->getIdentifier(),
					   _markers[index]->getIdentifier(),
					   utility::graphic::PositionF(x, y, 0.0f));
		}
	}

	TimePicker &TimePicker::setVariant(Variant variant)
	{
		_variant = variant;
		_built	 = false;
		buildContent();
		return *this;
	}

	TimePicker::Variant TimePicker::getVariant(void) const
	{
		return _variant;
	}

	TimePicker &TimePicker::setTime(int hour, int minute)
	{
		if (hour < 0) {
			hour = 0;
		}
		if (hour > 23) {
			hour = 23;
		}
		if (minute < 0) {
			minute = 0;
		}
		if (minute > 59) {
			minute = 59;
		}

		_hour	= hour;
		_minute = minute;

		_updating = false;
		this->getComponentRegistry()
			.getComponent<components::Value>(this->getIdentifier())
			.setValue(static_cast<float>(_hour * MinutesPerHour + _minute));

		refreshTexts();

		return *this;
	}

	int TimePicker::getHour(void) const
	{
		return _hour;
	}

	TimePicker &TimePicker::setHour(int hour)
	{
		return setTime(hour, _minute);
	}

	int TimePicker::getMinute(void) const
	{
		return _minute;
	}

	TimePicker &TimePicker::setMinute(int minute)
	{
		return setTime(_hour, minute);
	}

	int TimePicker::getValue(void) const
	{
		return _hour * MinutesPerHour + _minute;
	}

	TimePicker &TimePicker::show(void)
	{
		setOverlayVisible(this->getComponentRegistry(), this->getIdentifier(),
						  true);
		return *this;
	}

	TimePicker &TimePicker::hide(void)
	{
		setOverlayVisible(this->getComponentRegistry(), this->getIdentifier(),
						  false);
		return *this;
	}

	bool TimePicker::isVisible(void) const
	{
		return isOverlayVisible(this->getComponentRegistry(),
								this->getIdentifier());
	}

	TimePicker &TimePicker::open(void)
	{
		return show();
	}

	TimePicker &TimePicker::close(void)
	{
		return hide();
	}

	bool TimePicker::isOpen(void) const
	{
		return isVisible();
	}

	std::size_t TimePicker::getMarkerCount(void) const
	{
		return _markers.size();
	}

	void TimePicker::initialize(void)
	{
		SurfaceBase::initialize();
		applyVariant();
		applyOverlay();
		applyValue();
		buildContent();
	}

	void TimePicker::update(void)
	{
		SurfaceBase::update();
		applyOverlay();
		applyValue();
		buildContent();
		refreshTexts();

		if (_variant == Variant::Clock) {
			applyMarkerPlacement();
		}
	}

}	 // namespace guillaume::entities
