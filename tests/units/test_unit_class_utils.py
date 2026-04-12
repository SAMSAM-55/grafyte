from __future__ import annotations

import tempfile
import unittest
from pathlib import Path

from tests.units._unit_test_support import load_grafyte

grafyte = load_grafyte()


class ClassUtilsTests(unittest.TestCase):
    def test_vec2_proxy_updates_x_y_and_in_place_math(self):
        native = grafyte.Object(grafyte._NativeObject())
        native.pos = (2, 3)

        native.pos.x = 5
        native.pos.y = 7
        native.pos += (1, 2)
        native.pos *= 2

        self.assertEqual((native.pos.x, native.pos.y), (12.0, 18.0))

    def test_rot_proxy_supports_in_place_operations(self):
        native = grafyte.Object(grafyte._NativeObject())

        native.rot = 10
        native.rot += 5
        native.rot *= 2

        self.assertEqual(native.rot, 30.0)

    def test_asset_resolver_returns_embedded_path_unchanged(self):
        resolver = grafyte.AssetResolver(Path.cwd())
        self.assertEqual(resolver.resolve("@embed/Fonts/Base"), "@embed/Fonts/Base")

    def test_asset_resolver_resolves_relative_path_from_base_directory(self):
        with tempfile.TemporaryDirectory() as tmp:
            base = Path(tmp)
            asset = base / "font.ttf"
            asset.write_text("test", encoding="utf-8")

            resolver = grafyte.AssetResolver(base)

            self.assertEqual(resolver.resolve("font.ttf"), str(asset))

    def test_asset_resolver_raises_for_missing_file(self):
        resolver = grafyte.AssetResolver(Path.cwd())

        with self.assertRaises(FileNotFoundError):
            resolver.resolve("missing.file")


if __name__ == "__main__":
    unittest.main()
