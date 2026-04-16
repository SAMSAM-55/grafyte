from __future__ import annotations

import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

from tests.units._unit_test_support import Application as NativeApplication
from tests.units._unit_test_support import make_caller, load_grafyte

grafyte = load_grafyte()


class WrapperTests(unittest.TestCase):
    def setUp(self):
        NativeApplication.instances.clear()

    def test_scene_spawn_object_uses_default_basic_shader(self):
        scene = grafyte.Scene(grafyte._NativeScene())

        obj = scene.spawn_object((1, 2), (3, 4), layer=5)

        native_scene = scene._Scene__native
        x, y, width, height, shader_source, has_texture, layer, native_obj = native_scene.spawn_calls[-1]
        self.assertIs(obj._get_native(), native_obj)
        self.assertEqual((width, height), (3.0, 4.0))
        self.assertEqual((x, y), (1.0, 2.0))
        self.assertEqual(shader_source, "@embed/Shaders/Basic")
        self.assertFalse(has_texture)
        self.assertEqual(layer, 5)

    def test_scene_spawn_object_uses_default_texture_shader_for_textured_objects(self):
        scene = grafyte.Scene(grafyte._NativeScene())

        scene.spawn_object((0, 0), (10, 10), has_texture=True)

        shader_source = scene._Scene__native.spawn_calls[-1][4]
        self.assertEqual(shader_source, "@embed/Shaders/Texture")

    def test_non_textured_object_color_normalizes_before_forwarding(self):
        obj = grafyte.Object(grafyte._NativeObject(), has_texture=False)

        obj.color = ((64, 128, 255), 0.25)

        self.assertEqual(obj._get_native().last_color, (64 / 255, 128 / 255, 1.0, 0.25))
        self.assertEqual(tuple(obj.color), (64, 128, 255, 0.25))

    def test_textured_object_rejects_color_and_accepts_tint(self):
        obj = grafyte.Object(grafyte._NativeObject(), has_texture=True)

        with self.assertRaises(RuntimeError):
            obj.color = (255, 255, 255)

        obj.tint = (10, 20, 30, 0.75)

        self.assertEqual(obj._get_native().last_color, (10 / 255, 20 / 255, 30 / 255, 0.75))
        self.assertEqual(tuple(obj.tint), (10, 20, 30, 0.75))

    def test_dead_object_mutations_are_no_ops(self):
        native = grafyte._NativeObject()
        native.kill()
        obj = grafyte.Object(native, has_texture=False)

        obj.color = (255, 0, 0)
        obj.pos = (10, 20)

        self.assertIsNone(native.last_color)
        self.assertEqual((native.pos.x, native.pos.y), (0.0, 0.0))

    def test_application_resolves_relative_font_and_normalizes_window_size(self):
        with tempfile.TemporaryDirectory() as tmp:
            caller_dir = Path(tmp)
            caller_file = caller_dir / "game.py"
            caller_file.write_text("# test", encoding="utf-8")
            font = caller_dir / "font.ttf"
            font.write_text("font", encoding="utf-8")

            with patch("grafyte.inspect.stack", return_value=[None, make_caller(str(caller_file))]):
                app = grafyte.Application("Demo", (640.8, 480.2), "font.ttf")

            native_app = NativeApplication.instances[-1]
            self.assertIsInstance(app.input, grafyte.InputManager)
            self.assertTrue(Path(native_app.font_path).samefile(font))
            self.assertEqual(native_app.init_args, (640, 480))

    def test_application_background_color_is_normalized(self):
        with patch("grafyte.inspect.stack", return_value=[None, make_caller(__file__)]):
            app = grafyte.Application("Demo", (100, 200), "@embed/Fonts/Base")

        app.background_color = (255, 64, 0)

        native_app = NativeApplication.instances[-1]
        self.assertEqual(native_app.clear_color, (1.0, 64 / 255, 0.0, 1))

    def test_input_manager_create_action_and_lookup_use_native_static_api(self):
        manager = grafyte.InputManager()
        grafyte._NativeInputManager.active_actions.add("jump")

        manager.create_action("jump", grafyte.InputTrigger.Press, grafyte.Key.Space, grafyte.Key.J)

        self.assertEqual(grafyte._NativeInputManager.actions["jump"][0], grafyte.InputTrigger.Press)
        self.assertEqual(grafyte._NativeInputManager.actions["jump"][1], [grafyte.Key.Space, grafyte.Key.J])
        self.assertTrue(manager["jump"])


if __name__ == "__main__":
    unittest.main()
