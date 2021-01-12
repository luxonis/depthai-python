Install requirements on Windows
===============================

This tutorial provides steps to install DepthAI on a Windows 10 system

#. Install Python 3.9 from the Microsoft Store (`direct link <ms-windows-store://pdp/?ProductId=9p7qfqmjrfp7>`_).

    .. image:: /_static/images/tutorials/windows/store.png
      :alt: Windows store

    Once the installation is complete you need to disable the **App Execution Aliases** for
    both **python.exe** and **python3.exe**.

    This can be done by going to start and typing :code:`Manage app` and choose the :code:`Manage app execution aliases` entry.

    .. image:: /_static/images/tutorials/windows/execution_search.png
      :alt: Search execution

    From there you turn off the :code:`App Installer` alias for both **python.exe** and **python3.exe**.

    .. image:: /_static/images/tutorials/windows/execution_aliases.png
      :alt: Execution aliases

#. Install Git. You can find the latest version `here <https://git-scm.com/download/win>`__.

    .. image:: /_static/images/tutorials/windows/git.png
      :alt: Git install

#. Install CMake. You can find the latest version `here <https://cmake.org/download/#latest>`__.

    .. image:: /_static/images/tutorials/windows/cmake.png
      :alt: CMake install

#. Install Microsoft Visual Studio 2017 (direct download link `here <https://download.visualstudio.microsoft.com/download/pr/c5c75dfa-1b29-4419-80f8-bd39aed6bcd9/7ed8fa27575648163e07548ff5667b55b95663a2323e2b2a5f87b16284e481e6/vs_Community.exe>`_)

    The 2017 version is specifically required to build the OpenCV Python package from PyPI

    Note that you only need **Windows 10 SDK**, **Visual C++ for CMake** and **VC++ 2017**, not the whole package (see an image below)

    .. image:: /_static/images/tutorials/windows/vsstudio.png
      :alt: Visual Studio install


#. Open Windows PowerShell

    You can do that by typing **PowerShell** in the searchbar

    .. image:: /_static/images/tutorials/windows/powershell.png
      :alt: Open PowerShell

#. Upgrade Pip

    Latest pip version is required in order to correctly install PyPI requirements.

    To upgrade pip, type the following command in the powershell

    .. code-block:: bash

      python3.exe -m pip install -U pip

    .. image:: /_static/images/tutorials/windows/pip.png
      :alt: Update Pip

    After successful install, you should see pip installed in the latest version

    .. image:: /_static/images/tutorials/windows/pip_success.png
      :alt: Update Pip success

#. Run the DepthAI demo code

    See :ref:`Verify installation` for details

.. include::  /pages/includes/footer-short.rst
