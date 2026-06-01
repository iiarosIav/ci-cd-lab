FROM alpine:latest AS builder

RUN apk add --no-cache cmake make g++ musl-dev linux-headers

WORKDIR /app
COPY src ./src
COPY CMakeLists.txt .

# Отключаем сборку тестов внутри Docker-образа
RUN mkdir build && cd build && cmake -DBUILD_TESTS=OFF .. && make

FROM alpine:latest

RUN apk add --no-cache libstdc++

WORKDIR /app
COPY --from=builder /app/build/app .

EXPOSE 8080

CMD ["./app"]