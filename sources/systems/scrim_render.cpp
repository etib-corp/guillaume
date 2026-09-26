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

#include "guillaume/systems/scrim_render.hpp"

#include <memory>
#include <vector>

#include "guillaume/mesh_renderable.hpp"

namespace guillaume::systems
{
	ScrimRender::ScrimRender(std::unique_ptr<utility::Engine> &engine)
		: ecs::SystemFiller<components::Transform, components::Bound,
							components::Scrim>(ecs::Phase::Render)
		, _engine(engine)
	{
	}

	void ScrimRender::prepare(void)
	{
		markAllUnused();
	}

	void ScrimRender::cleanup(void)
	{
		removeUnused(*_engine);
	}

	void
		ScrimRender::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		if (!requireComponent<components::Bound>(entityIdentifier)
			|| !requireComponent<components::Transform>(entityIdentifier)
			|| !requireComponent<components::Scrim>(entityIdentifier)) {
			return;
		}

		const auto &pose =
			getComponent<components::Transform>(entityIdentifier).getPose();
		const auto &bound = getComponent<components::Bound>(entityIdentifier);
		const auto &color =
			getComponent<components::Scrim>(entityIdentifier).getColor();

		const float x0 = pose.getPosition().x;
		const float y0 = pose.getPosition().y;
		const float z  = pose.getPosition().z;
		const float x1 = x0 + bound.getWidth();
		const float y1 = y0 + bound.getHeight();

		utility::graphic::Mesh mesh(std::vector<utility::graphic::VertexF> {},
									std::vector<uint32_t> {});

		auto addVertex = [&mesh, &color](float x, float y, float z) {
			utility::graphic::VertexF vertex;
			vertex.setPosition(utility::graphic::PositionF(x, y, z));
			vertex.setColor(color);
			mesh.addVertex(vertex);
		};

		addVertex(x0, y0, z);
		addVertex(x1, y0, z);
		addVertex(x1, y1, z);
		addVertex(x0, y1, z);

		mesh.addIndex(0);
		mesh.addIndex(1);
		mesh.addIndex(2);
		mesh.addIndex(0);
		mesh.addIndex(2);
		mesh.addIndex(3);

		auto renderable = std::make_shared<MeshRenderable>(
			std::vector<utility::graphic::Mesh> { mesh }, "mesh_material");

		RenderHandle *handle = find(entityIdentifier);
		if (handle == nullptr) {
			insert(entityIdentifier,
				   RenderHandle { _engine->createObject(renderable),
								  renderable, true });
		} else {
			if (!_engine->updateObject(renderable, handle->objectId)) {
				handle->objectId = _engine->createObject(renderable);
			}
			handle->renderable = renderable;
			handle->used	  = true;
		}
	}

}	 // namespace guillaume::systems
