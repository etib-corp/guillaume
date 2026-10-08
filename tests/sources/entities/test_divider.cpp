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

#include "entities/test_divider.hpp"

#include <memory>

#include <utility/graphic/color.hpp>

#include <guillaume/components/bound.hpp>
#include <guillaume/components/color.hpp>
#include <guillaume/components/line.hpp>
#include <guillaume/components/transform.hpp>
#include <guillaume/ecs/entity.hpp>
#include <guillaume/ecs/entity_registry_container.hpp>

namespace guillaume::entities::tests
{
	namespace
	{
		std::shared_ptr<Divider>
			makeDivider(ecs::ComponentRegistry &registry, float length,
						Divider::Variant variant = Divider::Variant::FullWidth)
		{
			ecs::EntityRegistryContainer &entityRegistry =
				scratchEntityRegistry();
			Divider::Builder builder(registry, entityRegistry);
			Divider::Director director;

			return director.makeDivider(builder, nullptr, length, variant);
		}
	}	 // namespace

	TEST_F(TestDivider, FullWidthUsesWholeLength)
	{
		ecs::ComponentRegistry registry;
		auto divider = makeDivider(registry, 120.0f);

		divider->initialize();
		divider->update();

		const auto &bound =
			registry.getComponent<components::Bound>(divider->getIdentifier());
		EXPECT_FLOAT_EQ(bound.getWidth(), 120.0f);
		EXPECT_FLOAT_EQ(bound.getHeight(), 1.0f);
	}

	TEST_F(TestDivider, InsetTrimsLeadingEdgeOnly)
	{
		std::unique_ptr<ecs::ComponentRegistry> registry =
			std::make_unique<ecs::ComponentRegistry>();
		auto divider = makeDivider(*registry, 120.0f, Divider::Variant::Inset);
		divider->setInset(16.0f);

		divider->initialize();
		divider->update();

		const auto &bound =
			registry->getComponent<components::Bound>(divider->getIdentifier());
		EXPECT_FLOAT_EQ(bound.getWidth(), 104.0f);

		const auto &pose =
			registry
				->getComponent<components::Transform>(divider->getIdentifier())
				.getPose();
		EXPECT_FLOAT_EQ(pose.getPosition().getX(), 16.0f);
	}

	TEST_F(TestDivider, MiddleTrimsBothEdges)
	{
		ecs::ComponentRegistry registry;
		auto divider = makeDivider(registry, 120.0f, Divider::Variant::Middle);
		divider->setInset(16.0f);

		divider->initialize();
		divider->update();

		const auto &bound =
			registry.getComponent<components::Bound>(divider->getIdentifier());
		EXPECT_FLOAT_EQ(bound.getWidth(), 88.0f);
	}

	TEST_F(TestDivider, ThicknessPropagatesToLine)
	{
		ecs::ComponentRegistry registry;
		auto divider = makeDivider(registry, 120.0f);

		divider->setThickness(4.0f);
		divider->update();

		EXPECT_FLOAT_EQ(
			registry.getComponent<components::Line>(divider->getIdentifier())
				.getThickness(),
			4.0f);
		EXPECT_FLOAT_EQ(
			registry.getComponent<components::Bound>(divider->getIdentifier())
				.getHeight(),
			4.0f);
	}

	TEST_F(TestDivider, ColorRoundTrip)
	{
		ecs::ComponentRegistry registry;
		auto divider = makeDivider(registry, 120.0f);

		divider->setColor(utility::graphic::Color32Bit(10, 20, 30, 255));

		const auto &color =
			registry.getComponent<components::Color>(divider->getIdentifier());
		EXPECT_EQ(color.getColor().getRed(), 10);
		EXPECT_EQ(color.getColor().getAlpha(), 255);
	}

	TEST_F(TestDivider, DirectorBuildsAndRegisters)
	{
		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer entityRegistry;
		auto parent = std::make_shared<ecs::Entity>();

		Divider::Builder builder(registry, entityRegistry);
		Divider::Director director;

		auto divider = director.makeDivider(builder, parent, 80.0f,
											Divider::Variant::Middle);

		ASSERT_NE(divider, nullptr);
		EXPECT_EQ(divider->getParent(), parent);
		EXPECT_EQ(divider->getVariant(), Divider::Variant::Middle);
	}

}	 // namespace guillaume::entities::tests
