import unittest


class TestDepthAI(unittest.TestCase):
    def test_import(self):
        try:
            import depthai
        except ImportError:
            print("Unable to import DepthAI module!")
            raise

    def test_init_device(self):
        import depthai
        try:
            device = depthai.Device("", False)
            del device
        except:
            print("Unable to init DepthAI device!")
            raise


if __name__ == "__main__":
    unittest.main()
