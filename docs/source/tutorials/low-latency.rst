Low Latency
===========

These tables show what performance you can expect from **USB 3.2** Gen 1 (5 Gbps) connection with an OAK camera. XLink chunking was
disabled for these tests (:code:`pipeline.setXLinkChunkSize(0)`).

.. list-table::
   :header-rows: 1

   * - What
     - Resolution
     - FPS
     - FPS set
     - Time-to-Host [ms]
     - Bandwidth
     - Histogram
   * - Color (isp)
     - 1080P
     - 60
     - 60
     - 33
     - 1.5 Gbps
     - `link <https://user-images.githubusercontent.com/18037362/162675407-77882498-5bf1-4af8-b779-9b9e8321af57.png>`__
   * - Color (isp)
     - 4K
     - 28.5
     - 30
     - 150
     - 2.8 Gbps
     - `link <https://user-images.githubusercontent.com/18037362/162675403-f3c5a4c3-1f7d-4acc-a5d5-f5aecff5a66a.png>`__
   * - Mono
     - 720P/800P
     - 120
     - 120
     - 24.5
     - 442/482 Mbps
     - `link <https://user-images.githubusercontent.com/18037362/162675400-b19f1e2c-bee0-4482-a861-39481bf2e35e.png>`__
   * - Mono
     - 400P
     - 120
     - 120
     - 7.5
     - 246 Mbps
     - `link <https://user-images.githubusercontent.com/18037362/162675393-e3fb08fb-0f17-49d0-85d0-31ae7b5af0f9.png>`__

- **Time-to-Host** is measured time between frame timestamp (:code:`imgFrame.getTimestamp()`) and host timestamp when the frame is received (:code:`dai.Clock.now()`).
- **Histogram** shows how much Time-to-Host varies frame to frame. Y axis represents number of frame that occured at that time while the X axis represents microseconds.
- **Bandwidth** is calculated bandwidth required to stream specified frames at specified FPS.

Encoded frames
##############

.. list-table::
   :header-rows: 1

   * - What
     - Resolution
     - FPS
     - FPS set
     - Time-to-Host [ms]
     - Histogram
   * - Color video H.265
     - 4K
     - 28.5
     - 30
     - 210
     - `link <https://user-images.githubusercontent.com/18037362/162675386-a59a58d1-1d5c-4c82-89d8-12882926425b.png>`__
   * - Color video MJPEG
     - 4K
     - 30
     - 30
     - 71
     - `link <https://user-images.githubusercontent.com/18037362/162675384-64842bd6-f5e7-4b40-adf4-39b9c25d1c89.png>`__
   * - Color video H.265
     - 1080P
     - 60
     - 60
     - 42
     - `link <https://user-images.githubusercontent.com/18037362/162675356-c7c86730-63cd-4de8-bad6-4e1484b29ca5.png>`__
   * - Color video MJPEG
     - 1080P
     - 60
     - 60
     - 31
     - `link <https://user-images.githubusercontent.com/18037362/162675371-62aadb93-20f0-4838-9aab-727fb1bfe6f4.png>`__
   * - Mono H.265
     - 800P
     - 60
     - 60
     - 23.5
     - `link <https://user-images.githubusercontent.com/18037362/162675318-92a8541b-424c-49b1-8ec4-6d9e0170410a.png>`__
   * - Mono MJPEG
     - 800P
     - 60
     - 60
     - 22.5
     - `link <https://user-images.githubusercontent.com/18037362/162675328-20afbe8f-4587-4263-8981-919e5a8811c7.png>`__
   * - Mono H.265
     - 400P
     - 120
     - 120
     - 7.5
     - `link <https://user-images.githubusercontent.com/18037362/162675345-dfb4d04a-5d3a-4f84-81e9-dfc091bc00b5.png>`__
   * - Mono MJPEG
     - 400P
     - 120
     - 120
     - 7.5
     - `link <https://user-images.githubusercontent.com/18037362/162675335-2e5a9581-972a-448c-b650-6b6d076a04b8.png>`__

You can also reduce frame latency by using `Zero-Copy <https://github.com/luxonis/depthai-python/tree/tmp_zero_copy>`__
branch of the DepthAI.

.. include::  /includes/footer-short.rst
