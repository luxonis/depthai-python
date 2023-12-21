import depthai.node
import typing

# TODO Extend check

# NOTE Neumožňovat plný přístup ke všem možnostem bind.Data*putQueue, ale nastavit nějak možnosti a věci jako blocking řešit na relevantním nodu

class XLinkHostIn(depthai.node.Node):
    sync = False
    def __node_init__(self, context, stream_name, src_device):
        self.queue = context["running_devices"][src_device]\
                        .getOutputQueue(stream_name)
    def __run__(self) -> typing.Any:
        rv = self.queue.tryGet()
        if rv is None: raise depthai.Abort()
        return rv

# TODO Sjednotit názvy
class XLinkHostOut(depthai.node.Node):
    def __node_init__(self, context, stream_name, dst_device):
        self.queue = context["running_devices"][dst_device]\
                        .getInputQueue(stream_name)
    def __run__(self, input: typing.Any):
        #FIXME I need trySend method on which I can abort
        self.queue.send(input)

def create_xlinks(pipeline, context):
    stream_names = set()
    for dst in pipeline:
        for input_name, put_ref in dst.inputs.items():
            if put_ref == None: continue
            src = put_ref.node
            output_name = put_ref.name

            # Cross-device link
            if dst.device is not None \
            and src.device is not None \
            and src.device != dst.device:
                # Change Dst(Src()) to Dst(Identity(device=None, Src()))
                # Host node should be automatically added at the end of
                # pipeline and should be reprocessed later. Therefore,
                # only Dst -> Identity needs to be processed now. And
                # this already fits one of the later scenarios
                src = depthai.node.Identity(src, 
                                            pipeline=pipeline, 
                                            device=None)
                output_name ,= src.output_desc.values()

            stream_name = str(put_ref)
            while stream_name in stream_names: stream_name += "I"
            stream_names.add(stream_name)

            # Device -> Host
            if dst.device is None \
            and src.device is not None:
                depthai.node.XLinkDevOut(src,
                        stream_name=stream_name,
                        pipeline=pipeline,
                        device=src.device)
                host_in = depthai.node.XLinkHostIn(
                            stream_name=stream_name,
                            src_device=src.device,
                            pipeline=pipeline,
                            device=None)
                dst.link(input_name, host_in)

            # Host -> Device
            if src.device is None \
            and dst.device is not None:
                depthai.node.XLinkHostOut(src,
                        stream_name=stream_name,
                        dst_device=dst.device,
                        pipeline=pipeline,
                        device=None)
                dev_in = depthai.node.XLinkDevIn(
                            stream_name=stream_name,
                            pipeline=pipeline,
                            device=dst.device)
                dst.link(input_name, dev_in)


