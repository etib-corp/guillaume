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

#include "guillaume/systems/image_render.hpp"

namespace guillaume::systems
{
	ImageRender::ImageRender(
		std::shared_ptr<utility::RessourceProvider> ressourceProvider,
		std::unique_ptr<Engine> &engine)
		: ecs::SystemFiller<components::Transform, components::Bound,
							components::Image>(ecs::Phase::Render)
		, _ressourceProvider(ressourceProvider)
		, _engine(engine)
	{
	}

	ImageRender::~ImageRender(void)
	{
		clear();
	}

	void ImageRender::prepare(void)
	{
		apply([this](const ImageRenderCacheKey &key,
					 ImageRenderCacheEntry &entry) {
			entry.used = false;
		});
	}

	void ImageRender::cleanup(void)
	{
		erase_if([this](const ImageRenderCacheKey &key,
						const ImageRenderCacheEntry &entry) {
			if (!entry.used) {
				_engine->removeObject(entry.value);
				return true;
			}
			return false;
		});
	}

	void ImageRender::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		getLogger().debug()
			<< "Updating ImageRender system for entity " << entityIdentifier;

		if (!requireComponent<components::Transform>(entityIdentifier)
			|| !requireComponent<components::Bound>(entityIdentifier)
			|| !requireComponent<components::Image>(entityIdentifier)) {
			return;
		}

		const auto &transformComponent =
			getComponent<components::Transform>(entityIdentifier);
		const auto &boundComponent =
			getComponent<components::Bound>(entityIdentifier);
		const auto &imageComponent =
			getComponent<components::Image>(entityIdentifier);

		ImageRenderCacheKey cacheKey { transformComponent.getPose(),
									   boundComponent,
									   imageComponent.getTexturePath() };

		getLogger().debug() << "Rendering image '" << cacheKey.texturePath
							<< "' for entity " << entityIdentifier;

		if (const auto &entry = get(cacheKey); entry.has_value()) {
			ImageRenderCacheEntry newEntry { .used	= true,
											 .value = entry->value };
			put(cacheKey, std::move(newEntry));
			getLogger().debug() << "Cache hit for entity " << entityIdentifier;
			return;
		}

		if (!_ressourceProvider->loadImageMaterial(cacheKey.texturePath)) {
			getLogger().warning()
				<< "Failed to load image material for texture: "
				<< cacheKey.texturePath;
			return;
		}

		const float halfWidth  = cacheKey.size.getWidth() / 2.0f;
		const float halfHeight = cacheKey.size.getHeight() / 2.0f;

		const utility::graphic::PositionF center(
			cacheKey.pose.getPosition().x + halfWidth,
			cacheKey.pose.getPosition().y + halfHeight,
			cacheKey.pose.getPosition().z);

		const utility::graphic::Color32Bit white(255, 255, 255, 255);

		utility::graphic::Mesh mesh(std::vector<utility::graphic::VertexF> {},
									std::vector<uint32_t> {});

		const auto addVertex = [&](float x, float y, float u, float v) {
			utility::graphic::VertexF vertex;
			vertex.setPosition(utility::graphic::PositionF(
				center[0] + x, center[1] + y, center[2]));
			vertex.setColor(white);
			vertex.setTextureCoordinates(utility::math::Vector2F({ u, v }));
			mesh.addVertex(vertex);
		};

		addVertex(-halfWidth, -halfHeight, 0.0f, 0.0f);
		addVertex(halfWidth, -halfHeight, 1.0f, 0.0f);
		addVertex(halfWidth, halfHeight, 1.0f, 1.0f);
		addVertex(-halfWidth, halfHeight, 0.0f, 1.0f);

		mesh.addIndex(0);
		mesh.addIndex(1);
		mesh.addIndex(2);
		mesh.addIndex(0);
		mesh.addIndex(2);
		mesh.addIndex(3);

		auto identifier =
			_engine->addMesh(mesh, "image_" + cacheKey.texturePath);

		ImageRenderCacheEntry cacheEntry {
			.used  = true,
			.value = identifier,
		};
		put(cacheKey, std::move(cacheEntry));
	}

}	 // namespace guillaume::systems
