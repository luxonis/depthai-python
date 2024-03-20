# -*- coding: utf-8 -*-
import sys

import pytest

import depthai as dai
from pathlib import Path

def test_dai_path_conversion_positive():
    # Can raise RuntimeError but not TypeError (eg if string/Path weren't accepted as parameters)

    with pytest.raises(Exception) as excinfo:
        a = dai.AssetManager()
        nn = dai.Pipeline().create(dai.node.NeuralNetwork)

        string_path = 'test.txt'
        pathlib_path = Path('pathlib.txt')

        # AssetManager
        a.set('p1', string_path)
        a.set('p2', pathlib_path)

        # NN
        nn.setBlobPath('p1', string_path)
        nn.setBlobPath('p2', pathlib_path)
        nn.setBlob('p1', string_path)
        nn.setBlob('p2', pathlib_path)

    assert(excinfo is not RuntimeError)


def test_dai_path_conversion_negative():
    # Must raise TypeError, to indicate that given argument wasn't converted to Path

    a = dai.AssetManager()
    nn = dai.Pipeline().create(dai.node.NeuralNetwork)
    info = dai.DeviceInfo() # str representable

    with pytest.raises(TypeError) as excinfo:
        a.set('invalid', info)

    with pytest.raises(TypeError) as excinfo:
        nn.setBlobPath(info)

    with pytest.raises(TypeError) as excinfo:
        nn.setBlob(info)

