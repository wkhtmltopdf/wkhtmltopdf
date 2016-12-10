#!/bin/bash
cat <<'REPO' > /etc/yum.repos.d/slc6.repo
[slc6-scl]
name=Scientific Linux CERN (SLC6) - SCL addons
baseurl=http://linuxsoft.cern.ch/cern/scl/slc6X/$basearch/yum/scl/
gpgkey=http://linuxsoft.cern.ch/cern/scl/RPM-GPG-KEY-cern
gpgcheck=1
enabled=1
REPO
yum clean all
yum update -y
yum install -y \
    scl-utils \
    devtoolset-3-gcc-c++ \
    python27 \
    git19 \
    ruby200 \
    perl \
    git \
    make \
    gzip \
    diffutils \
    gperf \
    bison \
    flex \
    zlib-devel \
    openssl-devel \
    freetype-devel \
    fontconfig-devel \
    libX11-devel \
    libXrender-devel \
    libXext-devel
echo "source scl_source enable devtoolset-3 python27 git19 ruby200" > /etc/profile.d/scl_env.sh
