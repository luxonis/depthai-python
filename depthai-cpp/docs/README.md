# Idea
The idea of these files and types is to create the high-level representation of nnet (Neural Network) result parser.  

We propose a model where *NN inference result* consists of *tensors*. *Tensor* is divided into *entries*. And *entry* is divided into *properties*.  
**NN inference result** - all the information that neural network returns.  
**Tensor** - multidimensional array which has pre-defined dimensions.  
Tensor contains data about detection/recognition/... . Tensor has **entries**. It's the result of the objects.  
Entry has **properties**, it's the characteristics of the object. It might be x coordinate, confidence, ...  

All properties has specific type (`uint8` (unsigned integer 8 bit) or `f16` (floating point 16 bit), or other).  

# Neural network result iteration
```
p = depthai.create_pipeline(...)
nnet_packets, data_packets = p.get_available_nnet_and_data_packets()
```
Method `get_available_nnet_and_data_packets` returns the tuple with lists of NNet packets and list of data packets.  
Data packet contains raw bitmaps of the streams (*left*, *right*, etc.). You can access data through **numpy array**.  
NNet packet contains access to the same **numpy array**, but it has additional interfaces for better access to the semantic data of the results.  
Output of the neural network might have multiple *tensors* (multi-dimensional arrays).  

```
for nnet_packet in nnet_packets:
    # iterate through tensors
    for t in nnet_packet.tensors():
        pass

# TODO: required ?
    # access separately
    t_num = nnet_packet.get_tensors_number()  # number of tensors
    t0  = nnet_packet.get_tensor(0)           # get tensor by index
    t0_ = nnet_packet.get_tensor('out')       # get tensor by name
```

**t0, t0_** - numpy arrays  

To access the entries:
```
    for e in nnet_packet.entries():
        # each entry has property
        p0_ = e['id']  # access by name
        p0  = e[0][0]  # access by index (index of tensor, index of property)
```

The idea of the entry is to provide all the information from NN inference with one interface.  
For example, if CNN detects the objects and returns the bounding boxes, then the entry it's the object storing computed information, and you can access `e['confidence']` to get the confidence of the detection. All properties should be described in **JSON** file. It's path should be specified in `create_pipeline`.  
