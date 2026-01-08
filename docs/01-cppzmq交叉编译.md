# 交叉编译cppzmq

参考链接：https://blog.csdn.net/yyz_1987/article/details/146570869

```bash
git clone https://github.com/zeromq/libzmq.git
git clone https://github.com/zeromq/cppzmq.git

# 先编译libzmq
cd libzmq
mkdir build
cd build
cmake ..\
-DCMAKE_TOOLCHAIN_FILE=/path/to/toolchain.cmake \
-DCMAKE_INSTALL_PREFIX=/path/to/install

make install -j$nproc

# 编译cppzmq
cd ../../cppzmq
mkdir build
cd build
cmake .. \
-DCMAKE_TOOLCHAIN_FILE=/path/to/toolchain.cmake \
-DCMAKE_INSTALL_PREFIX=/path/to/install \
-DCPPZMQ_BUILD_TESTS=OFF \
-DZeroMQ_INCLUDE_DIR=/path/to/libzmq_install/include/ \
-DZeroMQ_LIBRARY=/path/to/libzmq_install/lib/

make install -j$nproc
```