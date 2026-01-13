# syntax=docker/dockerfile:1.0-experimental
# The syntax=docker/dockerfile:1.0-experimental is to allow ssh forwarding to clone private repositories
FROM ubuntu:24.04

LABEL authors="Lichens-CPP contributors"

SHELL ["/bin/bash", "-c"]

RUN apt-get update && apt-get install -y tzdata

RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        bash-completion \
        build-essential \
        ca-certificates \
        clang-format \
        cmake \
        cmake-curses-gui \
        gdb \
        g++ \
        clang \
        llvm \
        lld \
        git \
        jq \
        libssl-dev \
        ninja-build \
        openssl \
        just \
        libfmt-dev \
        libmosquitto-dev \
        libpaho-mqtt-dev \
        libpaho-mqttpp-dev \
        nlohmann-json3-dev \
        mosquitto-clients \
    && rm -rf /var/lib/apt/lists/*

RUN apt update && apt upgrade -y && \
    rm -rf /var/lib/apt/lists/*

# Add a user with same UID and GID as your host system
# (copied from https://code.visualstudio.com/remote/advancedcontainers/add-nonroot-user#_creating-a-nonroot-user)
ARG USERNAME=devuser
ARG USER_UID=1010
ARG USER_GID=$USER_UID

RUN groupadd --gid $USER_GID $USERNAME \
    && useradd -s /bin/bash --uid $USER_UID --gid $USER_GID -m $USERNAME \
    && apt-get update \
    && apt-get install -y sudo \
    && mkdir -p /etc/sudoers.d \
    && echo "$USERNAME ALL=(ALL) NOPASSWD: ALL" > /etc/sudoers.d/$USERNAME \
    && chmod 0440 /etc/sudoers.d/$USERNAME \
    && usermod -aG sudo $USERNAME

USER $USERNAME

WORKDIR /workspace

CMD ["/bin/bash"]
