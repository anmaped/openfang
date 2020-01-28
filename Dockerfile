FROM ubuntu:16.04

RUN \
  apt update && apt upgrade -y && \
  apt install -y \
  build-essential \
  git \
  autoconf \
  cmake \
  python \
  unzip \
  cpio \
  wget \
  vim \
  locales \
  lzop \
  bc \
  libncurses5-dev

RUN locale-gen --no-purge en_US.UTF-8
RUN git clone https://github.com/anmaped/openfang/ /root/openfang
WORKDIR /root/openfang
