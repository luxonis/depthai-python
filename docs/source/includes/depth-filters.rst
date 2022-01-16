.. tabs::

    .. tab:: Median

        This is a non-edge preserving `Median filter <https://en.wikipedia.org/wiki/Median_filter>`__, which can be used
        to reduce noise and smoothen the depth map.

        .. doxygenenum:: dai::MedianFilter
            :project: depthai-core
            :no-link:

    .. tab:: Speckle

        **Speckle Filter** is used to reduce the speckle noise. Speckle noise is a region with huge
        variance between neighboring depth pixels, and speckle filter tries to filter this region.

        .. doxygenstruct:: dai::RawStereoDepthConfig::PostProcessing::SpeckleFilter
            :project: depthai-core
            :no-link:

    .. tab:: Temporal

        **Temporal Filter** will take into account valid depth pixels from previous depth maps, so it does
        time averaging to improve depth map.

        .. doxygenstruct:: dai::RawStereoDepthConfig::PostProcessing::TemporalFilter
            :project: depthai-core
            :no-link:

    .. tab:: Spatial

        **Spatial Hole-Filling Filter** will fill invalid depth pixels with valid neighboring depth pixels.
        Radius of neighboring pixels is configurable and so is number of iteration.

        .. doxygenstruct:: dai::RawStereoDepthConfig::PostProcessing::SpatialFilter
            :project: depthai-core
            :no-link:

    .. tab:: Threshold

        **Threshold Filter** filters out all depth pixels outside the configured min/max
        threshold values.

        .. doxygenstruct:: dai::RawStereoDepthConfig::PostProcessing::ThresholdFilter
            :project: depthai-core
            :no-link:

    .. tab:: Decimation

        **Decimation Filter** will downscale the depth map, which means it reduces the depth scene complexity and allows
        other filters to run faster. Setting :code:`decimationFactor` to 2 will downscale 1280x800 depth map to 640x400.

        .. doxygenstruct:: dai::RawStereoDepthConfig::PostProcessing::DecimationFilter
            :project: depthai-core
            :no-link: