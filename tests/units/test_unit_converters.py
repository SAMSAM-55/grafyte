from __future__ import annotations

import unittest

from tests.units._unit_test_support import load_grafyte

grafyte = load_grafyte()


class ConverterTests(unittest.TestCase):
    def test_ensure_vec2f_accepts_vec2_and_tuples(self):
        self.assertEqual(grafyte.ensure_vec2f("pos", grafyte.Vec2(1, 2)), (1.0, 2.0))
        self.assertEqual(grafyte.ensure_vec2f("pos", (1, 2.5)), (1.0, 2.5))

    def test_ensure_vec2f_rejects_invalid_values(self):
        with self.assertRaises(TypeError):
            grafyte.ensure_vec2f("pos", ("x", 2))

    def test_ensure_vec2i_converts_float_like_values(self):
        self.assertEqual(grafyte.ensure_vec2i("size", (12.8, 7.2)), (12, 7))

    def test_ensure_color_clamps_rgb_components(self):
        self.assertEqual(grafyte.ensure_color("color", (-10, 128, 300)), (0, 128, 255))

    def test_ensure_color_rejects_non_integer_components(self):
        with self.assertRaises(TypeError):
            grafyte.ensure_color("color", (1.5, 2, 3))

    def test_ensure_color_normalize_returns_zero_to_one_range(self):
        self.assertEqual(grafyte.ensure_color_normalize("color", (0, 128, 255)), (0.0, 128 / 255, 1.0))


if __name__ == "__main__":
    unittest.main()
