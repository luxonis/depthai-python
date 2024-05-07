EncodedFrame
============    

The ``EncodedFrame`` message is an output of the :ref:`VideoEncoder` node. This message is used to represent a frame that has been encoded by the VideoEncoder. It provides various properties and methods for interacting with the encoded frame data.

Structure
#########

The ``EncodedFrame`` class inherits from ``Buffer`` and includes several nested classes and methods for accessing and modifying frame properties. Here are the primary components of the ``EncodedFrame`` class:

- ``FrameType``: Enumerates the types of frames that can be encoded (I, P, B, Unknown).
- ``Profile``: Indicates the encoding profile used (AVC, HEVC, JPEG).

FrameType
---------

The ``FrameType`` class identifies the type of the encoded frame. The available types are:

- **B**: Represents a bi-directional predicted frame.
- **I**: Represents an intra-coded frame.
- **P**: Represents a predicted frame.
- **Unknown**: Used when the frame type cannot be determined.

Profile
-------

The ``Profile`` class specifies the encoding profile used for the frame. The supported profiles are:

- **AVC**: H.264/AVC encoding profile.
- **HEVC**: H.265/HEVC encoding profile.
- **JPEG**: JPEG encoding profile.


Reference
#########

.. tabs::

  .. tab:: Python

    .. autoclass:: depthai.EncodedFrame
      :members:
      :inherited-members:
      :noindex:

  .. tab:: C++

    .. doxygenclass:: dai::EncodedFrame
      :project: depthai-core
      :members:
      :private-members:
      :undoc-members:

.. include::  ../../includes/footer-short.rst
