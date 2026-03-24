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

	constexpr float degreeToRadian = 0.01745329251994329576923690768489f;

	/**
	 * @brief Rotate a vector by Euler angles (degrees) in X->Y->Z order.
	 * @param vector Input vector.
	 * @param rotation Euler rotation angles in degrees.
	 * @return Rotated vector.
	 */
	utility::math::Vector<float, 3> rotateVectorByEulerDegrees(
		const utility::math::Vector<float, 3> &vector,
		const utility::math::Vector<float, 3> &rotation)
	{
		const float rotationX = rotation[0] * degreeToRadian;
		const float rotationY = rotation[1] * degreeToRadian;
		const float rotationZ = rotation[2] * degreeToRadian;

		const float cosineX = std::cos(rotationX);
		const float sineX	= std::sin(rotationX);
		const float cosineY = std::cos(rotationY);
		const float sineY	= std::sin(rotationY);
		const float cosineZ = std::cos(rotationZ);
		const float sineZ	= std::sin(rotationZ);

		utility::math::Vector<float, 3> result = vector;

		const float yAfterX = (result[1] * cosineX) - (result[2] * sineX);
		const float zAfterX = (result[1] * sineX) + (result[2] * cosineX);
		result[1]			= yAfterX;
		result[2]			= zAfterX;

		const float xAfterY = (result[0] * cosineY) + (result[2] * sineY);
		const float zAfterY = (-result[0] * sineY) + (result[2] * cosineY);
		result[0]			= xAfterY;
		result[2]			= zAfterY;

		const float xAfterZ = (result[0] * cosineZ) - (result[1] * sineZ);
		const float yAfterZ = (result[0] * sineZ) + (result[1] * cosineZ);
		result[0]			= xAfterZ;
		result[1]			= yAfterZ;

		return result;
	}

	/**
	 * @brief Extract Euler angles (degrees) from camera forward/up basis
	 * vectors.
	 * @param forward Camera forward direction.
	 * @param up Camera up direction.
	 * @return Euler rotation angles in degrees.
	 */
	utility::math::Vector<float, 3>
		eulerDegreesFromBasis(const utility::math::Vector<float, 3> &forward,
							  const utility::math::Vector<float, 3> &up)
	{
		const auto normalizedForward = forward.normalized();
		const auto right			 = normalizedForward.cross(up).normalized();
		const auto orthonormalUp = right.cross(normalizedForward).normalized();

		const float r00 = right[0];
		const float r10 = right[1];
		const float r20 = right[2];

		const float r21 = orthonormalUp[2];

		const float r22 = -normalizedForward[2];

		const float clampedValue = std::clamp(-r20, -1.0f, 1.0f);
		const float rotationY	 = std::asin(clampedValue);
		const float cosineY		 = std::cos(rotationY);

		float rotationX = 0.0f;
		float rotationZ = 0.0f;

		if (std::abs(cosineY) > 1.0e-6f) {
			rotationX = std::atan2(r21, r22);
			rotationZ = std::atan2(r10, r00);
		} else {
			rotationX = 0.0f;
			rotationZ = std::atan2(-orthonormalUp[0], orthonormalUp[1]);
		}

		constexpr float radianToDegree = 57.295779513082320876798154814105f;
		return { rotationX * radianToDegree, rotationY * radianToDegree,
				 rotationZ * radianToDegree };
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
			eulerDegreesFromBasis(_camera.getForward(), _camera.getUp());
		_lastMouseRay = Ray(_camera.getPosition(), _camera.getForward());
	}

	/**
	 * @brief Set camera Euler rotation and update the camera orientation basis.
	 * @param rotation Euler rotation in degrees.
	 */
	void Renderer::setCameraRotation(const Rotation &rotation)
	{
		_cameraRotation = rotation;

		const auto forward =
			rotateVectorByEulerDegrees({ 0.0f, 0.0f, -1.0f }, _cameraRotation)
				.normalized();
		const auto up =
			rotateVectorByEulerDegrees({ 0.0f, 1.0f, 0.0f }, _cameraRotation)
				.normalized();

		_camera.setForward(forward);
		_camera.setUp(up);
		_lastMouseRay = Ray(_camera.getPosition(), _camera.getForward());
	}

}	 // namespace guillaume