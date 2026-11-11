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

#include "guillaume/systems/core_systems.hpp"

#include "guillaume/systems/animation.hpp"
#include "guillaume/systems/arc_render.hpp"
#include "guillaume/systems/backdrop.hpp"
#include "guillaume/systems/badge.hpp"
#include "guillaume/systems/button.hpp"
#include "guillaume/systems/checkbox.hpp"
#include "guillaume/systems/chip.hpp"
#include "guillaume/systems/common_button.hpp"
#include "guillaume/systems/extended_floating_action_button.hpp"
#include "guillaume/systems/floating_action_button.hpp"
#include "guillaume/systems/icon_button.hpp"
#include "guillaume/systems/clip.hpp"
#include "guillaume/systems/divider.hpp"
#include "guillaume/systems/drag.hpp"
#include "guillaume/systems/elevation_render.hpp"
#include "guillaume/systems/ellipse_render.hpp"
#include "guillaume/systems/focus.hpp"
#include "guillaume/systems/focus_ring_render.hpp"
#include "guillaume/systems/glyph_render.hpp"
#include "guillaume/systems/hand_button.hpp"
#include "guillaume/systems/hand_motion.hpp"
#include "guillaume/systems/hand_pinch.hpp"
#include "guillaume/systems/hand_poke.hpp"
#include "guillaume/systems/hand_squeeze.hpp"
#include "guillaume/systems/hand_thumb_rest.hpp"
#include "guillaume/systems/hand_thumb_stick.hpp"
#include "guillaume/systems/hand_trigger.hpp"
#include "guillaume/systems/image_render.hpp"
#include "guillaume/systems/keyboard_control.hpp"
#include "guillaume/systems/layout.hpp"
#include "guillaume/systems/line_render.hpp"
#include "guillaume/systems/loading_indicator.hpp"
#include "guillaume/systems/measure_glyph.hpp"
#include "guillaume/systems/measure_mesh.hpp"
#include "guillaume/systems/measure_text.hpp"
#include "guillaume/systems/mesh_render.hpp"
#include "guillaume/systems/mouse_button.hpp"
#include "guillaume/systems/mouse_motion.hpp"
#include "guillaume/systems/overlay.hpp"
#include "guillaume/systems/progress_indicator.hpp"
#include "guillaume/systems/rectangle_render.hpp"
#include "guillaume/systems/ring_render.hpp"
#include "guillaume/systems/scrim_render.hpp"
#include "guillaume/systems/scroll.hpp"
#include "guillaume/systems/selection.hpp"
#include "guillaume/systems/radio_button.hpp"
#include "guillaume/systems/style_state.hpp"
#include "guillaume/systems/surface_tone.hpp"
#include "guillaume/systems/switch.hpp"
#include "guillaume/systems/text_field.hpp"
#include "guillaume/systems/text_input.hpp"
#include "guillaume/systems/text_render.hpp"
#include "guillaume/systems/typography.hpp"

