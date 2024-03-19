# -*- coding: utf-8 -*-
import sys

import pytest

import depthai as dai
from depthai_pybind11_tests import xlink_exceptions as m

def test_xlink_error_exception(msg):
    with pytest.raises(dai.XLinkError) as excinfo:
        m.throw_xlink_error()
    assert msg(excinfo.value) == "Yikes!"

def test_xlink_read_error_exception(msg):
    with pytest.raises(dai.XLinkReadError) as excinfo:
        m.throw_xlink_read_error()
    assert msg(excinfo.value) == "Couldn't read data from stream: 'stream_read' (X_LINK_ERROR)"

def test_xlink_write_error_exception(msg):
    with pytest.raises(dai.XLinkWriteError) as excinfo:
        m.throw_xlink_write_error()
    assert msg(excinfo.value) == "Couldn't write data to stream: 'stream_write' (X_LINK_ERROR)"

def test_xlink_error_exception_runtime_error(msg):
    with pytest.raises(RuntimeError) as excinfo:
        m.throw_xlink_error()
    assert msg(excinfo.value) == "Yikes!"

def test_xlink_read_error_exception_xlink_error(msg):
    with pytest.raises(dai.XLinkError) as excinfo:
        m.throw_xlink_read_error()
    assert msg(excinfo.value) == "Couldn't read data from stream: 'stream_read' (X_LINK_ERROR)"

def test_xlink_write_error_exception_xlink_error(msg):
    with pytest.raises(dai.XLinkError) as excinfo:
        m.throw_xlink_write_error()
    assert msg(excinfo.value) == "Couldn't write data to stream: 'stream_write' (X_LINK_ERROR)"
