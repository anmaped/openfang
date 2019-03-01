# Use docker to generate the openfang firmware and toolchain

```
docker build -f ./Dockerfile -t openfang/container .
docker run openfang/container
docker exec openfang/container sh /root/openfang/buildopenfang.sh
```
