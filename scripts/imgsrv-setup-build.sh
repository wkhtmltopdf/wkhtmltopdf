#!/bin/bash

# Create build environment for headless image server (fonts, g++, xvfb)

yum install -y python-multiprocessing gcc44-c++ libX11-devel libXext-devel libXrender-devel xz xorg-x11-fonts-75dpi.noarch
yum install -y xorg-x11-fonts-100dpi.noarch xorg-x11-xfs xorg-x11-xfs-utils xorg-x11-server-Xvfb rpm-build cabextract
yum install -y Xorg

# download and build ms core fonts
wget http://corefonts.sourceforge.net/msttcorefonts-2.5-1.spec -O msttcorefonts-2.5-1.spec
echo "%_topdir %(echo $HOME)/rpmbuild" > ~/.rpmmacros
mkdir ~/rpmbuild
mkdir ~/rpmbuild/BUILD
mkdir ~/rpmbuild/RPMS
mkdir ~/rpmbuild/RPMS/noarch
rpmbuild -bb msttcorefonts-2.5-1.spec
rpm -ivh ~/rpmbuild/RPMS/noarch/msttcorefonts-2.5-1.noarch.rpm 
rm msttcorefonts-2.5-1.spec

fc-cache
ln -s /usr/bin/g++44 /usr/bin/g++

./build.py centos
echo
echo "To build from source, execute build.sh within the build directory"
echo
