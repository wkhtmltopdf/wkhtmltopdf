#!/bin/bash

echo "%_topdir %(echo $HOME)/rpmbuild" > ~/.rpmmacros
mkdir  ~/rpmbuild
mkdir  ~/rpmbuild/SPECS
mkdir  ~/rpmbuild/SOURCES
mkdir  ~/rpmbuild/BUILD
mkdir  ~/rpmbuild/RPMS
mkdir  ~/rpmbuild/RPMS/noarch
mkdir  ~/rpmbuild/SRPMS

rpmbuild -bb scripts/imgsrv.spec



