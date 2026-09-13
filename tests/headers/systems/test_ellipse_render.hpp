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

#pragma once

#include <gtest/gtest.h>

#include <memory>
#include <string>
#include <vector>

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity_registry_container.hpp"

#include "guillaume/components/bound.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/ellipse.hpp"
#include "guillaume/components/transform.hpp"

#include "guillaume/engine.hpp"
#include "guillaume/systems/ellipse_render.hpp"

#include <utility/graphic/mesh.hpp>
#include <utility/graphic/pose.hpp>
#include <utility/graphic/text/text.hpp>
#include <utility/graphic/view.hpp>

namespace guillaume::systems::tests
{

	/**
	 * @brief Engine stub recording the meshes added to it.
	 */
	class EllipseEngineStub: public Engine
	{
		public:
		int addMeshCount { 0 };			///< Number of addMesh calls
		int removeObjectCount { 0 };	///< Number of removeObject calls
		utility::graphic::Mesh lastMesh {
			std::vector<utility::graphic::VertexF> {}, std::vector<uint32_t> {}
		};	  ///< Last mesh passed to addMesh

		void clear(void) override
		{
		}

		void present(void) override
		{
		}

		size_t addMesh(const utility::graphic::Mesh &mesh,
					   const std::string &) override
		{
			++addMeshCount;
			lastMesh = mesh;
			return static_cast<size_t>(addMeshCount);
		}

		bool removeObject(size_t) override
		{
			++removeObjectCount;
			return true;
		}

		utility::graphic::SizeF
			measureText(const utility::graphic::Text &) const override
		{
			return { 0.0f, 0.0f };
		}

		size_t addText(utility::graphic::Text) override
		{
			return 0;
		}

		size_t addModel(std::shared_ptr<utility::graphic::Model>) override
		{
			return 0;
		}

		utility::graphic::ViewF getView(void) const override
		{
			return utility::graphic::ViewF();
		}

		void addScene(size_t) override
		{
		}

		void pollEvents(void) override
		{
		}

		void update(void) override
		{
		}
	};

	class TestEllipseRender: public ::testing::Test
	{
		protected:
		std::unique_ptr<Engine> _engine;
		EllipseEngineStub *_engineStub { nullptr };
		std::unique_ptr<EllipseRender> _ellipseSystem;
		ecs::ComponentRegistry _componentRegistry;
		ecs::EntityRegistryContainer _entityRegistry;

		TestEllipseRender(void)			  = default;
		~TestEllipseRender(void) override = default;

		void SetUp(void) override
		{
			_engine		   = std::make_unique<EllipseEngineStub>();
			_engineStub	   = static_cast<EllipseEngineStub *>(_engine.get());
			_ellipseSystem = std::make_unique<EllipseRender>(_engine);
			_ellipseSystem->bindComponentRegistry(_componentRegistry);
		}

		void TearDown(void) override
		{
			_ellipseSystem->unbindComponentRegistry();
			_ellipseSystem.reset();
			_engine.reset();
		}

		/**
		 * @brief Create an entity carrying the ellipse render components.
		 * @param pose Initial pose.
		 * @param width Bounding box width.
		 * @param height Bounding box height.
		 * @param segments Outline segment count.
		 * @param color Fill color.
		 * @return The identifier of the created entity.
		 */
		ecs::Entity::Identifier createEllipseEntity(
			const utility::graphic::PoseF &pose = utility::graphic::PoseF(),
			float width = 100.0f, float height = 50.0f, int segments = 8,
			const utility::graphic::Color32Bit &color = { 255, 255, 255, 255 })
		{
			auto entity = std::make_unique<ecs::Entity>();
			auto id		= entity->getIdentifier();
			entity->setSignature(ecs::Entity::getSignatureFromTypes<
								 components::Transform, components::Bound,
								 components::Color, components::Ellipse>());
			_entityRegistry.addEntity(std::move(entity));

			_componentRegistry.addComponent<components::Transform>(id);
			_componentRegistry.addComponent<components::Bound>(id);
			_componentRegistry.addComponent<components::Color>(id);
			_componentRegistry.addComponent<components::Ellipse>(id);

			auto &transform =
				_componentRegistry.getComponent<components::Transform>(id);
			transform.setPose(pose);

			auto &bound =
				_componentRegistry.getComponent<components::Bound>(id);
			bound.setWidth(width);
			bound.setHeight(height);

			auto &colorComponent =
				_componentRegistry.getComponent<components::Color>(id);
			colorComponent.setColor(color);

			auto &ellipse =
				_componentRegistry.getComponent<components::Ellipse>(id);
			ellipse.setSegments(segments);

			return id;
		}
	};

}	 // namespace guillaume::systems::tests
