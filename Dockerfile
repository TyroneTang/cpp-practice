FROM ubuntu:24.04 AS base

ENV DEBIAN_FRONTEND=noninteractive

ENV LANG=C.UTF-8
ENV LC_ALL=C.UTF-8
ENV HOME=/workspace

SHELL ["/bin/bash", "-c"]

# ---------- DEV UTILS --------------- #
RUN --mount=target=/var/lib/apt/lists/cpp_base,type=cache,sharing=locked \
    --mount=target=/var/cache/apt/cpp_base,type=cache,sharing=locked \
    apt update -y && \
    apt install -y \
    git \
    bash-completion \
    iproute2 \
    wget \
    nano \
    iputils-ping \
    openssh-client \
    curl \
    rsync \
    vim \
    build-essential \
    locales \
    software-properties-common \
    gnupg \
    unzip \
    zip \
    pkg-config \
    sudo


# ---------- LLVM/Clang 21 --------------- #
RUN wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | gpg --dearmor > /etc/apt/trusted.gpg.d/llvm-archive-keyring.gpg
RUN echo "deb [signed-by=/etc/apt/trusted.gpg.d/llvm-archive-keyring.gpg] http://apt.llvm.org/noble/ llvm-toolchain-noble-21 main" > /etc/apt/sources.list.d/llvm.list

RUN --mount=target=/var/lib/apt/lists/cpp_base,type=cache,sharing=locked \
    --mount=target=/var/cache/apt/cpp_base,type=cache,sharing=locked \
    apt update -y && \
    apt install -y \
    clang-21 \
    clangd-21 \
    lldb-21 \
    lld-21 \
    clang-tidy-21 \
    libx11-dev \
    libxrandr-dev

RUN update-alternatives --install /usr/bin/clang clang /usr/bin/clang-21 100
RUN update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-21 100
RUN update-alternatives --install /usr/bin/clangd clangd /usr/bin/clangd-21 100
RUN update-alternatives --install /usr/bin/lldb lldb /usr/bin/lldb-21 100
RUN update-alternatives --install /usr/bin/lld lld /usr/bin/lld-21 100
RUN update-alternatives --install /usr/bin/clang-tidy clang-tidy /usr/bin/clang-tidy-21 100

# ---------- CMake --------------- #
RUN wget -O /tmp/cmake-install.sh https://github.com/Kitware/CMake/releases/download/v4.3.2/cmake-4.3.2-linux-x86_64.sh && \
    chmod +x /tmp/cmake-install.sh && \
    mkdir -p /opt/cmake-4.3.2 && \
    /tmp/cmake-install.sh --prefix=/opt/cmake-4.3.2 --skip-license && \
    rm /tmp/cmake-install.sh

RUN update-alternatives --install /usr/bin/cmake cmake /opt/cmake-4.3.2/bin/cmake 100
RUN update-alternatives --install /usr/bin/ctest ctest /opt/cmake-4.3.2/bin/ctest 100
RUN update-alternatives --install /usr/bin/cpack cpack /opt/cmake-4.3.2/bin/cpack 100

ENV PATH="/opt/cmake-4.3.2/bin:${PATH}"

# ---------- Ninja --------------- #
RUN wget -O /tmp/ninja-linux.zip https://github.com/ninja-build/ninja/releases/download/v1.13.2/ninja-linux.zip && \
    unzip /tmp/ninja-linux.zip -d /usr/bin && \
    chmod +x /usr/bin/ninja && \
    rm /tmp/ninja-linux.zip

# ---------- Workspace / Non-root user --------------- #
# Rename pre-existing ubuntu user (UID 1000) to devuser, with /workspace as home
RUN usermod -l devuser ubuntu && \
    groupmod -n devuser ubuntu && \
    usermod -d /workspace devuser && \
    usermod -s /bin/bash devuser && \
    rm -rf /home/ubuntu

# Create workspace dirs and give ownership
RUN mkdir -p /workspace/project /workspace/.cache /workspace/.local /workspace/.config /workspace/Documents/oh_my_posh && \
    chown -R devuser:devuser /workspace

RUN echo "devuser ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers

# ---------- vcpkg --------------- #
# Pre-create vcpkg dir so devuser can write into it
RUN mkdir -p /opt/vcpkg && chown -R devuser:devuser /opt/vcpkg

USER devuser

RUN git clone https://github.com/microsoft/vcpkg.git /opt/vcpkg && \
    /opt/vcpkg/bootstrap-vcpkg.sh

ENV VCPKG_ROOT=/opt/vcpkg
ENV PATH="$VCPKG_ROOT:$PATH"

WORKDIR /workspace/project
