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

#include "guillaume/systems/focus_ring_render.hpp"

#include <vector>

#include "guillaume/components/borders.hpp"
#include "guillaume/mesh_renderable.hpp"
#include "guillaume/systems/rectangle_render.hpp"
#include "guillaume/systems/shape_mesh.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::systems
{
	FocusRingRender::FocusRingRender(utility::Engine &engine)
		: ecs::SystemFiller<components::Transform, components::Bound,
							components::StyleState>(ecs::Phase::Render)
		, _engine(engine)
	{
	}

	void FocusRingRender::prepare(void)
	{
		markAllUnused();
	}

	void FocusRingRender::cleanup(void)
	{
		removeUnused(_engine);
	}

	void
		FocusRingRender::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		if (!requireComponent<components::Transform>(entityIdentifier)
			|| !requireComponent<components::Bound>(entityIdentifier)
			|| !requireComponent<components::StyleState>(entityIdentifier)) {
			return;
		}

		const auto &state =
			getComponent<components::StyleState>(entityIdentifier);
		if (!state.isFocused()) {
			return;
		}

		const auto &pose =
			getComponent<components::Transform>(entityIdentifier).getPose();
		const auto &bound = getComponent<components::Bound>(entityIdentifier);

		float radius = 0.0f;
		if (hasComponent<components::Borders>(entityIdentifier)) {
			const auto &borders =
				getComponent<components::Borders>(entityIdentifier);
			radius = (borders.getTopLeftRadius() + borders.getTopRightRadius()
					  + borders.getBottomRightRadius()
					  + borders.getBottomLeftRadius())
				/ 4.0f;
		}

		constexpr float grow =
			theme::FocusRing::Offset + theme::FocusRing::Width;
		const float halfWidth	= bound.getWidth() / 2.0f + grow;
		const float halfHeight	= bound.getHeight() / 2.0f + grow;
		const float outerRadius = radius > 0.0f ? radius + grow : 0.0f;

		const utility::graphic::PositionF lift(
			-pose.getOrientation().getForward() * RectangleRender::StrokeLift);
		const utility::graphic::PositionF center(
			pose.getPosition().x + bound.getWidth() / 2.0f + lift[0],
			pose.getPosition().y + bound.getHeight() / 2.0f + lift[1],
			pose.getPosition().z + lift[2]);

		utility::graphic::Mesh mesh(std::vector<utility::graphic::VertexF> {},
									std::vector<uint32_t> {});
		shape::buildRoundedStrokeMesh(
			mesh, center, pose.getOrientation(), halfWidth, halfHeight,
			outerRadius, theme::FocusRing::Width, 16,
			getActiveColor(SchemeColorRole::Secondary));

		auto renderable = std::make_shared<MeshRenderable>(
			std::vector<utility::graphic::Mesh> { mesh }, "mesh_material");

		RenderHandle *handle = find(entityIdentifier);
		if (handle == nullptr) {
			insert(entityIdentifier,
				   RenderHandle { _engine.createObject(renderable), renderable,
								  true });
		} else {
			if (!_engine.updateObject(renderable, handle->objectId)) {
				handle->objectId = _engine.createObject(renderable);
			}
			handle->renderable = renderable;
			handle->used	   = true;
		}
	}

}	 // namespace guillaume::systems
