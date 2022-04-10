Low Latency
===========

These tables show what performance you can expect from **USB 3** (5 Gbps) connection with an OAK camera. XLink chunking was
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
     - `link <https://google.com>`__
   * - Color (isp)
     - 4K
     - 28.5
     - 30
     - 150
     - 2.8 Gbps
     - `link <https://google.com>`__
   * - Mono
     - 720P/800P
     - 120
     - 120
     - 24.5
     - 442/482 Mbps
     - `link <https://google.com>`__
   * - Mono
     - 400P
     - 120
     - 120
     - 7.5
     - 246 Mbps
     - `link <https://google.com>`__

- **Time-to-Host** is measured time between frame timestamp (:code:`imgFrame.getTimestamp()`) and host timestamp when the frame is received (:code:`dai.Clock.now()`).
- **Histogram** shows how much Time-to-Host varies frame to frame.
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
     - 29.5*
     - 30
     - 210
     - `link <https://google.com>`__
   * - Color video MJPEG
     - 4K
     - 30
     - 30
     - 71
     - `link <https://google.com>`__
   * - Color video H.265
     - 1080P
     - 60
     - 60
     - 42
     - `link <https://google.com>`__
   * - Color video MJPEG
     - 1080P
     - 60
     - 60
     - 31
     - `link <https://google.com>`__
   * - Mono H.265
     - 800P
     - 60
     - 60
     - 23.5
     - `link <https://google.com>`__
   * - Mono MJPEG
     - 800P
     - 60
     - 60
     - 22.5
     - `link <https://google.com>`__
   * - Mono H.265
     - 400P
     - 120
     - 120
     - 7.5
     - `link <https://google.com>`__
   * - Mono MJPEG
     - 400P
     - 120
     - 120
     - 7.5
     - `link <https://google.com>`__

.. include::  /includes/footer-short.rst
