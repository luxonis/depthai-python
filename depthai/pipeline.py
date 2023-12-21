import depthai.global_state as global_state

class Pipeline(list):
    def __enter__(self):
        self.old = global_state.pipeline
        global_state.pipeline = self
    def __exit__(self, *args):
        assert global_state.pipeline == self
        global_state.pipeline = self.old

class NoPipeline:
    def __getattribute__(self):
        raise AttributeError("No pipeline is defined. "
                 "Perhaps you forgot to call `with depthai.Pipeline():`?")
