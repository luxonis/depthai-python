import unittest
from depthai.type_checker import check_type, check_pipeline
import depthai.global_state
import depthai
import typing

class B: pass
class C(B): pass

class TestCheckType(unittest.TestCase):
    def test_int(self):
        with self.assertNoLogs(level="WARNING"):
            check_type(6, int)

    def test_not_int(self):
        with self.assertNoLogs(level="WARNING"):
            with self.assertRaises(TypeError):
                check_type("abc", int)

    def test_union(self):
        with self.assertNoLogs(level="WARNING"):
            check_type("abc", int | str)

    def test_not_union(self):
        with self.assertNoLogs(level="WARNING"):
            with self.assertRaises(TypeError):
                check_type(None, int | str)

    def test_list(self):
        with self.assertNoLogs(level="WARNING"):
            check_type(["abc"], list[str])

    def test_not_list(self):
        with self.assertNoLogs(level="WARNING"):
            with self.assertRaises(TypeError):
                check_type("abc", list[str])

    def test_none(self):
        import types
        with self.assertNoLogs(level="WARNING"):
            check_type(None, types.NoneType)

    def test_user_class(self):
        with self.assertNoLogs(level="WARNING"):
            check_type(B(), B)

    def test_not_user_class(self):
        with self.assertNoLogs(level="WARNING"):
            with self.assertRaises(TypeError):
                check_type(B(), C)

    def test_inheritance(self):
        with self.assertNoLogs(level="WARNING"):
            check_type(C(), B)

    def test_warning(self):
        class X: pass
        with self.assertLogs(level="WARNING"):
            check_type(X(), X)

class IntSrc(depthai.Node):
    output_desc = {"output": int}

class IntDst(depthai.Node):
    input_desc = {"input": int}

class StrDst(depthai.Node):
    input_desc = {"input": str}

class AnyDst(depthai.Node):
    input_desc = {"input": typing.Any}

T = typing.TypeVar("T")
class GenericPipe(depthai.Node):
    input_desc = {"input": T}
    output_desc = {"output": T}

class GenericSrc(depthai.Node):
    output_desc = {"output": T}

class TestCheckPipeline(unittest.TestCase):

    def test_fit(self):
        with self.assertNoLogs(level="WARNING"):
            with (p := depthai.Pipeline()):
                IntDst(IntSrc())
            check_pipeline(depthai.global_state.pipeline)

    def test_not_fit(self):
        with self.assertNoLogs(level="WARNING"):
            with self.assertRaises(TypeError):
                with (p := depthai.Pipeline()):
                    StrDst(IntSrc())
                check_pipeline(depthai.global_state.pipeline)

    def test_any(self):
        with self.assertNoLogs(level="WARNING"):
            with (p := depthai.Pipeline()):
                AnyDst(IntSrc())
            check_pipeline(depthai.global_state.pipeline)

    def test_generic(self):
        with self.assertNoLogs(level="WARNING"):
            with (p := depthai.Pipeline()):
                IntDst(GenericPipe(IntSrc()))
            check_pipeline(depthai.global_state.pipeline)

    def test_not_generic(self):
        with self.assertNoLogs(level="WARNING"):
            with self.assertRaises(TypeError):
                with (p := depthai.Pipeline()):
                    StrDst(GenericPipe(IntSrc()))
                check_pipeline(depthai.global_state.pipeline)
