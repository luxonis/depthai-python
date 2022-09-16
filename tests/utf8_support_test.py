# -*- coding: utf-8 -*-
import sys

import pytest

import depthai as dai

def test_utf8_path_asset_manager(tmp_path):

    FILE_CONTENTS="Hello World 👍"

    tmp_sneaky_file_dir = tmp_path / "UTF-8 dir 🙈🙉🙉"
    tmp_sneaky_file_dir.mkdir()
    tmp_sneaky_file_path = tmp_sneaky_file_dir / "sneaky 👍 čćšđžĆČLpđšlčk file.txt"
    tmp_sneaky_file_path.write_bytes(FILE_CONTENTS.encode("utf-8"))

    assetManager = dai.AssetManager()
    asset = assetManager.set('test', tmp_sneaky_file_path)
    assert(bytes(asset.data) == FILE_CONTENTS.encode("utf-8"))
