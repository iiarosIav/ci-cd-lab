FROM alpine:latest AS builder

RUN apk add --no-cache cmake make g++ musl-dev linux-headers

WORKDIR /app
COPY src ./src
COPY CMakeLists.txt .

RUN mkdir build && cd build && cmake .. && make

FROM alpine:latest

RUN apk add --no-cache libstdc++

WORKDIR /app
COPY --from=builder /app/build/app .

EXPOSE 8080

CMD ["./app"]