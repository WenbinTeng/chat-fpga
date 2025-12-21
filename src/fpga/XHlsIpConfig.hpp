#ifndef __XHLS_IP_CONFIG_HPP__
#define __XHLS_IP_CONFIG_HPP__

#include <bits/stdint-uintn.h>
#include <cassert>
#include <vector>

#include "FpgaConfig.hpp"

// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2023.2 (64-bit)
// Tool Version Limit: 2023.10
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2023 Advanced Micro Devices, Inc. All Rights Reserved.
//
// ==============================================================
// control
// 0x00 : Control signals
//        bit 0  - ap_start (Read/Write/COH)
//        bit 1  - ap_done (Read/COR)
//        bit 2  - ap_idle (Read)
//        bit 3  - ap_ready (Read/COR)
//        bit 7  - auto_restart (Read/Write)
//        bit 9  - interrupt (Read)
//        others - reserved
// 0x04 : Global Interrupt Enable Register
//        bit 0  - Global Interrupt Enable (Read/Write)
//        others - reserved
// 0x08 : IP Interrupt Enable Register (Read/Write)
//        bit 0 - enable ap_done interrupt (Read/Write)
//        bit 1 - enable ap_ready interrupt (Read/Write)
//        others - reserved
// 0x0c : IP Interrupt Status Register (Read/TOW)
//        bit 0 - ap_done (Read/TOW)
//        bit 1 - ap_ready (Read/TOW)
//        others - reserved
// 0x10 : Data signal of ap_return
//        bit 31~0 - ap_return[31:0] (Read)
// 0x18 : Data signal of input_r
//        bit 31~0 - input_r[31:0] (Read/Write)
// 0x1c : Data signal of input_r
//        bit 31~0 - input_r[63:32] (Read/Write)
// 0x20 : reserved
// 0x24 : Data signal of weights
//        bit 31~0 - weights[31:0] (Read/Write)
// 0x28 : Data signal of weights
//        bit 31~0 - weights[63:32] (Read/Write)
// 0x2c : reserved
// 0x30 : Data signal of bias
//        bit 31~0 - bias[31:0] (Read/Write)
// 0x34 : Data signal of bias
//        bit 31~0 - bias[63:32] (Read/Write)
// 0x38 : reserved
// 0x3c : Data signal of output_r
//        bit 31~0 - output_r[31:0] (Read/Write)
// 0x40 : Data signal of output_r
//        bit 31~0 - output_r[63:32] (Read/Write)
// 0x44 : reserved
// (SC = Self Clear, COR = Clear on Read, TOW = Toggle on Write, COH = Clear on Handshake)

#define XHLS_IP_CONTROL_ADDR_AP_CTRL 0x00
#define XHLS_IP_CONTROL_ADDR_GIE 0x04
#define XHLS_IP_CONTROL_ADDR_IER 0x08
#define XHLS_IP_CONTROL_ADDR_ISR 0x0c
#define XHLS_IP_CONTROL_ADDR_AP_RETURN 0x10
#define XHLS_IP_CONTROL_BITS_AP_RETURN 32
#define XHLS_IP_CONTROL_ADDR_INPUT_DATA 0x18

struct XHlsIp {
    uint64_t controlBaseAddr;
    std::vector<uint64_t*> params; // input, params, output
};

class XHlsIpConfig {
  private:
    /* data */
  public:
    XHlsIpConfig();
    ~XHlsIpConfig();
    static void start(XHlsIp *ipInstPtr);
    static uint32_t isDone(XHlsIp *ipInstPtr);
    static uint32_t isIdle(XHlsIp *ipInstPtr);
    static uint32_t isReady(XHlsIp *ipInstPtr);
    static void setParams(XHlsIp *ipInstPtr);
    static void getParams(XHlsIp *ipInstPtr);
    static void setInput(XHlsIp *ipInstPtr, void *varPtr, size_t size);
    static void getOutput(XHlsIp *ipInstPtr, void *varPtr, size_t size);
};

#endif