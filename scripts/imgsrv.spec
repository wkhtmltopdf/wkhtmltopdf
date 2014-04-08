
%define name imgsrv

Summary: Image Server
Name: %{name}
Version: 0.0.0.1
Release: 1
License: Spec file is LGPL, binary rpm is gratis but non-distributable
Group: Applications/System
BuildRoot: %{_topdir}/BUILD/%{name}-%{version}-%{release}
Source: %{expand:%%(pwd)}

%description
Image rasterizer for html -> png

%prep
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT/usr/bin
cd $RPM_BUILD_ROOT
cp %{SOURCEURL0}/static-build/centos/app/bin/imgsrv ./usr/bin/

%build

%install

%clean

%files
%defattr(-,root,root)
%doc

/usr/bin/imgsrv

%changelog

