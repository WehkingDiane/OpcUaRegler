#if defined(OPCUAREGLER_WITH_OPEN62541)

#include "opcuaregler/opcua_backend.hpp"

#include <open62541/server.h>
#include <open62541/server_config_default.h>

#include <algorithm>
#include <stdexcept>
#include <utility>

namespace opcuaregler {
namespace {

UA_NodeId nodeId(const char* identifier) {
    return UA_NODEID_STRING_ALLOC(1, identifier);
}

UA_QualifiedName qname(const char* name) {
    return UA_QUALIFIEDNAME_ALLOC(1, name);
}

UA_LocalizedText text(const char* value) {
    return UA_LOCALIZEDTEXT_ALLOC("en-US", value);
}

void clearAllocated(UA_NodeId& id, UA_QualifiedName& name, UA_LocalizedText& displayName) {
    UA_NodeId_clear(&id);
    UA_QualifiedName_clear(&name);
    UA_LocalizedText_clear(&displayName);
}

void addFolder(UA_Server* server, const char* id, const char* browseName, const UA_NodeId& parent) {
    UA_ObjectAttributes attr = UA_ObjectAttributes_default;
    attr.displayName = text(browseName);

    auto folderId = nodeId(id);
    auto qualified = qname(browseName);
    auto result = UA_Server_addObjectNode(
        server,
        folderId,
        parent,
        UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
        qualified,
        UA_NODEID_NUMERIC(0, UA_NS0ID_FOLDERTYPE),
        attr,
        nullptr,
        nullptr);

    clearAllocated(folderId, qualified, attr.displayName);

    if (result != UA_STATUSCODE_GOOD) {
        throw std::runtime_error("Failed to add OPC UA folder node");
    }
}

void addDouble(UA_Server* server, const char* id, const char* browseName, const UA_NodeId& parent, double value, bool writable) {
    UA_VariableAttributes attr = UA_VariableAttributes_default;
    UA_Variant_setScalar(&attr.value, &value, &UA_TYPES[UA_TYPES_DOUBLE]);
    attr.displayName = text(browseName);
    attr.accessLevel = UA_ACCESSLEVELMASK_READ | (writable ? UA_ACCESSLEVELMASK_WRITE : 0);

    auto variableId = nodeId(id);
    auto qualified = qname(browseName);
    auto result = UA_Server_addVariableNode(
        server,
        variableId,
        parent,
        UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
        qualified,
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
        attr,
        nullptr,
        nullptr);

    clearAllocated(variableId, qualified, attr.displayName);

    if (result != UA_STATUSCODE_GOOD) {
        throw std::runtime_error("Failed to add OPC UA double node");
    }
}

void addBool(UA_Server* server, const char* id, const char* browseName, const UA_NodeId& parent, bool value, bool writable) {
    UA_Boolean uaValue = value;
    UA_VariableAttributes attr = UA_VariableAttributes_default;
    UA_Variant_setScalar(&attr.value, &uaValue, &UA_TYPES[UA_TYPES_BOOLEAN]);
    attr.displayName = text(browseName);
    attr.accessLevel = UA_ACCESSLEVELMASK_READ | (writable ? UA_ACCESSLEVELMASK_WRITE : 0);

    auto variableId = nodeId(id);
    auto qualified = qname(browseName);
    auto result = UA_Server_addVariableNode(
        server,
        variableId,
        parent,
        UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
        qualified,
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
        attr,
        nullptr,
        nullptr);

    clearAllocated(variableId, qualified, attr.displayName);

    if (result != UA_STATUSCODE_GOOD) {
        throw std::runtime_error("Failed to add OPC UA bool node");
    }
}

void addInt32(UA_Server* server, const char* id, const char* browseName, const UA_NodeId& parent, int value, bool writable) {
    UA_Int32 uaValue = value;
    UA_VariableAttributes attr = UA_VariableAttributes_default;
    UA_Variant_setScalar(&attr.value, &uaValue, &UA_TYPES[UA_TYPES_INT32]);
    attr.displayName = text(browseName);
    attr.accessLevel = UA_ACCESSLEVELMASK_READ | (writable ? UA_ACCESSLEVELMASK_WRITE : 0);

    auto variableId = nodeId(id);
    auto qualified = qname(browseName);
    auto result = UA_Server_addVariableNode(
        server,
        variableId,
        parent,
        UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
        qualified,
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
        attr,
        nullptr,
        nullptr);

    clearAllocated(variableId, qualified, attr.displayName);

    if (result != UA_STATUSCODE_GOOD) {
        throw std::runtime_error("Failed to add OPC UA int32 node");
    }
}

void addString(UA_Server* server, const char* id, const char* browseName, const UA_NodeId& parent, const char* value, bool writable) {
    UA_String uaValue = UA_STRING_ALLOC(value);
    UA_VariableAttributes attr = UA_VariableAttributes_default;
    UA_Variant_setScalar(&attr.value, &uaValue, &UA_TYPES[UA_TYPES_STRING]);
    attr.displayName = text(browseName);
    attr.accessLevel = UA_ACCESSLEVELMASK_READ | (writable ? UA_ACCESSLEVELMASK_WRITE : 0);

    auto variableId = nodeId(id);
    auto qualified = qname(browseName);
    auto result = UA_Server_addVariableNode(
        server,
        variableId,
        parent,
        UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
        qualified,
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
        attr,
        nullptr,
        nullptr);

    UA_String_clear(&uaValue);
    clearAllocated(variableId, qualified, attr.displayName);

    if (result != UA_STATUSCODE_GOOD) {
        throw std::runtime_error("Failed to add OPC UA string node");
    }
}

template <typename T>
bool readScalar(UA_Server* server, const char* id, T& value, const UA_DataType& type) {
    UA_Variant variant;
    UA_Variant_init(&variant);
    auto variableId = nodeId(id);
    const auto status = UA_Server_readValue(server, variableId, &variant);
    UA_NodeId_clear(&variableId);

    if (status != UA_STATUSCODE_GOOD || !UA_Variant_hasScalarType(&variant, &type)) {
        UA_Variant_clear(&variant);
        return false;
    }

    value = *static_cast<T*>(variant.data);
    UA_Variant_clear(&variant);
    return true;
}

void writeDouble(UA_Server* server, const char* id, double value) {
    UA_Variant variant;
    UA_Variant_init(&variant);
    UA_Variant_setScalar(&variant, &value, &UA_TYPES[UA_TYPES_DOUBLE]);
    auto variableId = nodeId(id);
    UA_Server_writeValue(server, variableId, variant);
    UA_NodeId_clear(&variableId);
}

void writeBool(UA_Server* server, const char* id, bool value) {
    UA_Boolean uaValue = value;
    UA_Variant variant;
    UA_Variant_init(&variant);
    UA_Variant_setScalar(&variant, &uaValue, &UA_TYPES[UA_TYPES_BOOLEAN]);
    auto variableId = nodeId(id);
    UA_Server_writeValue(server, variableId, variant);
    UA_NodeId_clear(&variableId);
}

void writeInt32(UA_Server* server, const char* id, int value) {
    UA_Int32 uaValue = value;
    UA_Variant variant;
    UA_Variant_init(&variant);
    UA_Variant_setScalar(&variant, &uaValue, &UA_TYPES[UA_TYPES_INT32]);
    auto variableId = nodeId(id);
    UA_Server_writeValue(server, variableId, variant);
    UA_NodeId_clear(&variableId);
}

void writeString(UA_Server* server, const char* id, const std::string& value) {
    UA_String uaValue = UA_STRING_ALLOC(value.c_str());
    UA_Variant variant;
    UA_Variant_init(&variant);
    UA_Variant_setScalar(&variant, &uaValue, &UA_TYPES[UA_TYPES_STRING]);
    auto variableId = nodeId(id);
    UA_Server_writeValue(server, variableId, variant);
    UA_NodeId_clear(&variableId);
    UA_String_clear(&uaValue);
}

} // namespace

struct Open62541OpcUaBackend::Impl {
    explicit Impl(std::uint16_t configuredPort, std::string configuredApplicationUri)
        : port(configuredPort), applicationUri(std::move(configuredApplicationUri)) {}

