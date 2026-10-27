Project VS Code template for CH32V00x microcontrollers. It uses official WCH's SDK with `riscv32-wch-elf-gcc` compiler.

<p align="center">
  <img src="https://directus.yarkov.tech/assets/eb7c37b8-a0fc-4369-8828-64a09fc6b794" alt="Screenshot" width="700">
  <br>
  Debugging process of CH32V003F4P6 in VS Code
</p>

## Instructions to build

1. Download MRS toolchain from mounriver.com: [link](https://mounriver.com/download)
2. Extract folder and add binaries to `PATH` env:

```bash
sudo mkdir -p /opt/wch/MRS_Toolchain
sudo tar -xf MRS_Toolchain_Linux_X64_V240.tar.xz -C /opt/wch --strip-components=1
export PATH=/opt/wch/MRS_Toolchain/RISC-V\ Embedded\ GCC15/bin:$PATH
```

3. Download SDK from wch.cn for CH32V003 for example:

```bash
wget -O CH32V003EVT.ZIP https://file.wch.cn/download/file?id=412
sudo unzip CH32V003EVT.ZIP -d /opt/wch/CH32V003EVT
```

4. Install [wlink](https://github.com/ch32-rs/wlink) flasher tool:

```bash
cargo install --git https://github.com/ch32-rs/wlink
```

5. Build and flash

```bash
make && make prog_flash
```

For debugging, make sure you have the `libjaylink0` package installed.
