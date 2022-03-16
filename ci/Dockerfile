FROM python:3.9-bullseye

RUN apt-get update && apt-get install -y wget build-essential cmake pkg-config libjpeg-dev libtiff5-dev libavcodec-dev libavformat-dev libswscale-dev libv4l-dev libxvidcore-dev libx264-dev libgtk2.0-dev libgtk-3-dev libatlas-base-dev gfortran git

ADD ci/docker_dependencies.sh .
RUN ./docker_dependencies.sh
RUN wget https://github.com/libusb/libusb/releases/download/v1.0.24/libusb-1.0.24.tar.bz2
RUN tar xf libusb-1.0.24.tar.bz2
RUN cd libusb-1.0.24 && \
    ./configure --disable-udev && \
    make -j && make install


RUN pip install -U pip && pip install --extra-index-url https://www.piwheels.org/simple/ --prefer-binary opencv-python

COPY . /depthai-python
RUN cd /depthai-python && python3 -m pip install .