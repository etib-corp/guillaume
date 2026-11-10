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

#include "guillaume/entities/mesh.hpp"
#include "guillaume/entities/builder_base.hpp"

namespace guillaume::entities
{

	Mesh::Mesh::Builder::Builder(ecs::ComponentRegistry &componentRegistry,
								 ecs::EntityRegistry &entityRegistry)
		: EntityBuilderBase<Mesh>(componentRegistry, entityRegistry)
	{
		reset();
	}

	std::shared_ptr<Mesh> Mesh::Builder::buildEntity(void)
	{
		auto entity =
			std::make_shared<Mesh>(this->getComponentRegistry(), _meshPath);
		entity->setMeshPath(_meshPath);
		entity->setTexturePath(_texturePath);
		return entity;
	}

	void Mesh::Builder::reset(void)
	{
		_meshPath.clear();
		_texturePath.clear();
	}

	Mesh::Builder &Mesh::Builder::withMeshPath(const std::string &meshPath)
	{
		_meshPath = meshPath;
		return *this;
	}

	Mesh::Builder &
		Mesh::Builder::withTexturePath(const std::string &texturePath)
	{
		_texturePath = texturePath;
		return *this;
	}

	std::shared_ptr<Mesh>
		Mesh::Builder::makeMesh(std::shared_ptr<Entity> parent,
								const std::string &meshPath,
								const std::string &texturePath)
	{
		return withMeshPath(meshPath)
			.withTexturePath(texturePath)
			.registerEntity(parent);
	}

	Mesh::Mesh(ecs::ComponentRegistry &registry, const std::string &meshPath)
		: ecs::EntityFiller<components::Transform, components::Bound,
							components::Mesh>(registry)
		, _meshPath(meshPath)
	{
	}

	Mesh::~Mesh()
	{
	}

	Mesh &Mesh::setMeshPath(const std::string &meshPath)
	{
		_meshPath = meshPath;

		getComponentRegistry()
			.getComponent<components::Mesh>(getIdentifier())
			.setMeshPath(meshPath);

		return *this;
	}

	Mesh &Mesh::setTexturePath(const std::string &texturePath)
	{
		_texturePath = texturePath;

		getComponentRegistry()
			.getComponent<components::Mesh>(getIdentifier())
			.setTexturePath(texturePath);

		return *this;
	}

	void Mesh::initialize(void)
	{
	}

	void Mesh::update(void)
	{
		setMeshPath(_meshPath);
		setTexturePath(_texturePath);
	}

}	 // namespace guillaume::entities