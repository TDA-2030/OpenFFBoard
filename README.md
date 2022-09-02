# OpenFFBoard-ESP32S2 + VESC

这里将介绍使用 ESP32-S2/S3 版本的 OpenFFBoard 搭配自制 VESC 驱动器使用的一个低成本实现直驱力反馈方向盘方案。[OpenffBoard](https://github.com/Ultrawipf/OpenFFBoard) 是一个开源的力反馈方向盘的项目，整个项目硬件分为 `控制板` 和 `电机驱动板` 两个部分，这里对两个部分都进行了低成本方向的修改。

## 文件结构

```
├── LICENSE                <- 开源协议 >
├── Simple-BLDC            <- 电机驱动板 >
├── openffboard-esp32s2    <- OpenFFBoard 主控板 >
├── Structural_parts       <- 一些可能有帮助的结构件>
└── README.md              <- 此文件 >
```

## OpenFFBoard-ESP32S2 控制板

原项目控制板是 STM32F4 做主控，这里将主控换成了[乐鑫](https://www.espressif.com/zh-hans)的 ESP32-S2/S3 芯片，成本得到极大的降低，同时提供了 WiFi 的连接能力（虽然现在并没有用上）。硬件开源资料见 [openffboard-esp32s2](./openffboard-esp32s2)

> V1.1 版本的硬件支持 ESP32-S2 和 ESP32-S3 二选一焊接，下文统称为 ESP32

和原项目相比，ESP32 不能提供足够多的 GPIO ，所以按键输入和模拟输入通道数均从原来的 8 通道精简到了 4 通道。详细信息见原理图 [esp32s2-openffb.pdf](openffboard-esp32s2/esp32s2-openffb.pdf)。
ESP32 控制板的源码已提交 Pull Request 到原项目，支持的功能以及进度见 [PR](https://github.com/Ultrawipf/OpenFFBoard/pull/46)。

### 控制板固件烧录

参见[快速下载固件指南](https://github.com/TDA-2030/OpenFFBoard/tree/feature/add_esp32s2/Firmware/Targets/ESP32SX#quick-download-firmware)

## 电机驱动板

为了造福广大劳动人民，针对原项目的电机驱动我也进行了低成本的改造，原项目是采用了比较贵的 TMC4761 专用 FOC 控制芯片，我将著名的开源电调 [VESC](https://vesc-project.com/) 进行了魔改，去掉了昂贵的 DRV8301 芯片，用多个芯片实现了这一个芯片的功能。集成度下降了，但是价格更友好。魔改后的 VESC 我叫它 `Simple-BLDC` 。关于电机驱动板的更多信息见 [Simple-BLDC](Simple-BLDC/README.md)

## 组装好的样子

<img src="assembly.gif" alt="assembly" width=500 />