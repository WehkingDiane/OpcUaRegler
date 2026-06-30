#pragma once

#include "opcuaregler/controller.hpp"
#include "opcuaregler/euromap83_model.hpp"

#include <cstdint>
#include <memory>
#include <string>

namespace opcuaregler {

class OpcUaBackend {
public:
    virtual ~OpcUaBackend() = default;

    virtual void start(const Euromap83WorkingPointModel& model) = 0;
    virtual void stop() = 0;
    virtual void publish(const ProcessImage& process, const std::vector<Alarm>& alarms) = 0;
    virtual double readActualValue() = 0;
    virtual double readSetpoint(double currentSetpoint) = 0;
    virtual bool readQualityGood() = 0;
    virtual bool readEnabled() = 0;
    virtual bool readManualMode() = 0;
    virtual double readManualOutput(double currentOutput) = 0;
    virtual bool readAcknowledgeRequested() = 0;
};

class MemoryOpcUaBackend final : public OpcUaBackend {
public:
    void start(const Euromap83WorkingPointModel& model) override;
    void stop() override;
    void publish(const ProcessImage& process, const std::vector<Alarm>& alarms) override;
    double readActualValue() override;
    double readSetpoint(double currentSetpoint) override;
    bool readQualityGood() override;
    bool readEnabled() override;
    bool readManualMode() override;
    double readManualOutput(double currentOutput) override;
    bool readAcknowledgeRequested() override;

private:
    bool running_{false};
    double simulatedActual_{95.0};
    bool enabled_{true};
    std::uint64_t cycle_{0};
};

#if defined(OPCUAREGLER_WITH_OPEN62541)
class Open62541OpcUaBackend final : public OpcUaBackend {
public:
    Open62541OpcUaBackend(std::uint16_t port, std::string applicationUri);
    ~Open62541OpcUaBackend() override;

    void start(const Euromap83WorkingPointModel& model) override;
    void stop() override;
    void publish(const ProcessImage& process, const std::vector<Alarm>& alarms) override;
    double readActualValue() override;
    double readSetpoint(double currentSetpoint) override;
    bool readQualityGood() override;
    bool readEnabled() override;
    bool readManualMode() override;
    double readManualOutput(double currentOutput) override;
    bool readAcknowledgeRequested() override;

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};
#endif

} // namespace opcuaregler
