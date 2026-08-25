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

#include "guillaume/entities/model.hpp"

namespace guillaume::entities
{

	Model::Model::Builder::Builder(ecs::ComponentRegistry &componentRegistry,
								   ecs::EntityRegistry &entityRegistry)
		: ecs::EntityBuilder(componentRegistry, entityRegistry)
	{
		reset();
	}

	Model::Builder::~Builder(void)
	{
	}

	std::shared_ptr<Model>
		Model::Builder::registerEntity(std::shared_ptr<Entity> parent)
	{
		_model =
			std::make_shared<Model>(this->getComponentRegistry(), _modelPath);
		_model->setModelPath(_modelPath);
		_model->setTexturePath(_texturePath);
		_model->setParent(parent);

		this->getEntityRegistry().addEntity(_model);

		auto modelCopy =
			_model;	   // Create a copy of the shared pointer to return

		reset();

		return modelCopy;
	}

	void Model::Builder::reset(void)
	{
		_model.reset();
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

	Model::Director::Director(void)
		: ecs::EntityDirector()
	{
	}

	Model::Director::~Director(void)
	{
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