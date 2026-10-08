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

#include "guillaume/systems/ring_render.hpp"

#include <cmath>
#include <memory>
#include <vector>

#include "guillaume/mesh_renderable.hpp"
#include "guillaume/systems/shape_mesh.hpp"

namespace guillaume::systems
{
	RingRender::RingRender(std::unique_ptr<utility::Engine> &engine)
		: ecs::SystemFiller<components::Transform, components::Bound,
							components::Color, components::Ring>(
			  ecs::Phase::Render)
		, _engine(engine)
	{
	}

	void RingRender::prepare(void)
	{
		markAllUnused();
	}

	void RingRender::cleanup(void)
	{
		removeUnused(*_engine);
	}

	void RingRender::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		if (!requireComponent<components::Bound>(entityIdentifier)
			|| !requireComponent<components::Transform>(entityIdentifier)
			|| !requireComponent<components::Color>(entityIdentifier)
			|| !requireComponent<components::Ring>(entityIdentifier)) {
			return;
		}

		const auto &pose =
			getComponent<components::Transform>(entityIdentifier).getPose();
		const auto &bound = getComponent<components::Bound>(entityIdentifier);
		const auto &color =
			getComponent<components::Color>(entityIdentifier).getColor();
		const auto &ring = getComponent<components::Ring>(entityIdentifier);

		const utility::graphic::PositionF center(
			pose.getPosition().x + bound.getWidth() / 2.0f,
			pose.getPosition().y + bound.getHeight() / 2.0f,
			pose.getPosition().z);

		utility::graphic::Mesh mesh(std::vector<utility::graphic::VertexF> {},
									std::vector<uint32_t> {});

		shape::buildRingMesh(mesh, center, pose.getOrientation(),
							 bound.getWidth() / 2.0f, bound.getHeight() / 2.0f,
							 ring.getThickness(), 0.0f, 2.0f * std::acos(-1.0f),
							 ring.getSegments(), true, color);

		auto renderable = std::make_shared<MeshRenderable>(
			std::vector<utility::graphic::Mesh> { mesh }, "mesh_material");

		RenderHandle *handle = find(entityIdentifier);
		if (handle == nullptr) {
			insert(entityIdentifier,
				   RenderHandle { _engine->createObject(renderable), renderable,
								  true });
		} else {
			if (!_engine->updateObject(renderable, handle->objectId)) {
				handle->objectId = _engine->createObject(renderable);
			}
			handle->renderable = renderable;
			handle->used	   = true;
		}
	}

}	 // namespace guillaume::systems
