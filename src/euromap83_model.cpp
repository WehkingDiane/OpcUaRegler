#include "opcuaregler/euromap83_model.hpp"

namespace opcuaregler {

std::string Euromap83WorkingPointModel::namespaceUri() const {
    return "urn:opcuaregler:euromap83-working-point-controller";
}

std::string Euromap83WorkingPointModel::euromapGeneralTypesUri() const {
    return "http://opcfoundation.org/UA/PlasticsRubber/GeneralTypes/";
}

std::vector<NodeDefinition> Euromap83WorkingPointModel::nodes() const {
    return {
        {"Regler/Identification", "Identification", "Object", "Service identification object", false},
        {"Regler/Identification/ApplicationUri", "ApplicationUri", "String", "OPC UA application URI", false},
        {"Regler/Identification/SemanticBase", "SemanticBase", "String", "Referenced EUROMAP 83 / OPC 40083 General Types URI", false},
        {"Regler/Parameters/Setpoint", "Setpoint", "Double", "Target working point value", true},
        {"Regler/Parameters/ToleranceWarning", "ToleranceWarning", "Double", "Warning tolerance around setpoint", true},
        {"Regler/Parameters/ToleranceError", "ToleranceError", "Double", "Error tolerance around setpoint", true},
        {"Regler/Parameters/Kp", "Kp", "Double", "Proportional gain", true},
        {"Regler/Parameters/Ki", "Ki", "Double", "Integral gain", true},
        {"Regler/Parameters/OutputMin", "OutputMin", "Double", "Minimum manipulated value", true},
        {"Regler/Parameters/OutputMax", "OutputMax", "Double", "Maximum manipulated value", true},
        {"Regler/Process/ActualValue", "ActualValue", "Double", "Current process value", true},
        {"Regler/Process/Deviation", "Deviation", "Double", "Setpoint minus actual value", false},
        {"Regler/Process/OutputValue", "OutputValue", "Double", "Current manipulated value", false},
        {"Regler/Process/State", "State", "String", "Controller state", false},
        {"Regler/Process/QualityGood", "QualityGood", "Boolean", "Input value quality flag", true},
        {"Regler/Alarms/WarningActive", "WarningActive", "Boolean", "At least one warning alarm is active", false},
        {"Regler/Alarms/ErrorActive", "ErrorActive", "Boolean", "At least one error alarm is active", false},
        {"Regler/Alarms/ErrorCode", "ErrorCode", "Int32", "Highest priority active alarm code", false},
        {"Regler/Alarms/ErrorText", "ErrorText", "String", "Highest priority active alarm text", false},
        {"Regler/Commands/Enable", "Enable", "Boolean", "Enable or disable controller", true},
        {"Regler/Commands/Acknowledge", "Acknowledge", "Boolean", "Acknowledge active alarms", true},
        {"Regler/Commands/ManualMode", "ManualMode", "Boolean", "Switch controller to manual mode", true},
        {"Regler/Commands/ManualOutput", "ManualOutput", "Double", "Manual manipulated value", true},
        {"Regler/Simulation/Enabled", "Enabled", "Boolean", "Enable internal process simulation", true},
        {"Regler/Simulation/ActualValue", "ActualValue", "Double", "Current simulated process value", true},
        {"Regler/Simulation/Disturbance", "Disturbance", "Double", "Additive simulation disturbance", true},
        {"Regler/Simulation/TimeConstantSeconds", "TimeConstantSeconds", "Double", "First-order process time constant", true},
        {"Regler/Simulation/Reset", "Reset", "Boolean", "Reset simulated process value to setpoint", true},
    };
}

} // namespace opcuaregler
