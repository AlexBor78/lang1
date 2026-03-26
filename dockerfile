# Dockerfile
FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

# Step 1: Update package index
RUN apt-get update

# Step 2: Install toolchain (GCC 14 for full C++23 support)
RUN apt-get install -y --no-install-recommends \
    gcc-14 \
    g++-14 \
    cmake \
    ninja-build \
    make \
    doxygen \
    git

# Step 3: Cleanup cache
RUN apt-get clean && rm -rf /var/lib/apt/lists/* /var/cache/apt/archives/*.deb

# Step 4: Symlinks for default compiler
RUN ln -s /usr/bin/gcc-14 /usr/local/bin/gcc && \
    ln -s /usr/bin/g++-14 /usr/local/bin/g++

WORKDIR /app

CMD ["bash"]
