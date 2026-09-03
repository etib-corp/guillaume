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

#include "guillaume/entities/image.hpp"

namespace guillaume::entities
{

	Image::Image::Builder::Builder(ecs::ComponentRegistry &componentRegistry,
								   ecs::EntityRegistry &entityRegistry)
		: ecs::EntityBuilder(componentRegistry, entityRegistry)
	{
		reset();
	}

	Image::Builder::~Builder(void)
	{
	}

	std::shared_ptr<Image>
		Image::Builder::registerEntity(std::shared_ptr<Entity> parent)
	{
		_image =
			std::make_shared<Image>(this->getComponentRegistry(), _texturePath);
		_image->setTexturePath(_texturePath);
		_image->setParent(parent);

		this->getEntityRegistry().addEntity(_image);

		auto imageCopy =
			_image;	   // Create a copy of the shared pointer to return

		reset();

		return imageCopy;
	}

	void Image::Builder::reset(void)
	{
		_image.reset();
		_texturePath.clear();
	}

	Image::Builder &
		Image::Builder::withTexturePath(const std::string &texturePath)
	{
		_texturePath = texturePath;
		return *this;
	}

	Image::Director::Director(void)
		: ecs::EntityDirector()
	{
	}

	Image::Director::~Director(void)
	{
	}

	std::shared_ptr<Image> Image::Director::makeImage(
		Builder &builder, std::shared_ptr<Entity> parent,
		const std::string &texturePath)
	{
		return builder.withTexturePath(texturePath).registerEntity(parent);
	}

	Image::Image(ecs::ComponentRegistry &registry,
				 const std::string &texturePath)
		: ecs::EntityFiller<components::Transform, components::Bound,
							components::Image>(registry)
		, _texturePath(texturePath)
	{
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
	}

}	 // namespace guillaume::entities
