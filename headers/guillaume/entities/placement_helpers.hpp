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

#include "guillaume/systems/placement_helpers.hpp"

/**
 * @file
 * @brief Makes the placement helpers callable from guillaume::entities
 * without qualification.
 *
 * This header declares nothing of its own: the functions are defined in
 * guillaume::systems::placement. They read the pose of an entity and
 * position a child relative to its parent, for the widgets that place
 * children themselves instead of relying on systems::Layout.
 *
 * @see guillaume::systems::placement
 */
namespace guillaume::entities
{
	using systems::placement::centerChild;
	using systems::placement::placeChild;
	using systems::placement::placeNormalized;
	using systems::placement::poseOf;
}	 // namespace guillaume::entities
