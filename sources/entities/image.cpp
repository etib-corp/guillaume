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
#include <stdexcept>

#include "guillaume/entities/image.hpp"
#include "guillaume/entities/builder_base.hpp"

namespace guillaume::entities
{

	Image::Image::Builder::Builder(ecs::ComponentRegistry &componentRegistry,
								   ecs::EntityRegistry &entityRegistry)
		: EntityBuilderBase<Image>(componentRegistry, entityRegistry)
	{
		reset();
	}

	std::shared_ptr<Image> Image::Builder::buildEntity(void)
	{
		auto entity =
			std::make_shared<Image>(this->getComponentRegistry(), _texturePath);
		entity->setTexturePath(_texturePath);
		entity->setFallbackPath(_fallbackPath);
		if (_width > 0.0f && _height > 0.0f) {
			entity->setSize(_width, _height);
		}
		return entity;
	}

	void Image::Builder::reset(void)
	{
		_texturePath.clear();
		_fallbackPath.clear();
		_width	= 0.0f;
		_height = 0.0f;
	}

	Image::Builder &
		Image::Builder::withFallbackPath(const std::string &fallbackPath)
	{
		_fallbackPath = fallbackPath;
		return *this;
	}

	Image::Builder &Image::Builder::withSize(float width, float height)
	{
		_width	= width;
		_height = height;
		return *this;
	}

	Image::Builder &
		Image::Builder::withTexturePath(const std::string &texturePath)
	{
		_texturePath = texturePath;
		return *this;
	}

	std::shared_ptr<Image>
		Image::Director::makeImage(Builder &builder,
								   std::shared_ptr<Entity> parent,
								   const std::string &texturePath)
	{
		return builder.withTexturePath(texturePath).registerEntity(parent);
	}

	Image::Image(ecs::ComponentRegistry &registry,
				 const std::string &texturePath)
		: ecs::EntityFiller<components::Transform, components::Bound,
							components::Image, components::Accessibility>(
			  registry)
		, _texturePath(texturePath)
	{
		getComponentRegistry()
			.getComponent<components::Accessibility>(getIdentifier())
			.setRole(components::AccessibilityRole::Image);
	}

	const std::string &Image::getTexturePath(void) const
	{
		return _texturePath;
	}

	Image &Image::setFallbackPath(const std::string &fallbackPath)
	{
		_fallbackPath = fallbackPath;
		getComponentRegistry()
			.getComponent<components::Image>(getIdentifier())
			.setFallbackPath(fallbackPath);
		return *this;
	}

	Image &Image::setSize(float width, float height)
	{
		if (width <= 0.0f || height <= 0.0f) {
			throw std::invalid_argument("Image size must be positive");
		}
		_width	= width;
		_height = height;
		applySize();
		return *this;
	}

	Image &Image::setAspectRatio(float width, float aspectRatio)
	{
		if (width <= 0.0f || aspectRatio <= 0.0f) {
			throw std::invalid_argument(
				"Image width and aspect ratio must be positive");
		}
		return setSize(width, width / aspectRatio);
	}

	void Image::applySize(void)
	{
		if (_width <= 0.0f || _height <= 0.0f) {
			return;
		}
		getComponentRegistry()
			.getComponent<components::Bound>(getIdentifier())
			.setWidth(_width)
			.setHeight(_height);
	}

	components::Image::LoadState Image::getLoadState(void) const
	{
		return getComponentRegistry()
			.getComponent<components::Image>(getIdentifier())
			.getLoadState();
	}

	Image &Image::setAccessibilityLabel(const std::string &label)
	{
		getComponentRegistry()
			.getComponent<components::Accessibility>(getIdentifier())
			.setLabel(label);
		return *this;
	}

	Image::~Image()
	{
	}

	Image &Image::setTexturePath(const std::string &texturePath)
	{
		_texturePath = texturePath;

		getComponentRegistry()
			.getComponent<components::Image>(getIdentifier())
			.setTexturePath(texturePath);

		return *this;
	}

	void Image::initialize(void)
	{
	}

	void Image::update(void)
	{
		setTexturePath(_texturePath);
		applySize();
	}

}	 // namespace guillaume::entities
