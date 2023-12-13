import depthai_bind
import queue
import collections

class Abort(Exception): pass

def init_host_node(node):
    def method(m):
        setattr(node, m.__name__, m.__get__(node, type(node)))


    node.output_queues = collections.defaultdict(list)
    node.input_queues = {name: queue.Queue() for name in node.inputs.keys()}
    for name, output_ref in node.inputs.items():
        if output_ref is None: continue
        if not hasattr(output_ref.node, "output_queues"):
            init_host_node(output_ref.node)
        output_ref.node.output_queues[output_ref.name].append(
                node.PutRef(node, name))

    def retrieve_queue_property(node, input_name, property):
        for name in (input_name, "*"):
            try: return node.queues[name + "-" + property]
            except KeyError: continue
        raise KeyError(f'For input "{input_name}" of node {node} ' 
                         f'queue property "{property}" is not available.')
    @method
    def is_queue_blocking(self, input_name):
        return retrieve_queue_property(self, input_name, "blocking")
    @method
    def get_queue_size(self, input_name):
        return retrieve_queue_property(self, input_name, "size")

    @method
    def is_blocked(self):
        return any(input_ref.node.is_queue_blocking(input_ref.name)
                   and input_ref.node.input_queues[input_ref.name].qsize()
                        >= input_ref.node.get_queue_size(input_ref.name)
                for outputs in self.output_queues.values()
                for input_ref in outputs)

    @method 
    def dispatch(self):
        if self.is_blocked(): return
        args = [None if queue.empty() else queue.queue[0]
                   for queue in self.input_queues.values()]
        if self.sync and any(arg is None for arg in args):
            return

        try:  
            output = self.__run__(*args)
        except Abort: 
            return
        for queue, arg in zip(self.input_queues.values(), args):
            if arg is not None: queue.get()

        if len(self.output_queues) == 0:
            assert output == None, \
                    f"Node {self} has no outputs and didn't return None"
            return
        if len(self.output_queues) == 1: output = (output,)
        for value, queues in zip(output, self.output_queues.values(), 
                                 strict=True):
            for input_ref in queues:
                input_ref.node.input_queues[input_ref.name].put(value)

def simple_main_loop(pipeline):
    while True:
        for node in pipeline:
            node.dispatch()

def run(pipeline, main_loop = simple_main_loop):
    device_pipeline = []
    host_pipeline = []
    for node in pipeline:
        match node.side:
            case "device": device_pipeline.append(node)
            case "host":   
                init_host_node(node)
                host_pipeline.append(node)
            case _:        
                raise ValueError(f"Attribute side of {node}"
                                 ' should be one of "device" or "host".')
    if len(device_pipeline) == 0:
        main_loop(host_pipeline)
        return
    with depthai_bind.Device(device_pipeline) as device:
        # TODO Bind XLinks
        main_loop(host_pipeline)


