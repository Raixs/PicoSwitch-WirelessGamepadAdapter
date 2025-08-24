FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    git \
    build-essential \
    cmake \
    make \
    gcc-arm-none-eabi \
    libnewlib-arm-none-eabi \
    && rm -rf /var/lib/apt/lists/*

# Allow git to operate on the mounted project directory without ownership warnings
RUN git config --global --add safe.directory /project

WORKDIR /project
