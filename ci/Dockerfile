FROM python:3.9-buster

RUN apt-get update && apt-get install -y wget build-essential cmake pkg-config libjpeg-dev libtiff5-dev libavcodec-dev libavformat-dev libswscale-dev libv4l-dev libxvidcore-dev libx264-dev libgtk2.0-dev libgtk-3-dev libatlas-base-dev gfortran git

ADD ci/docker_dependencies.sh .
# RUN wget http://docs.luxonis.com/install_dependencies.sh && chmod +x install_dependencies.sh && ./install_dependencies.sh
RUN ./docker_dependencies.sh  # for development only, should be removed before merge
RUN wget https://github.com/libusb/libusb/releases/download/v1.0.24/libusb-1.0.24.tar.bz2
RUN tar xf libusb-1.0.24.tar.bz2
RUN cd libusb-1.0.24 && \
    ./configure --disable-udev && \
    make -j && make install


RUN pip install --extra-index-url https://www.piwheels.org/simple/ opencv-python

COPY . /depthai-python
RUN mkdir -p depthai-python/build
RUN cd /depthai-python && \
    cd build && \
    cmake .. && \
    make
ENV PYTHONPATH=/depthai-python/build