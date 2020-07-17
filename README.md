## librga

RGA (Raster Graphic Acceleration Unit)是一个独立的2D硬件加速器，可用于加速点/线绘制，执行图像缩放、旋转、bitBlt、alpha混合等常见的2D图形操作。本仓库代码实现了RGA用户空间驱动，并提供了一系列2D图形操作API。

### 版本说明

**librga** 版本: v1.03<br/>
**librga API** 版本: v1.00

### 适用芯片平台

Rockchip RK3066 | RK3188 | RK2926 | RK2928 | RK3026 | RK3028 | RK3128 | Sofia3gr | RK3288 | RK3288w | RK3190 | RK1108 | RK3368 | RK3326 | RK3228 | RK3228H | RK3326 | RK1808 | RV1126 | RV1109 | RK3399 | RK3399pro

### 目录说明

**core:** RGA用户空间驱动实现

**include:** 相关头文件

**im2d_api:** RGA API相关实现及头文件

**samples:** librga使用例程

### 编译说明

* **Android Source Project**

下载librga仓库拷贝至android源码工程 hardware/rockchip目录，执行**mm**进行编译。根据不同的Android版本将自动选择Android.mk或Android.bp作为编译脚本。

* **Android NDK (build for android)**

修改librga源码根目录下的文件**ndk-android.sh**，指定**ANDROID_NDK_HOME**为NDK开发包的路径。执行以下操作完成编译：

```bash
$ mkdir build
$ cd build
$ cp ../cmake-android.sh ./
$ chmod +x ./cmake-android.sh
$ ./cmake-android.sh
$ make
```

* **Cmake (buildroot/debian)**

修改librga源码根目录下的**buildroot.cmake**文件，指定**TOOLCHAIN_HOME**为交叉编译工具的路径。执行以下操作完成编译:

```bash
$ mkdir build
$ cd build
$ cp ../cmake-linux.sh ./
$ chmod +x ./cmake-linux.sh
$ ./cmake-linux.sh
$ make
```

* **Makefile(buildroot/debian)**

librga也提供了Makefile文件，可以在开发板上直接执行make命令，或修改Makefile自行定义交叉编译工具后执行make完成编译。

### 使用说明

* **头文件引用**

  include/RockchipRga.h

  im2d_api/im2d.h

  im2d_api/im2d.hpp

* **库文件**

  librga.so

* librga应用开发接口说明参考以下文件：

  [API document](im2d_api/README.md)

