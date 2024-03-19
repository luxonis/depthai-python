Crash report
============

In case of a firmware crash, OAK cameras will automatically generate a crash report and store it in the device.
The crash report contains information about the crash, such as the stack trace, the device's configuration,
and the device's state at the time of the crash. The crash report can be read from the device and sent to Luxonis for debugging purposes.


Demo
####

In case a crash report was found on the device, this example will read it and save it to a json file:

.. code-block:: bash

    > python crash_report.py
    Crash dump found on your device!
    Saved to crashDump_0_184430102163DB0F00_3575b77f20e796b4e79953bf3d2ba22f0416ee8b.json
    Please report to developers!

Please **send the crash reports** together `with an MRE <https://docs.luxonis.com/en/latest/pages/support/#depthai-issue>`__ (DepthAI issue)
to our `Discuss Forum <https://discuss.luxonis.com/>`__. Thank you!

Setup
#####

.. include::  /includes/install_from_pypi.rst

Source code
###########

.. tabs::

    .. tab:: Python

        Also `available on GitHub <https://github.com/luxonis/depthai-python/blob/main/examples/CrashReport/crash_report.py>`__

        .. literalinclude:: ../../../../examples/CrashReport/crash_report.py
           :language: python
           :linenos:

    .. tab:: C++

        Also `available on GitHub <https://github.com/luxonis/depthai-core/blob/main/examples/CrashReport/crash_report.cpp>`__

        .. literalinclude:: ../../../../depthai-core/examples/CrashReport/crash_report.cpp
           :language: cpp
           :linenos:

.. include::  /includes/footer-short.rst
