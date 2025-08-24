FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    git \
    build-essential \
    cmake \
    make \
    python3 \
    gcc-arm-none-eabi \
    libnewlib-arm-none-eabi \
    patch \
    && rm -rf /var/lib/apt/lists/*

# Allow git to operate on mounted project directories without ownership warnings
RUN git config --global --add safe.directory '*'

WORKDIR /project
