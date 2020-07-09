fakemuduo 是一个用于学习高性能网络编程、重复造轮子的项目

### 编译

```shell
$ ./build.sh
```

生成库文件和测试程序

```
.
|-cmake-build-debug
  |-base
    |-tests
    |-libfakeMuduo_base.a
```

### 使用

引用头文件

```
-I fakeMuduo路径/base
```

引用库

```
-L fakeMuduo路径/cmake-build-debug/base
-llibfakeMuduo_base
```

