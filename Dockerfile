FROM debian:stretch

RUN apt-get update && apt-get install -y \
  build-essential autoconf libtool git pkg-config curl \
  automake libtool curl make g++ unzip \
  && apt-get clean

# install protobuf first, then grpc
ENV GRPC_RELEASE_TAG v1.21.x
RUN git clone -b ${GRPC_RELEASE_TAG} https://github.com/grpc/grpc /var/local/git/grpc && \
    cd /var/local/git/grpc && \
    git submodule update --init && \
    echo "--- installing protobuf ---" && \
    cd third_party/protobuf && \
    git submodule update --init && \
    ./autogen.sh && ./configure --enable-shared && \
    make -j$(nproc) && make -j$(nproc) check && make install && make clean && ldconfig && \
    echo "--- installing grpc ---" && \
    cd /var/local/git/grpc && \
    make -j$(nproc) && make install && make clean && ldconfig && \
    rm -rf /var/local/git/grpc


RUN apt-get update && apt-get install -y \
  python-pip libsamplerate0-dev libespeak-ng-dev \
  && apt-get clean

RUN pip2 install --no-cache-dir setuptools --upgrade
RUN pip2 install --no-cache-dir pip --upgrade
RUN pip2 install --no-cache-dir six --upgrade
RUN pip2 install --no-cache-dir pyasn1==0.4.6
RUN pip2 install --no-cache-dir hazm
RUN pip2 install --no-cache-dir tensorflow==1.9.0 --user
RUN pip2 install --no-cache-dir tensor2tensor==1.7.0 --user

RUN git clone https://github.com/tihu-nlp/tihu/ && \
  cd tihu && make release && make grpc && make clean

EXPOSE 50051

CMD /tihu/build/tihu_server 0.0.0.0:50051 /tihu/build/libtihu.so ""
