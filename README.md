# bsp_service
BSP Service - Daemon processes for service-oriented management of lib_bsp. Implements independent services with IPC support, providing concurrent access control, fault recovery, and standardized hardware access interfaces for upper-layer applications.

## 编译

### 环境要求

- CMake 3.10 或更高版本
- C++11 兼容的编译器（g++ 4.9+ 或 clang++ 3.3+）
- Linux 系统

### 编译步骤

BSP Service的编译依赖以下库, 请在编译BSP Service前安装好这些库
- [bsp-lib](https://github.com/Li-Ruiqi777/bsp-lib)
- [cppzmq](https://github.com/zeromq/cppzmq)
- [cereal](https://github.com/USCiLab/cereal)

BSP Service
```bash
mkdir build && cd build

make -j$nproc

make install
```