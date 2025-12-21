#include "XHlsIpConfig.hpp"
#include <iostream>

XHlsIpConfig::XHlsIpConfig() {}

XHlsIpConfig::~XHlsIpConfig() {}

void XHlsIpConfig::start(XHlsIp *ipInstPtr) {
    assert(ipInstPtr != nullptr);
    
    uint32_t data;
    FpgaConfig::readFpga(reinterpret_cast<void*>(&data), 4, ipInstPtr->controlBaseAddr + XHLS_IP_CONTROL_ADDR_AP_CTRL);

    data = (data & 0x80) | 0x01;
    FpgaConfig::writeFpga(reinterpret_cast<void*>(&data), 4, ipInstPtr->controlBaseAddr + XHLS_IP_CONTROL_ADDR_AP_CTRL);
}

uint32_t XHlsIpConfig::isDone(XHlsIp *ipInstPtr) {
    assert(ipInstPtr != nullptr);

    uint32_t data;
    FpgaConfig::readFpga(reinterpret_cast<void*>(&data), 4, ipInstPtr->controlBaseAddr + XHLS_IP_CONTROL_ADDR_AP_CTRL);

    return (data >> 1) & 0x01;
}

uint32_t XHlsIpConfig::isIdle(XHlsIp *ipInstPtr) {
    assert(ipInstPtr != nullptr);

    uint32_t data;
    FpgaConfig::readFpga(reinterpret_cast<void*>(&data), 4, ipInstPtr->controlBaseAddr + XHLS_IP_CONTROL_ADDR_AP_CTRL);

    return (data >> 2) & 0x01;
}

uint32_t XHlsIpConfig::isReady(XHlsIp *ipInstPtr) {
    assert(ipInstPtr != nullptr);
    
    uint32_t data;
    FpgaConfig::readFpga(reinterpret_cast<void*>(&data), 4, ipInstPtr->controlBaseAddr + XHLS_IP_CONTROL_ADDR_AP_CTRL);

    return (data >> 3) & 0x01;
}

void XHlsIpConfig::setParams(XHlsIp *ipInstPtr) {
    assert(ipInstPtr != nullptr);

    uint64_t offset = 0x10;
    for (auto param : ipInstPtr->params) {
        assert(param != nullptr);
        FpgaConfig::writeFpga(param, 8, ipInstPtr->controlBaseAddr + offset);
        // std::cout << std::hex << *param << " " << ipInstPtr->controlBaseAddr + offset << std::endl;
        offset += 0xc;
    }
    // std::cout << std::endl;
}

void XHlsIpConfig::getParams(XHlsIp *ipInstPtr) {

}

void XHlsIpConfig::setInput(XHlsIp *ipInstPtr, void *varPtr, size_t size) {
    assert(ipInstPtr != nullptr);
    uint64_t in_addr = *ipInstPtr->params.front();
    FpgaConfig::writeFpga(varPtr, size, in_addr);
}

void XHlsIpConfig::getOutput(XHlsIp *ipInstPtr, void *varPtr, size_t size) {
    assert(ipInstPtr != nullptr);
    uint64_t out_addr = *ipInstPtr->params.back();
    FpgaConfig::readFpga(varPtr, size, out_addr);
}
