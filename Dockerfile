FROM quay.io/pypa/manylinux_2_28_x86_64

RUN yum install -y \
      git curl zip unzip tar \
      cmake ninja-build gcc gcc-c++ make \
      pkgconf-pkg-config \
      libX11-devel libXcursor-devel libXinerama-devel libXrandr-devel \
      libXext-devel libXi-devel libXrender-devel \
      mesa-libGL-devel mesa-libGLU-devel \
    && yum clean all

# Install and setup vcpkg
ENV VCPKG_ROOT=/opt/vcpkg
RUN git clone https://github.com/microsoft/vcpkg.git ${VCPKG_ROOT} \
    && ${VCPKG_ROOT}/bootstrap-vcpkg.sh -disableMetrics

ENV VCPKG_FEATURE_FLAGS=manifests,versions
ENV VCPKG_DISABLE_METRICS=1

ENV CMAKE_TOOLCHAIN_FILE=${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake