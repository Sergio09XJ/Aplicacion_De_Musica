FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

# Instalar compilador, CMake, Qt6, libpqxx y Doxygen
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    pkg-config \
    libpq-dev \
    libpqxx-dev \
    qt6-base-dev \
    qt6-declarative-dev \
    doxygen \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /workspace

