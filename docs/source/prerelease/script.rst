Script
======

Scripting node enables users to write scripts that will run on the device.

How to place it
###############

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    script = pipeline.create(dai.node.Script)

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto script = pipeline.create<dai::node::NeuralNetwork>();

Demo
####

.. code-block: python

    feed_manip_config_script = pipeline.create(dai.node.Script)
    feed_manip_config_script.setScriptData("""
    score, bb_cx, bb_cy, bb_w, rect_cx, rect_cy, rect_w, rotation = node.io['in'].get().getLayerFp16("result")
    rr = RotatedRect()
    rr.center.x    = rect_cx
    rr.center.y    = rect_cy
    rr.size.width  = rect_w
    rr.size.height = rect_w
    rr.angle       = rotation
    cfg = ImageManipConfig()
    cfg.setCropRotatedRect(rr, True)
    cfg.setResize(224, 224)
    node.io['out'].send(cfg)
    """)
    pp_nn.out.link(feed_manip_config_script.inputs['in'])
    feed_manip_config_script.outputs['out'].link(pre_lm_manip.inputConfig)

.. code-block: python

    script = pipeline.create(dai.node.Script)
    script.setScriptData("""
    # Logging to the host can be enable with
    # node.trace, node.debug, node.warn, node.error, node.critical
    node.trace("Hello World")
    """)

    # After initializing the device, enable log levels
    device.setLogLevel(dai.LogLevel.WARN)
    device.setLogOutputLevel(dai.LogLevel.WARN)

.. include::  ../../includes/footer-short.rst