    UA_Server* server{nullptr};
    std::uint16_t port{};
    std::string applicationUri;
    bool running{false};
};

Open62541OpcUaBackend::Open62541OpcUaBackend(std::uint16_t port, std::string applicationUri)
    : impl_(std::make_unique<Impl>(port, std::move(applicationUri))) {}

Open62541OpcUaBackend::~Open62541OpcUaBackend() {
    stop();
}

void Open62541OpcUaBackend::start(const Euromap83WorkingPointModel& model) {
    impl_->server = UA_Server_new();
    if (impl_->server == nullptr) {
        throw std::runtime_error("Failed to create open62541 server");
    }

    auto* config = UA_Server_getConfig(impl_->server);
    auto status = UA_ServerConfig_setMinimal(config, impl_->port, nullptr);
    if (status != UA_STATUSCODE_GOOD) {
        throw std::runtime_error("Failed to configure open62541 server");
    }

    UA_String_clear(&config->applicationDescription.applicationUri);
    config->applicationDescription.applicationUri = UA_STRING_ALLOC(impl_->applicationUri.c_str());

    UA_Server_addNamespace(impl_->server, model.namespaceUri().c_str());

    const auto objects = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
    addFolder(impl_->server, "Regler", "Regler", objects);
    auto regler = nodeId("Regler");
    addFolder(impl_->server, "Regler.Identification", "Identification", regler);
    addFolder(impl_->server, "Regler.Parameters", "Parameters", regler);
    addFolder(impl_->server, "Regler.Process", "Process", regler);
    addFolder(impl_->server, "Regler.Alarms", "Alarms", regler);
    addFolder(impl_->server, "Regler.Commands", "Commands", regler);

    auto identification = nodeId("Regler.Identification");
    auto parameters = nodeId("Regler.Parameters");
    auto process = nodeId("Regler.Process");
    auto alarms = nodeId("Regler.Alarms");
    auto commands = nodeId("Regler.Commands");

    addString(impl_->server, "Regler.Identification.ApplicationUri", "ApplicationUri", identification, impl_->applicationUri.c_str(), false);
    addString(impl_->server, "Regler.Identification.SemanticBase", "SemanticBase", identification, model.euromapGeneralTypesUri().c_str(), false);
    addDouble(impl_->server, "Regler.Parameters.Setpoint", "Setpoint", parameters, 100.0, true);
    addDouble(impl_->server, "Regler.Parameters.ToleranceWarning", "ToleranceWarning", parameters, 2.0, true);
    addDouble(impl_->server, "Regler.Parameters.ToleranceError", "ToleranceError", parameters, 5.0, true);
    addDouble(impl_->server, "Regler.Parameters.Kp", "Kp", parameters, 1.0, true);
    addDouble(impl_->server, "Regler.Parameters.Ki", "Ki", parameters, 0.0, true);
    addDouble(impl_->server, "Regler.Parameters.OutputMin", "OutputMin", parameters, 0.0, true);
    addDouble(impl_->server, "Regler.Parameters.OutputMax", "OutputMax", parameters, 100.0, true);
    addDouble(impl_->server, "Regler.Process.ActualValue", "ActualValue", process, 100.0, true);
    addDouble(impl_->server, "Regler.Process.Deviation", "Deviation", process, 0.0, false);
    addDouble(impl_->server, "Regler.Process.OutputValue", "OutputValue", process, 0.0, false);
    addString(impl_->server, "Regler.Process.State", "State", process, "INIT", false);
    addBool(impl_->server, "Regler.Process.QualityGood", "QualityGood", process, true, true);
    addBool(impl_->server, "Regler.Alarms.WarningActive", "WarningActive", alarms, false, false);
    addBool(impl_->server, "Regler.Alarms.ErrorActive", "ErrorActive", alarms, false, false);
    addInt32(impl_->server, "Regler.Alarms.ErrorCode", "ErrorCode", alarms, 0, false);
    addString(impl_->server, "Regler.Alarms.ErrorText", "ErrorText", alarms, "", false);
    addBool(impl_->server, "Regler.Commands.Enable", "Enable", commands, true, true);
    addBool(impl_->server, "Regler.Commands.Acknowledge", "Acknowledge", commands, false, true);
    addBool(impl_->server, "Regler.Commands.ManualMode", "ManualMode", commands, false, true);
    addDouble(impl_->server, "Regler.Commands.ManualOutput", "ManualOutput", commands, 0.0, true);

    UA_NodeId_clear(&commands);
    UA_NodeId_clear(&alarms);
    UA_NodeId_clear(&process);
    UA_NodeId_clear(&parameters);
    UA_NodeId_clear(&identification);
    UA_NodeId_clear(&regler);

    status = UA_Server_run_startup(impl_->server);
    if (status != UA_STATUSCODE_GOOD) {
        throw std::runtime_error("Failed to start open62541 server");
    }

    impl_->running = true;
}

void Open62541OpcUaBackend::stop() {
    if (impl_ == nullptr || impl_->server == nullptr) {
        return;
    }

    if (impl_->running) {
        UA_Server_run_shutdown(impl_->server);
        impl_->running = false;
    }

    UA_Server_delete(impl_->server);
    impl_->server = nullptr;
}

void Open62541OpcUaBackend::publish(const ProcessImage& process, const std::vector<Alarm>& alarms) {
    writeDouble(impl_->server, "Regler.Parameters.Setpoint", process.setpoint);
    writeDouble(impl_->server, "Regler.Process.Deviation", process.deviation);
    writeDouble(impl_->server, "Regler.Process.OutputValue", process.outputValue);
    writeString(impl_->server, "Regler.Process.State", toString(process.state));

    const auto hasWarning = std::any_of(alarms.begin(), alarms.end(), [](const Alarm& alarm) {
        return alarm.active && alarm.severity == Severity::Warning;
    });
    const auto errorIt = std::find_if(alarms.begin(), alarms.end(), [](const Alarm& alarm) {
        return alarm.active && alarm.severity == Severity::Error;
    });

    writeBool(impl_->server, "Regler.Alarms.WarningActive", hasWarning);
    writeBool(impl_->server, "Regler.Alarms.ErrorActive", errorIt != alarms.end());
    writeInt32(impl_->server, "Regler.Alarms.ErrorCode", errorIt != alarms.end() ? errorIt->code : 0);
    writeString(impl_->server, "Regler.Alarms.ErrorText", errorIt != alarms.end() ? errorIt->text : "");

    UA_Server_run_iterate(impl_->server, false);
}

double Open62541OpcUaBackend::readActualValue() {
    double value = 0.0;
    readScalar(impl_->server, "Regler.Process.ActualValue", value, UA_TYPES[UA_TYPES_DOUBLE]);
    return value;
}

double Open62541OpcUaBackend::readSetpoint(double currentSetpoint) {
    double value = currentSetpoint;
    readScalar(impl_->server, "Regler.Parameters.Setpoint", value, UA_TYPES[UA_TYPES_DOUBLE]);
    return value;
}

bool Open62541OpcUaBackend::readQualityGood() {
    UA_Boolean value = true;
    readScalar(impl_->server, "Regler.Process.QualityGood", value, UA_TYPES[UA_TYPES_BOOLEAN]);
    return value;
}

bool Open62541OpcUaBackend::readEnabled() {
    UA_Boolean value = true;
    readScalar(impl_->server, "Regler.Commands.Enable", value, UA_TYPES[UA_TYPES_BOOLEAN]);
    return value;
}

bool Open62541OpcUaBackend::readManualMode() {
    UA_Boolean value = false;
    readScalar(impl_->server, "Regler.Commands.ManualMode", value, UA_TYPES[UA_TYPES_BOOLEAN]);
    return value;
}

double Open62541OpcUaBackend::readManualOutput(double currentOutput) {
    double value = currentOutput;
    readScalar(impl_->server, "Regler.Commands.ManualOutput", value, UA_TYPES[UA_TYPES_DOUBLE]);
    return value;
}

bool Open62541OpcUaBackend::readAcknowledgeRequested() {
    UA_Boolean value = false;
    if (readScalar(impl_->server, "Regler.Commands.Acknowledge", value, UA_TYPES[UA_TYPES_BOOLEAN]) && value) {
        writeBool(impl_->server, "Regler.Commands.Acknowledge", false);
        return true;
    }
    return false;
}

} // namespace opcuaregler

#endif
