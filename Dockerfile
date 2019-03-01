FROM ubuntu:16.04

RUN \
  apt update && apt upgrade -y && \
  apt install -y \
  build-essential \
  git \
  gcc-mips-linux-gnu \
  autoconf \
  libtool \
  cmake \
  python \
  unzip \
  cpio \
  wget \
  nano \
  locales \
  gawk \
  libgtk2.0-dev \
  lzop \
  bc

RUN locale-gen --no-purge en_US.UTF-8
RUN git clone https://github.com/anmaped/openfang/ /root/openfang
WORKDIR /root/openfang
