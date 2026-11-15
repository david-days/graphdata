Name:           graphdata
Version:        1.0.0
Release:        1%{?dist}
Summary:        Efficient implementation for common graph data operations

License:        MIT
URL:            https://github.com/david-days/graphdata
# GitHub tag archive (includes LICENSE; CMake at repo root).
# Prefer this over the CPack *-src.tar.gz until that tarball is cleaned up.
# The #/ fragment sets the local SOURCES filename for rpmbuild/spectool.
Source0:        https://github.com/david-days/graphdata/archive/refs/tags/v%{version}.tar.gz#/graphdata-%{version}.tar.gz

BuildRequires:  cmake >= 3.25
BuildRequires:  gcc
BuildRequires:  ninja-build

%description
Native C library providing general graph data structures and operations
(LINKED, HASHED, ARRAY, SHARED_MEM, SHARED_MMAP) behind a common graphops_t
API.

%package devel
Summary:        Development files for %{name}
Requires:       %{name}%{?_isa} = %{version}-%{release}

%description devel
Headers and pkg-config file for building against %{name}.

%prep
%autosetup -n graphdata-%{version}

%build
# Build only the library target (skip tests/examples).
%cmake
%cmake_build --target graphData

%install
%cmake_install

# First COPR pass: no %%check. Add check-devel + ctest once the package builds.

%files
%license LICENSE
%doc README.md
%{_libdir}/libgraphData.so.*

%files devel
%{_includedir}/graphData.h
%{_includedir}/graphInit.h
%{_includedir}/graphOps.h
%{_includedir}/graphCrud.h
%{_libdir}/libgraphData.so
%{_libdir}/pkgconfig/graphdata.pc

%changelog
* Sat Nov 14 2026 David C Days <david.c.days@gmail.com> - 1.0.0-1
- Initial COPR / Fedora-style package
