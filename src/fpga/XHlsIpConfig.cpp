#include "XHlsIpConfig.hpp"

XHlsIpConfig::XHlsIpConfig() {}

XHlsIpConfig::~XHlsIpConfig() {}

void XHlsIpConfig::start(XHlsIp *ipInstPtr) {
    assert(ipInstPtr != nullptr);
    
    uint32_t data;
    FpgaConfig::readFpga(reinterpret_cast<void*>(&data), 4,ipInstPtr->controlBaseAddr + XHLS_IP_CONTROL_ADDR_AP_CTRL);

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

void XHlsIpConfig::setParams(XHlsIp *ipInstPtr, size_t offset, void *paramsPtr, uint64_t paramsSize) {
    assert(ipInstPtr != nullptr);
    assert(paramsPtr != nullptr);
    assert(paramsSize > 0);

    FpgaConfig::writeFpga(paramsPtr, paramsSize, ipInstPtr->controlBaseAddr + offset);
}

void XHlsIpConfig::getParams(XHlsIp *ipInstPtr, size_t offset, void *paramsPtr, uint64_t paramsSize) {
    assert(ipInstPtr != nullptr);
    assert(paramsPtr != nullptr);
    assert(paramsSize > 0);

    FpgaConfig::readFpga(paramsPtr, paramsSize, ipInstPtr->controlBaseAddr + offset);
}