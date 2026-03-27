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

#include "guillaume/renderer.hpp"

#include <algorithm>
#include <cmath>

namespace
{
	utility::math::Vector<float, 3>
		rotateVectorByQuaternion(const utility::math::Vector<float, 3> &vector,
								 const utility::graphics::Rotation &rotation)
	{
		const auto normalizedRotation = rotation.normalizedQuaternion();
		const float qx				  = normalizedRotation.getX();
		const float qy				  = normalizedRotation.getY();
		const float qz				  = normalizedRotation.getZ();
		const float qw				  = normalizedRotation.getW();
		const float vectorX			  = vector[0];
		const float vectorY			  = vector[1];
		const float vectorZ			  = vector[2];

		const float crossX = (qy * vectorZ) - (qz * vectorY);
		const float crossY = (qz * vectorX) - (qx * vectorZ);
		const float crossZ = (qx * vectorY) - (qy * vectorX);

		const float tX = 2.0f * crossX;
		const float tY = 2.0f * crossY;
		const float tZ = 2.0f * crossZ;

		const float crossTX = (qy * tZ) - (qz * tY);
		const float crossTY = (qz * tX) - (qx * tZ);
		const float crossTZ = (qx * tY) - (qy * tX);

		return {
			vectorX + (qw * tX) + crossTX,
			vectorY + (qw * tY) + crossTY,
			vectorZ + (qw * tZ) + crossTZ,
		};
	}

	/**
	 * @brief Extract a quaternion from camera forward/up basis vectors.
	 * @param forward Camera forward direction.
	 * @param up Camera up direction.
	 * @return Rotation quaternion.
	 */
	utility::graphics::Rotation
		quaternionFromBasis(const utility::math::Vector<float, 3> &forward,
							const utility::math::Vector<float, 3> &up)
	{
		const auto normalizedForward = forward.normalized();
		const auto right			 = normalizedForward.cross(up).normalized();
		const auto orthonormalUp = right.cross(normalizedForward).normalized();

		const auto back = -normalizedForward;

		const float m00 = right[0];
		const float m01 = orthonormalUp[0];
		const float m02 = back[0];
		const float m10 = right[1];
		const float m11 = orthonormalUp[1];
		const float m12 = back[1];
		const float m20 = right[2];
		const float m21 = orthonormalUp[2];
		const float m22 = back[2];

		float qx = 0.0f;
		float qy = 0.0f;
		float qz = 0.0f;
		float qw = 1.0f;

		const float trace = m00 + m11 + m22;
		if (trace > 0.0f) {
			const float scale = std::sqrt(trace + 1.0f) * 2.0f;
			qw				  = 0.25f * scale;
			qx				  = (m21 - m12) / scale;
			qy				  = (m02 - m20) / scale;
			qz				  = (m10 - m01) / scale;
		} else if ((m00 > m11) && (m00 > m22)) {
			const float scale = std::sqrt(1.0f + m00 - m11 - m22) * 2.0f;
			qw				  = (m21 - m12) / scale;
			qx				  = 0.25f * scale;
			qy				  = (m01 + m10) / scale;
			qz				  = (m02 + m20) / scale;
		} else if (m11 > m22) {
			const float scale = std::sqrt(1.0f + m11 - m00 - m22) * 2.0f;
			qw				  = (m02 - m20) / scale;
			qx				  = (m01 + m10) / scale;
			qy				  = 0.25f * scale;
			qz				  = (m12 + m21) / scale;
		} else {
			const float scale = std::sqrt(1.0f + m22 - m00 - m11) * 2.0f;
			qw				  = (m10 - m01) / scale;
			qx				  = (m02 + m20) / scale;
			qy				  = (m12 + m21) / scale;
			qz				  = 0.25f * scale;
		}

		auto rotation = utility::graphics::Rotation(qx, qy, qz, qw);
		rotation.normalizeQuaternion();
		return rotation;
	}

}	 // namespace

namespace guillaume
{

	Renderer::Renderer(void)
		: Loggable()
	{
		_camera.setPosition({ 0.0f, 0.0f, 5.0f });
		_lastMouseRay = Ray(_camera.getPosition(), _camera.getForward());
	}

	void Renderer::setCamera(const Camera &camera)
	{
		_camera = camera;
		_cameraRotation =
			quaternionFromBasis(_camera.getForward(), _camera.getUp());
		_lastMouseRay = Ray(_camera.getPosition(), _camera.getForward());
	}

	/**
	 * @brief Set camera quaternion rotation and update orientation basis.
	 * @param rotation Camera rotation quaternion.
	 */
	void Renderer::setCameraRotation(const Rotation &rotation)
	{
		_cameraRotation = rotation.normalizedQuaternion();

		const auto forward =
			rotateVectorByQuaternion({ 0.0f, 0.0f, -1.0f }, _cameraRotation)
				.normalized();
		const auto up =
			rotateVectorByQuaternion({ 0.0f, 1.0f, 0.0f }, _cameraRotation)
				.normalized();

		_camera.setForward(forward);
		_camera.setUp(up);
		_lastMouseRay = Ray(_camera.getPosition(), _camera.getForward());
	}

}	 // namespace guillaume