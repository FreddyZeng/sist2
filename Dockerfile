FROM simon987/sist2-build as build
MAINTAINER simon987 <me@simon987.net>

WORKDIR /build/
ADD . /build/
RUN cmake -DSIST_DEBUG=off -DBUILD_TESTS=off -DCMAKE_TOOLCHAIN_FILE=/vcpkg/scripts/buildsystems/vcpkg.cmake .
RUN make -j$(nproc)
RUN strip sist2

FROM ubuntu:20.10

RUN apt update && apt install -y curl

RUN mkdir -p /usr/share/tessdata && \
    cd /usr/share/tessdata/ && \
    curl -o /usr/share/tessdata/hin.traineddata https://raw.githubusercontent.com/tesseract-ocr/tessdata/master/hin.traineddata &&\
    curl -o /usr/share/tessdata/jpn.traineddata https://raw.githubusercontent.com/tesseract-ocr/tessdata/master/jpn.traineddata &&\
    curl -o /usr/share/tessdata/eng.traineddata https://raw.githubusercontent.com/tesseract-ocr/tessdata/master/eng.traineddata &&\
    curl -o /usr/share/tessdata/fra.traineddata https://raw.githubusercontent.com/tesseract-ocr/tessdata/master/fra.traineddata &&\
    curl -o /usr/share/tessdata/rus.traineddata https://raw.githubusercontent.com/tesseract-ocr/tessdata/master/rus.traineddata &&\
    curl -o /usr/share/tessdata/spa.traineddata https://raw.githubusercontent.com/tesseract-ocr/tessdata/master/spa.traineddata

COPY --from=build /build/sist2 /root/sist2

ENV LANG C.UTF-8
ENV LC_ALL C.UTF-8

ENTRYPOINT ["/root/sist2"]
