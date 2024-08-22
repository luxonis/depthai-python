PointCloudConfig
================

`PointCloudConfig` is a configuration class used to adjust settings for point cloud generation within the DepthAI ecosystem. It allows users to configure properties such as sparsity and transformation matrices, which are crucial for tailoring the point cloud data to specific application requirements.

Configuration Options
######################

- **Sparsity**: Determines whether the generated point cloud should be sparse. Sparse point clouds may omit points based on certain criteria, such as depth value thresholds, to reduce data volume and processing requirements.
- **Transformation Matrix**: Applies a transformation matrix to the point cloud data, enabling rotations, translations, and scaling to align the point cloud with a world or application-specific coordinate system.

Usage
#####

Configuring `PointCloudConfig` allows for precise control over the generation of point cloud data. Here's an example of how to configure and apply `PointCloudConfig` in a DepthAI application:

.. tabs:: 

    .. code-tab:: py

        import depthai as dai

        # Create pipeline
        pipeline = dai.Pipeline()

        # Create PointCloud node
        pointCloud = pipeline.create(dai.node.PointCloud)

        pointCloud.initialConfig.setSparse(True) # Enable sparse point cloud generation

        # Define a transformation matrix
        transformationMatrix = [
            [1.0, 0.0, 0.0, 0.0],
            [0.0, 1.0, 0.0, 0.0],
            [0.0, 0.0, 1.0, 0.0],
            [0.0, 0.0, 0.0, 1.0]
        ]
        pointCloud.initialConfig.setTransformationMatrix(transformationMatrix) # Apply transformation matrix

        # Further pipeline setup and execution...

    .. code-tab:: c++

        #include "depthai/depthai.hpp"

        int main() {
            // Create pipeline
            dai::Pipeline pipeline;

            // Create PointCloud node
            auto pointCloud = pipeline.create<dai::node::PointCloud>();

            pointCloud->initialConfig.setSparse(true); // Enable sparse point cloud generation

            // Define a transformation matrix
            std::vector<std::vector<float>> transformationMatrix = {
                {1.0, 0.0, 0.0, 0.0},
                {0.0, 1.0, 0.0, 0.0},
                {0.0, 0.0, 1.0, 0.0},
                {0.0, 0.0, 0.0, 1.0}
            };
            pointCloud->initialConfig.setTransformationMatrix(transformationMatrix); // Apply transformation matrix

            // Further pipeline setup and execution...

            return 0;
        }

This example demonstrates initializing `PointCloudConfig`, setting it to generate sparse point clouds, and applying a transformation matrix. This configuration is then applied to a `PointCloud` node within the DepthAI pipeline.

Examples of Functionality
#########################

- **3D Object Localization**: Adjusting the transformation matrix to align point clouds with a known coordinate system for precise object placement.
- **Scene Optimization**: Utilizing sparse point clouds for efficient processing in large-scale or complex scenes.
- **Data Alignment**: Applying transformation matrices for seamless integration of point cloud data with other sensor data or pre-existing 3D models.

Reference
#########

.. tabs::

  .. tab:: Python

    .. autoclass:: depthai.PointCloudConfig
      :members:
      :inherited-members:
      :noindex:

  .. tab:: C++

    .. doxygenclass:: dai::PointCloudConfig
      :project: depthai-core
      :members:
      :private-members:
      :undoc-members:

.. include::  ../../includes/footer-short.rst