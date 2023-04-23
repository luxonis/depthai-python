FROM ubuntu:22.04

ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y libusb-1.0-0-dev clang python3-opencv python3-pip clang-14 doxygen graphviz cmake git
RUN python3 -m pip install -U pip numpy when-changed

ENV CC clang-14
ENV CXX clang++-14
WORKDIR /app
ADD docs/requirements.txt docs/requirements.txt
RUN python3 -m pip install -r docs/requirements.txt
ADD . .
CMD docs/docker/docker_run.sh