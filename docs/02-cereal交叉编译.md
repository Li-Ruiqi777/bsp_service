# 交叉编译cereal

```bash
git clone https://github.com/USCiLab/cereal.git
cd cdreal
mkdir build
cd build
cmake .. \ 
  -DCMAKE_TOOLCHAIN_FILE=../../toolchain.cmake \
  -DCMAKE_INSTALL_PREFIX=/home/lrq/linux/nfs/qtrootfs/usr \
  -DJUST_INSTALL_CEREAL=ON \
  -DBUILD_TESTS=OFF \
  -DBUILD_SANDBOX=OFF \
  -DBUILD_DOC=OFF \
  -DTHREAD_SAFE=OFF
make install 
```