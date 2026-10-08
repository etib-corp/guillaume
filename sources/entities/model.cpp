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

#include "guillaume/entities/model.hpp"
#include "guillaume/entities/builder_base.hpp"

namespace guillaume::entities
{

	Model::Model::Builder::Builder(ecs::ComponentRegistry &componentRegistry,
								   ecs::EntityRegistry &entityRegistry)
		: EntityBuilderBase<Model>(componentRegistry, entityRegistry)
	{
		reset();
	}

	std::shared_ptr<Model> Model::Builder::buildEntity(void)
	{
		auto entity =
			std::make_shared<Model>(this->getComponentRegistry(), _modelPath);
		entity->setModelPath(_modelPath);
		entity->setTexturePath(_texturePath);
		return entity;
	}

	void Model::Builder::reset(void)
	{
		_modelPath.clear();
		_texturePath.clear();
	}

	Model::Builder &Model::Builder::withModelPath(const std::string &modelPath)
	{
		_modelPath = modelPath;
		return *this;
	}

	Model::Builder &
		Model::Builder::withTexturePath(const std::string &texturePath)
	{
		_texturePath = texturePath;
		return *this;
	}

	std::shared_ptr<Model> Model::Director::makeModel(
		Builder &builder, std::shared_ptr<Entity> parent,
		const std::string &modelPath, const std::string &texturePath)
	{
		return builder.withModelPath(modelPath)
			.withTexturePath(texturePath)
			.registerEntity(parent);
	}

	Model::Model(ecs::ComponentRegistry &registry, const std::string &modelPath)
		: ecs::EntityFiller<components::Transform, components::Bound,
							components::Model>(registry)
		, _modelPath(modelPath)
	{
	}

	Model::~Model()
	{
	}

	Model &Model::setModelPath(const std::string &modelPath)
	{
		_modelPath = modelPath;

		getComponentRegistry()
			.getComponent<components::Model>(getIdentifier())
			.setModelPath(modelPath);

		return *this;
	}

	Model &Model::setTexturePath(const std::string &texturePath)
	{
		_texturePath = texturePath;

		getComponentRegistry()
			.getComponent<components::Model>(getIdentifier())
			.setTexturePath(texturePath);

		return *this;
	}

	void Model::initialize(void)
	{
	}

	void Model::update(void)
	{
		setModelPath(_modelPath);
		setTexturePath(_texturePath);
	}

}	 // namespace guillaume::entities