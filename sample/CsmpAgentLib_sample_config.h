#ifndef __SAMPLE_CONFIG_H__
#define __SAMPLE_CONFIG_H__


/// Min Registration inetval in seconds
#define CSMP_AGENT_REG_INTERVAL_MIN   10U

/// Max Registration inetval in seconds
#define CSMP_AGENT_REG_INTERVAL_MAX   100U

/// EUI64 Address of the Agent
#define CSMP_AGENT_EUI64_ADDRESS      "0000000000000000"

/// NMS Address
#define CSMP_AGENT_NMS_ADDRESS        "dead:beef::1" 

/// Enable/Disable Signature Settings
// #define CSMP_AGENT_SIGNATURE_SETTINGS 1


#endif // __SAMPLE_CONFIG_H__