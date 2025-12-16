# CHAT-FPGA



**CHAT-FPGA** is a lightweight, quick-deployable LLM chat demo accelerated by FPGA. The goal of this project is to demonstrate the end-to-end flow of utilizing FPGA to accelerate LLM inference, with a focus on practical integration. 

The frontend basically relays on the [minchatgpt.cpp](https://github.com/iangitonga/minchatgpt.cpp), and runs a GPT-2 Medium (355M) models. The backend is extended with FPGA-accelerated decode operators, enabling selected Transformer computations to be offloaded from the CPU to the FPGA.



### Demo

(Demo content to be added)



### Quick install and run

1. Clone the repository

```bash
git clone https://github.com/WenbinTeng/chat-fpga.git
cd chat-fpga
```

2. Compile the host source files

```bash
make
```

3. Program the U280 FPGA with pre-compiled bitstreams

```bash
cd bitstreams
vivado -mode batch -source program.tcl
```

4. Download, build, and load the XDMA driver

```bash
git clone https://github.com/Xilinx/dma_ip_drivers.git
cd dma_ip_drivers/XDMA/linux-kernel

cd xdma
sudo make install

cd ../tools
sudo make

cd ../tests
sudo ./load_driver.sh
```

5. Run the demo

```bash
sudo ./minichatgpt
```

