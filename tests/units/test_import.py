from __future__ import annotations

import unittest


class ImportTests(unittest.TestCase):
    def test_top_level_import_exposes_expected_symbols(self):
        import grafyte
        from grafyte import Application, Anchor, Direction, InputTrigger, Key, Vec2

        self.assertIs(grafyte.Application, Application)
        self.assertIs(grafyte.Anchor, Anchor)
        self.assertIs(grafyte.Direction, Direction)
        self.assertIs(grafyte.InputTrigger, InputTrigger)
        self.assertIs(grafyte.Key, Key)
        self.assertIs(grafyte.Vec2, Vec2)

    def test_module_reports_a_version(self):
        import grafyte

        self.assertIsInstance(grafyte.__version__, str)
        self.assertNotEqual(grafyte.__version__.strip(), "")


if __name__ == "__main__":
    unittest.main()
