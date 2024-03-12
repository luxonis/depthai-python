PointCloud
==========

The PointCloud node is essential for 3D spatial data processing, transforming depth maps into point clouds. This node, combined with visualization libraries such as Open3D, facilitates advanced 3D modeling and analysis, supporting applications in robotics, AR/VR, and environmental scanning.

How to Place it
###############

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    pointCloud = pipeline.create(dai.node.PointCloud)

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto pointCloud = pipeline.create<dai::node::PointCloud>();


Inputs and Outputs
##################

.. code-block::

                 ┌─────────────────────┐
  inputDepth     │                     │
  ──────────────►│                     │        outputPointCloud
  inputConfig    │     PointCloud      ├────────────────►
  ──────────────►│                     │
                 │                     │        passthroughDepth
                 │                     ├────────────────►
                 └─────────────────────┘

**Message types**

- :code:`inputDepth` - :ref:`ImgFrame`
- :code:`inputConfig` - :ref:`PointCloudConfig`
- :code:`outputPointCloud` - :ref:`PointCloudData`
- :code:`passthroughDepth` - :ref:`ImgFrame` (passthrough input depth map)


Example visualization with Open3D
#################################

.. tabs::

  .. code-tab:: py
    
    import open3d as o3d
    import numpy as np
    import depthai as dai

    pcd = o3d.geometry.PointCloud()
    vis = o3d.visualization.VisualizerWithKeyCallback()
    vis.create_window()

    with dai.Device(pipeline) as device:
        coordinateFrame = o3d.geometry.TriangleMesh.create_coordinate_frame(size=1000, origin=[0,0,0])
        vis.add_geometry(coordinateFrame)
        
        while device.isPipelineRunning():
            inMessage = q.get()
            inColor = inMessage["rgb"]
            inPointCloud = inMessage["pcl"]
            cvColorFrame = inColor.getCvFrame()
          
            if inPointCloud:
                points = inPointCloud.getPoints().astype(np.float64)
                pcd.points = o3d.utility.Vector3dVector(points)
                colors = (cvRGBFrame.reshape(-1, 3) / 255.0).astype(np.float64)
                pcd.colors = o3d.utility.Vector3dVector(colors)
                vis.update_geometry(pcd)

            vis.poll_events()
            vis.update_renderer()

        vis.destroy_window()


  .. code-tab:: c++

    #include <iostream>
    #include <open3d/Open3D.h>
    #include <depthai/depthai.hpp>

    int main() {
        auto viewer = std::make_unique<pcl::visualization::PCLVisualizer>("Cloud Viewer");
        viewer->addPointCloud<pcl::PointXYZ>(cloud, "cloud");

        dai::Device device(pipeline);

        auto q = device.getOutputQueue("out", 8, false);
        auto qDepth = device.getOutputQueue("depth", 8, false);

        while(true) {
            std::cout << "Waiting for data" << std::endl;
            auto depthImg = qDepth->get<dai::ImgFrame>();
            auto pclMsg = q->get<dai::PointCloudData>();
            
            if(!pclMsg) {
                std::cout << "No data" << std::endl;
                continue;
            }

            auto frame = depthImg->getCvFrame();
            frame.convertTo(frame, CV_8UC1, 255 / depth->initialConfig.getMaxDisparity());

            if(pclMsg->getPoints().empty()) {
                std::cout << "Empty point cloud" << std::endl;
                continue;
            }

            pcl::PointCloud<pcl::PointXYZ>::Ptr cloud = pclMsg->getPclData();
            viewer->updatePointCloud(cloud, "cloud");
            
            viewer->spinOnce(10);

            if(viewer->wasStopped()) {
                break;
            }
        }


Examples using PointCloud
#########################

- :ref:`PointCloud Visualization`
- :ref:`PointCloud Control`


Reference
#########

.. tabs::

  .. tab:: Python

    .. autoclass:: depthai.node.PointCloud
      :members:
      :inherited-members:
      :noindex:

  .. tab:: C++

    .. doxygenclass:: dai::node::PointCloud
      :project: depthai-core
      :members:
      :private-members:
      :undoc-members:


.. include::  ../../includes/footer-short.rst
