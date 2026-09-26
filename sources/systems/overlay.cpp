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

#include "guillaume/systems/overlay.hpp"

#include <algorithm>

namespace guillaume::systems
{
	Overlay::Overlay(event::EventBus &eventBus,
					 std::unique_ptr<utility::Engine> &engine)
		: ecs::SystemFiller<components::Overlay, components::Transform,
							components::Bound>(ecs::Phase::Event)
		, event::EventManager<utility::event::KeyboardEvent>(eventBus)
		, _engine(engine)
		, _stack()
		, _lastTime(std::chrono::steady_clock::now())
	{
	}

	void Overlay::prepare(void)
	{
		consumeNextEvent();

		const auto now = std::chrono::steady_clock::now();
		const std::chrono::duration<float> elapsed = now - _lastTime;
		_lastTime								   = now;
		advance(elapsed.count());

		const auto key = getLastEvent();
		if (key && key->getIsDownEvent()
			&& key->getScancode()
				== utility::event::KeyboardEvent::ScanCode::Escape) {
			handleEscape();
		}
	}

	void Overlay::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		if (!requireComponent<components::Overlay>(entityIdentifier)) {
			return;
		}

		auto &overlay = getComponent<components::Overlay>(entityIdentifier);

		if (overlay.isVisible() && !isOpen(entityIdentifier)) {
			_stack.push_back(entityIdentifier);
			_engine->setShouldCaptureViewportInput(false);
		} else if (!overlay.isVisible() && isOpen(entityIdentifier)) {
			dismiss(entityIdentifier);
		}
	}

	void Overlay::show(const ecs::Entity::Identifier &entityIdentifier)
	{
		if (isOpen(entityIdentifier)) {
			return;
		}

		_stack.push_back(entityIdentifier);

		if (hasComponent<components::Overlay>(entityIdentifier)) {
			getComponent<components::Overlay>(entityIdentifier)
				.setVisible(true);
		}

		_engine->setShouldCaptureViewportInput(false);
	}

	void Overlay::dismiss(void)
	{
		if (_stack.empty()) {
			return;
		}

		const auto top = _stack.back();
		_stack.pop_back();

		if (hasComponent<components::Overlay>(top)) {
			auto &overlay = getComponent<components::Overlay>(top);
			overlay.setVisible(false);
			overlay.setElapsed(0.0f);
			if (const auto handler = overlay.getOnDismissHandler()) {
				handler();
			}
		}

		if (_stack.empty()) {
			_engine->setShouldCaptureViewportInput(true);
		}
	}

	void Overlay::dismiss(const ecs::Entity::Identifier &entityIdentifier)
	{
		const auto iterator =
			std::find(_stack.begin(), _stack.end(), entityIdentifier);
		if (iterator == _stack.end()) {
			return;
		}
		_stack.erase(iterator);

		if (hasComponent<components::Overlay>(entityIdentifier)) {
			auto &overlay = getComponent<components::Overlay>(entityIdentifier);
			overlay.setVisible(false);
			overlay.setElapsed(0.0f);
			if (const auto handler = overlay.getOnDismissHandler()) {
				handler();
			}
		}

		if (_stack.empty()) {
			_engine->setShouldCaptureViewportInput(true);
		}
	}

	bool Overlay::isOpen(const ecs::Entity::Identifier &entityIdentifier) const
	{
		return std::find(_stack.begin(), _stack.end(), entityIdentifier)
			!= _stack.end();
	}

	ecs::Entity::Identifier Overlay::getTop(void) const
	{
		return _stack.empty() ? ecs::Entity::InvalidIdentifier : _stack.back();
	}

	std::size_t Overlay::getDepth(void) const
	{
		return _stack.size();
	}

	void Overlay::handleEscape(void)
	{
		if (_stack.empty()) {
			return;
		}

		const auto top = _stack.back();
		if (hasComponent<components::Overlay>(top)
			&& getComponent<components::Overlay>(top).dismissesOnEscape()) {
			dismiss();
		}
	}

	void Overlay::handleOutsideClick(const utility::math::Vector2F &point)
	{
		if (_stack.empty()) {
			return;
		}

		const auto top = _stack.back();
		if (!hasComponent<components::Overlay>(top)
			|| !hasComponent<components::Transform>(top)
			|| !hasComponent<components::Bound>(top)) {
			return;
		}

		auto &overlay = getComponent<components::Overlay>(top);
		if (!overlay.dismissesOnOutsideClick()) {
			return;
		}

		const auto pose	  = getComponent<components::Transform>(top).getPose();
		const auto &bound = getComponent<components::Bound>(top);

		const float x	   = pose.getPosition().getX();
		const float y	   = pose.getPosition().getY();
		const float width  = bound.getWidth();
		const float height = bound.getHeight();

		const bool inside = point.x >= x && point.x <= x + width && point.y >= y
			&& point.y <= y + height;

		if (!inside) {
			dismiss();
		}
	}

	void Overlay::advance(float deltaTime)
	{
		if (_stack.empty()) {
			return;
		}

		const auto top = _stack.back();
		if (!hasComponent<components::Overlay>(top)) {
			return;
		}

		auto &overlay = getComponent<components::Overlay>(top);
		if (!overlay.hasAutoDismiss()) {
			return;
		}

		overlay.setElapsed(overlay.getElapsed() + deltaTime);
		if (overlay.getElapsed() >= overlay.getAutoDismiss()) {
			dismiss();
		}
	}

}	 // namespace guillaume::systems
