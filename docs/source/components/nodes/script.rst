Script
======

enables users to write scripts that will run on the device.

Logging: node.trace, node.debug, node.warn, node.error, node.critical
//device.setLogLevel to view these logs

You can only send messages as on host - if you want to send custom data do this:
buf = Buffer([num bytes])
buf.getData() <- serialize data into this memoryview

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