Warp
====

Warp node is used for image warping and dewarping, which can be used to undistort images from wide FOV cameras.
The node can also be used to apply a perspective transform to the image.

Compared to :ref:`ImageManip` node (the `setWarpMesh()` function):

**Warp node** uses underlyting warp HW block (additional `docs here <https://docs.luxonis.com/projects/hardware/en/latest/pages/rvc/rvc2.html>`__),
with no extra resources (SHAVE/cmx cores). HW limitation: **width must be divisible by 16.**

**ImageManip node** combines the power of warp HW block together  the efficiency of CMX memory to achieve higher
throughput (e.g. 4k@30 fps). Scheduling of the HW block is done by SHAVE cores which also do color space conversion, type conversion (YUV420 to NV12), etc.
The downside of using ImageManip node is extra RAM and SHAVE usage.

How to place it
###############

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    warp = pipeline.create(dai.node.Warp)

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto warp = pipeline.create<dai::node::Warp>();

Inputs and Outputs
##################

.. code-block::

             ┌────────────┐
  inputImage │            │    out
  ──────────►│    Warp    ├──────►
             │            │
             └────────────┘

**Message types**

- ``inputImage`` - :ref:`ImgFrame`
- ``out`` - :ref:`ImgFrame`

Usage
#####

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()

    warp = pipeline.create(dai.node.Warp)
    # Create a custom warp mesh
    p1 = dai.Point2f(20, 20)
    p2 = dai.Point2f(460, 20)
    p3 = dai.Point2f(20, 460)
    p4 = dai.Point2f(460, 460)
    warp.setWarpMesh([p1,p2,p3,p4], 2, 2)
    warp.setOutputSize((512,512))
    warp.setMaxOutputFrameSize(512 * 512 * 3)
    # Warp engines to be used (0,1,2)
    warp.setHwIds([1])
    # Warp interpolation mode, choose between BILINEAR, BICUBIC, BYPASS
    warp.setInterpolation(dai.Interpolation.NEAREST_NEIGHBOR)

  .. code-tab:: c++

    dai::Pipeline pipeline;

    auto warp = pipeline.create<dai::node::Warp>();
    // Create a custom warp mesh
    dai::Point2f p1(20, 20);
    dai::Point2f p2(460, 20);
    dai::Point2f p3(20, 460);
    dai::Point2f p4(460, 460);
    warp->setWarpMesh({p1,p2,p3,p4}, 2, 2);
    warp->setOutputSize({512, 512});
    warp->setMaxOutputFrameSize(512 * 512 * 3);
    // Warp engines to be used (0,1,2)
    warp->setHwIds({1});
    // Warp interpolation mode, choose between BILINEAR, BICUBIC, BYPASS
    warp->setInterpolation(dai::node::Warp::Properties::Interpolation::BYPASS);

Examples of functionality
#########################

- :ref:`Warp Mesh`
- :ref:`Interactive Warp Mesh`

Reference
#########

.. tabs::

  .. tab:: Python

    .. autoclass:: depthai.node.Warp
      :members:
      :inherited-members:
      :noindex:

  .. tab:: C++

    .. doxygenclass:: dai::node::Warp
      :project: depthai-core
      :members:
      :private-members:
      :undoc-members:

.. include::  ../../includes/footer-short.rst
