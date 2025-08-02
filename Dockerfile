################################################################################
# build                                                                        #
################################################################################

FROM ubuntu:22.04 AS build

RUN apt-get update && apt-get install -y \
    cmake \
    build-essential \
    git \
    zlib1g-dev \
    libboost-all-dev \
    libssl-dev

WORKDIR /app

COPY . .

RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=0 && \
    cmake --build build --target edit-server

################################################################################
# run                                                                          #
################################################################################

FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    zlib1g

COPY --from=build /app/build/edit-server/edit-server /usr/local/bin/edit-server

RUN mkdir -p documents

ENTRYPOINT ["edit-server"]
