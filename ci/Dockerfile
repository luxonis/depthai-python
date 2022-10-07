FROM python:3.9-bullseye

RUN apt-get update && apt-get install -y wget build-essential cmake pkg-config libjpeg-dev libtiff5-dev libavcodec-dev libavformat-dev libswscale-dev libv4l-dev libxvidcore-dev libx264-dev libgtk2.0-dev libgtk-3-dev libatlas-base-dev gfortran git libopencv-dev

ADD ci/docker_dependencies.sh .
RUN ./docker_dependencies.sh

RUN pip install -U pip && pip install --extra-index-url https://www.piwheels.org/simple/ --prefer-binary opencv-python

# Copy over the files
COPY . /depthai-python

# Install C++ library
RUN cmake -S /depthai-python/depthai-core -B /build -D CMAKE_BUILD_TYPE=Release -D BUILD_SHARED_LIBS=ON -D CMAKE_INSTALL_PREFIX=/usr/local
RUN cmake --build /build --parallel 4 --config Relase --target install

# Install Python library
RUN cd /depthai-python && python3 -m pip install .