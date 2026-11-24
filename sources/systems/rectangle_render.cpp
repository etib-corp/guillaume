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

#include "guillaume/systems/rectangle_render.hpp"

#include "guillaume/components/ring.hpp"
#include "guillaume/mesh_renderable.hpp"
#include "guillaume/systems/shape_mesh.hpp"

#include <memory>
#include <vector>

namespace guillaume::systems
{
	RectangleRender::RectangleRender(utility::Engine &engine)
		: ecs::SystemFiller<components::Transform, components::Bound,
							components::Color, components::Borders>(
			  ecs::Phase::Render)
		, _engine(engine)
	{
	}

	RectangleRender::~RectangleRender(void)
	{
		clear();
	}

	void RectangleRender::prepare(void)
	{
		markAllUnused();
	}

	void RectangleRender::cleanup(void)
	{
		removeUnused(_engine);
	}

	void
		RectangleRender::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		getLogger().debug() << "Updating RectangleRender system for entity "
							<< entityIdentifier;

		if (!requireComponent<components::Bound>(entityIdentifier)
			|| !requireComponent<components::Transform>(entityIdentifier)
			|| !requireComponent<components::Color>(entityIdentifier)
			|| !requireComponent<components::Borders>(entityIdentifier)) {
			return;
		}

		// An entity with a Ring is drawn as that ring by systems::RingRender,
		// with the same Color: filling its rectangle would cover the ring
		// (a radio button would be a filled disc).
		if (getComponentRegistry().hasComponent<components::Ring>(
				entityIdentifier)) {
			return;
		}

		const auto &transformComponent =
			getComponent<components::Transform>(entityIdentifier);
		const auto &boundComponent =
			getComponent<components::Bound>(entityIdentifier);
		const auto &colorComponent =
			getComponent<components::Color>(entityIdentifier);
		const auto &bordersComponent =
			getComponent<components::Borders>(entityIdentifier);

		const auto &pose  = transformComponent.getPose();
		const auto &color = colorComponent.getColor();

		getLogger().debug()
			<< "Rendering rectangle with pose: " << pose
			<< ", size: " << boundComponent.getWidth() << "x"
			<< boundComponent.getHeight() << ", color: " << color;

		const utility::graphic::PositionF center =
			shape::pointInPlane(pose, boundComponent.getWidth() / 2.0f,
								-boundComponent.getHeight() / 2.0f);

		// Each corner keeps its own radius (a connected segmented button has
		// two square and two rounded corners).
		const shape::CornerRadii radii = shape::cornerRadiiOf(bordersComponent);
		const auto roundedVertices	   = shape::transformToWorldVertices(
			shape::buildRoundedRectOutline(boundComponent.getWidth() / 2.0f,
											   boundComponent.getHeight() / 2.0f,
											   radii, ArcSegments, 0.001f),
			center, pose.getOrientation());

		utility::graphic::Mesh mesh(std::vector<utility::graphic::VertexF> {},
									std::vector<uint32_t> {});
		shape::buildTriangleFanVertices(mesh, center, roundedVertices, color);

		std::vector<utility::graphic::Mesh> meshes;
		if (!mesh.getVertices().empty()) {
			meshes.push_back(mesh);
		}

		if (bordersComponent.hasStroke()) {
			const utility::graphic::PositionF strokeCenter(
				-pose.getOrientation().getForward() * StrokeLift);
			utility::graphic::Mesh stroke(
				std::vector<utility::graphic::VertexF> {},
				std::vector<uint32_t> {});
			shape::buildRoundedStrokeMesh(
				stroke,
				utility::graphic::PositionF(center[0] + strokeCenter[0],
											center[1] + strokeCenter[1],
											center[2] + strokeCenter[2]),
				pose.getOrientation(), boundComponent.getWidth() / 2.0f,
				boundComponent.getHeight() / 2.0f, radii,
				bordersComponent.getWidth(), ArcSegments,
				bordersComponent.getColor());
			if (!stroke.getVertices().empty()) {
				meshes.push_back(stroke);
			}
		}

		// Transparent fill and no visible stroke: submit nothing, so the
		// object is removed at the end of the frame.
		if (meshes.empty()) {
			return;
		}

		auto renderable =
			std::make_shared<MeshRenderable>(meshes, "mesh_material");

		RenderHandle *handle = find(entityIdentifier);
		if (handle == nullptr) {
			size_t objectId = _engine.createObject(renderable);
			insert(entityIdentifier,
				   RenderHandle { objectId, renderable, true });
		} else {
			if (!_engine.updateObject(renderable, handle->objectId)) {
				handle->objectId = _engine.createObject(renderable);
			}
			handle->renderable = renderable;
			handle->used	   = true;
		}
	}

}	 // namespace guillaume::systems