namespace guillaume::systems
{
	void registerCoreSystems(
		ecs::SystemRegistry &systemRegistry, event::EventBus &eventBus,
		utility::Engine &engine,
		std::shared_ptr<utility::RessourceProvider> ressourceProvider)
	{
		// Typography resolves the metrics MeasureText measures with, and both
		// run in the Measure phase: keep it registered first.
		systemRegistry.registerNewSystem(
			std::make_unique<systems::Typography>());
		systemRegistry.registerNewSystem(
			std::make_unique<systems::SurfaceTone>());
		systemRegistry.registerNewSystem(
			std::make_unique<systems::MeasureText>(ressourceProvider, engine));
		systemRegistry.registerNewSystem(
			std::make_unique<systems::MeasureGlyph>(ressourceProvider, engine));
		systemRegistry.registerNewSystem(
			std::make_unique<systems::MouseMotion>(eventBus, engine));
		systemRegistry.registerNewSystem(
			std::make_unique<systems::MouseButton>(eventBus, engine));
		systemRegistry.registerNewSystem(
			std::make_unique<systems::Drag>(eventBus));
		systemRegistry.registerNewSystem(
			std::make_unique<systems::Scroll>(eventBus));
		systemRegistry.registerNewSystem(
			std::make_unique<systems::Selection>());
		systemRegistry.registerNewSystem(
			std::make_unique<systems::HandMotion>(eventBus));
		systemRegistry.registerNewSystem(
			std::make_unique<systems::HandButton>(eventBus));
		systemRegistry.registerNewSystem(
			std::make_unique<systems::HandPinch>(eventBus));
		systemRegistry.registerNewSystem(
			std::make_unique<systems::HandPoke>(eventBus));
		systemRegistry.registerNewSystem(
			std::make_unique<systems::HandSqueeze>(eventBus));
		systemRegistry.registerNewSystem(
			std::make_unique<systems::HandThumbRest>(eventBus));
		systemRegistry.registerNewSystem(
			std::make_unique<systems::HandThumbStick>(eventBus));
		systemRegistry.registerNewSystem(
			std::make_unique<systems::HandTrigger>(eventBus));
		systemRegistry.registerNewSystem(
			std::make_unique<systems::TextRender>(ressourceProvider, engine));
		systemRegistry.registerNewSystem(
			std::make_unique<systems::GlyphRender>(ressourceProvider, engine));
		systemRegistry.registerNewSystem(
			std::make_unique<systems::KeyboardControl>(eventBus));
		systemRegistry.registerNewSystem(
			std::make_unique<systems::TextInput>(eventBus));
		systemRegistry.registerNewSystem(
			std::make_unique<systems::RectangleRender>(engine));
		systemRegistry.registerNewSystem(
			std::make_unique<systems::EllipseRender>(engine));
		systemRegistry.registerNewSystem(
			std::make_unique<systems::RingRender>(engine));
		systemRegistry.registerNewSystem(
			std::make_unique<systems::ArcRender>(engine));
		systemRegistry.registerNewSystem(
			std::make_unique<systems::LineRender>(engine));
		systemRegistry.registerNewSystem(
			std::make_unique<systems::ElevationRender>(engine));
		systemRegistry.registerNewSystem(
			std::make_unique<systems::ScrimRender>(engine));
		systemRegistry.registerNewSystem(
			std::make_unique<systems::Clip>(engine));
		systemRegistry.registerNewSystem(std::make_unique<systems::Divider>());
		systemRegistry.registerNewSystem(std::make_unique<systems::Backdrop>());
		systemRegistry.registerNewSystem(std::make_unique<systems::Layout>());
		// Widget systems place children the Layout system does not arrange,
		// from the bounds it has just resolved: keep them after it.
		systemRegistry.registerNewSystem(std::make_unique<systems::Checkbox>());
		systemRegistry.registerNewSystem(
			std::make_unique<systems::RadioButton>());
		systemRegistry.registerNewSystem(std::make_unique<systems::Switch>());
		systemRegistry.registerNewSystem(
			std::make_unique<systems::CommonButton>());
		systemRegistry.registerNewSystem(
			std::make_unique<systems::IconButton>());
		systemRegistry.registerNewSystem(
			std::make_unique<systems::FloatingActionButton>());
		systemRegistry.registerNewSystem(
			std::make_unique<systems::ExtendedFloatingActionButton>());
		systemRegistry.registerNewSystem(std::make_unique<systems::Badge>());
		systemRegistry.registerNewSystem(std::make_unique<systems::Chip>());
		systemRegistry.registerNewSystem(
			std::make_unique<systems::LoadingIndicator>());
		systemRegistry.registerNewSystem(
			std::make_unique<systems::ProgressIndicator>());
		systemRegistry.registerNewSystem(
			std::make_unique<systems::Focus>(eventBus, engine));
		systemRegistry.registerNewSystem(
			std::make_unique<systems::StyleState>());
		systemRegistry.registerNewSystem(
			std::make_unique<systems::Overlay>(eventBus, engine));
		systemRegistry.registerNewSystem(
			std::make_unique<systems::FocusRingRender>(engine));
		systemRegistry.registerNewSystem(
			std::make_unique<systems::Animation>(engine));
		systemRegistry.registerNewSystem(
			std::make_unique<systems::TextField>());
		systemRegistry.registerNewSystem(
			std::make_unique<systems::MeasureMesh>(ressourceProvider, engine));
		systemRegistry.registerNewSystem(
			std::make_unique<systems::MeshRender>(ressourceProvider, engine));
		systemRegistry.registerNewSystem(
			std::make_unique<systems::ImageRender>(ressourceProvider, engine));
	}
}	 // namespace guillaume::systems
