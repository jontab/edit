################################################################################
# build                                                                        #
################################################################################

FROM ubuntu:22.04 AS build

RUN apt-get update && apt-get install -y \
    cmake \
    build-essential \
    git \
    zlib1g-dev

WORKDIR /app

COPY common.cmake common.cmake
COPY deps/ deps/
COPY edit-server/ edit-server/

RUN cmake -S edit-server -B build -DCMAKE_BUILD_TYPE=Release && \
    cmake --build build --target edit-server

################################################################################
# run                                                                          #
################################################################################

FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    zlib1g

COPY --from=build /app/build/edit-server /usr/local/bin/edit-server

ENTRYPOINT ["edit-server"]
