import unittest
from pathlib import Path


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

    def test_create_pipeline(self):
        import depthai
        device = depthai.Device("", False)
        try:
            pipeline = device.create_pipeline({
                'streams': ['previewout'],
                'ai': {
                    'blob_file': str(Path('./mobilenet-ssd.blob').resolve().absolute()),
                    'blob_file_config': str(Path('./mobilenet-ssd.json').resolve().absolute())
                }
            })
            assert pipeline is not None, "Unable to create DepthAI pipeline"
            del pipeline
        except:
            print("Unable to create pipeline!")
            raise


if __name__ == "__main__":
    unittest.main()
