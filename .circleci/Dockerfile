FROM fedora:32

# RUN yum update && yum -y install \
#     git \
#     ssh \
#     tar \
#     gzip \
#     ca-certificates \
#     cmake \
#     build-essential \
#     curl \
#     wget

RUN \
dnf makecache && \
  dnf -y update && \
  dnf -y groupinstall "C Development Tools and Libraries" && \
  dnf -y install \
  cmake \
  curl \
  git \
  gzip \
  openssl-devel \
  tar \
  wget \
  which && \
  dnf clean all
