# openfang
The openfang is a bootloader, kernel and toolchain for devices using Ingenic T10 and T20 SOC.

## Overview

At the present time, this repository only contains kernel and rootfs for cameras using Inegnic T20 SOC. To ease identifying these cameras please use the pictures below. A more detailed technical description can be found [here](doc/overview.md).

Ingenic T20L (64Mb DDR) | &nbsp;
:-- | --:
![Xiaomi Mijia](doc/SXJ02ZM/img/xiaomi_mijia_2018.jpg) Xiaomi Mijia 2018 | ![Xiaomi Xiaofang 1S](doc/xiaomi_xiaofang1s/img/xiaofang1s.jpg) Xiaomi Xiaofang 1S

Ingenic T20N (64Mb DDR + SIMD128)
:-- |
![DIGOO DG W30](doc/dg-w30/img/dg-w30.jpg) DIGOO DG W30

Ingenic T20X (128Mb DDR) | &nbsp;
:-- | --:
![Wyze Cam v2](doc/wyzecam_v2/img/wyzecam_v2.jpg) Wyze Cam V2 | 
![Xiaomi Dafang](doc/xiaomi_dafang/img/xiaomi_dafang.jpg) Xiaomi Dafang | ![Wyze Cam Pan](doc/WYZECP1/img/wyzecam_pan.jpg) Wyze Cam Pan

If you have a device with a Ingenic T10 SOC, consider using for now https://github.com/EliasKotlyar/Xiaomi-Dafang-Hacks

If you have a classic XiaoFang with a ARM-Processor, consider using https://github.com/samtap/fang-hacks


## How to install openfang

Before installing openfang, we suggest you carefully read the [FAQ](/doc/faq.md).

After that, continue to the [Installation](/doc/install.md) procedure. More details on [troubleshooting](doc/troubleshooting.md).

## How to compile openfang

Openfang is actively developed and releases are only provided when the openfang distribution is stable. If you want to test the development version you can easily compile it using [docker](https://www.docker.com/). More details on how to do it [here](doc/docker.md).

### Issues and support ###

If you encounter an issue which you think is a bug in the openfang or the associated libraries, you are welcome to submit it here on Github: https://github.com/anmaped/openfang/issues.

Please provide as much context as possible:

- buildroot core version which you are using;
- kernel version and modules you have enable;
- build root packages you are trying to integrate;
- when encountering an issue which happens at run time, attach serial output. Wrap it into a code block, just like the code;
- for issues which happen at compile time, enable verbose compiler output in the buildroot preferences, and attach that output (also inside a code block);
- development board model and brand;
- other settings (board choice, flash size, etc.).

Or ask in our [Gitter channel](https://gitter.im/openfang_project) for help.

### Contributing

For minor fixes of code and documentation, please go ahead and submit a pull request.

Check out the list of issues which are easy to fix — [easy issues for openfang 01](https://github.com/anmaped/openfang/issues). Working on them is a great way to move the project forward.

Larger changes (rewriting parts of existing libraries from scratch, adding new functions to the v4l core, adding new libraries) should generally be discussed by opening an issue first.

Feature branches with lots of small commits (especially titled "oops", "fix typo", "forgot to add file", etc.) should be squashed before opening a pull request. At the same time, please refrain from putting multiple unrelated changes into a single pull request.

### License and credits ###

Buildroot is developed and maintained by The Buildroot developers.
