# TI Wi-SUN CSMP Agent - Complete Setup Guide

This guide walks you through everything needed to run the CSMP example on a TI Wi-SUN device from scratch. It covers building the firmware for all supported boards, setting up the border router, configuring network routing, flashing the device, and verifying that it registers with Cisco FND.


## 1. What Is This and How Does It Work?

**CSMP** (CoAP Simple Management Protocol) is a lightweight protocol that lets IoT devices register with and be managed by a central server. In this setup:

- A **TI Wi-SUN LaunchPad** (mesh node) runs the CSMP Agent firmware, which periodically sends registration messages to the management server.
- A second TI board acts as the **Border Router**. It is connected to the Linux PC via USB/UART.
- **Linux Host container** — a Docker container that runs the border router software stack and communicates with the border router board over UART.
- **wfantund container** — a Docker container that communicates with the Linux Host container via IPv6 tunneling. It creates a `wfan0` TUN interface that is accessible on the Linux PC (outside Docker).
- The **Linux PC** routes packets between `wfan0` and FND over its physical network interface.
- **Cisco FND** (Field Network Director) is the management server. The CSMP Agent on the mesh node registers with FND over IPv6.

Both Docker containers run on the same Linux PC. The packet flow looks like this:

```
TI Wi-SUN mesh node (CSMP Agent firmware)
    |  (Wi-SUN RF)
    v
TI border router board
    |  (UART/USB)
    v
Linux Host Docker container (border router software stack)
    |  (IPv6 tunnel between containers)
    v
wfantund Docker container (creates wfan0 TUN on Linux PC)
    |  (wfan0 accessible on Linux PC host)
    v
Linux PC (routes packets via physical NIC)
    |  (IPv6 over LAN)
    v
Cisco FND (management server, receives CoAP registration)
```

### Supported boards

The following boards have pre-built CSMP project configurations under `Vendors/TI/`:

| Board | Architecture | Device family |
|-------|-------------|---------------|
| LP-CC1312R7 | Cortex-M4F | CC13X2X7 |
| LP-CC1352P7-1 | Cortex-M4F | CC13X2X7 |
| LP-EM-CC1314R10 | Cortex-M33 | CC13X4 |
| LP-EM-CC1354P10-1 | Cortex-M33 | CC13X4 |
| LP-EM-CC1354P10-6 | Cortex-M33 | CC13X4 |

A single command builds firmware for all boards at once. Adding a new board requires only creating a `Vendors/TI/<BOARD>/csmp_example_tirf/` directory — the build system discovers it automatically.

---

## 2. What You Need (Hardware and Software)

### Hardware

| Item | Purpose |
|------|---------|
| Any supported TI LaunchPad (x1, from the table in Section 1) | Wi-SUN mesh node running CSMP Agent firmware |
| Any TI CC13xx LaunchPad (x1) flashed with `rcp_lmac` border router firmware | Bridges Wi-SUN mesh to the Linux host |
| Linux PC | Hosts both Docker containers, routes packets between wfan0 and FND |
| Cisco FND server or VM | Management server that receives CSMP registration |

> The mesh node and border router do not need to be the same board type, but both must run compatible Wi-SUN FAN stack versions from the same SDK release.

### Software (install before you begin)

On the Linux host:

```bash
# TI UniFlash (for flashing) - download from:
# https://www.ti.com/tool/UNIFLASH

# iptables persistence
sudo apt install iptables-persistent
```

On the Linux PC, also install:
- **TI SimpleLink CC13xx/CC26xx SDK**  - download from [ti.com/tool/SIMPLELINK-CC13XX-CC26XX-SDK](https://www.ti.com/tool/SIMPLELINK-CC13XX-CC26XX-SDK)

---

## 3. Network Architecture

Before touching any commands, understand what network addresses are involved. This example uses the following layout - **adapt the addresses to match your own setup**.

```
+---------------------------+       +-----------------------------------+
|       FND Server          |       |         Linux PC (host)           |
|   (RedHat VM)             |       |                                   |
|   ens192: fd00::2/64      +-------+  eno2: fd00::1/64   (LAN)         |
|                           |  LAN  |  wfan0: 2020:abcd::/64            |
+---------------------------+       |  (TUN exposed by wfantund         |
                                    |   container, accessible on host)  |
                                    +---+---------------------------+---+
                                        |                           |
                          (Docker IPv6 bridge)             (Docker IPv6 bridge)
                                        |                           |
                          +-------------+-------+   +--------------+------+
                          |  Linux Host         |   |  wfantund           |
                          |  Docker Container   |   |  Docker Container   |
                          |                     +---+                     |
                          |  Runs border router |IPv6| Creates wfan0 TUN  |
                          |  software stack     |tun | Exposes to host PC |
                          +----------+----------+   +---------------------+
                                     |
                                     | (UART/USB)
                                     |
                          +----------+----------+
                          |  TI Wi-SUN          |
                          |  Border Router Board|
                          +----------+----------+
                                     |
                                     | (Wi-SUN RF)
                                     |
                          +----------+----------+
                          |  TI Wi-SUN Mesh Node |
                          |  CSMP Agent firmware |
                          |  2020:abcd::0212:... |
                          +---------------------+
```

**Address summary for this guide:**

| Component | Interface | IPv6 Address | Notes |
|-----------|-----------|-------------|-------|
| FND VM | ens192 | `fd00::2/64` | On physical LAN |
| Linux PC | eno2 | `fd00::1/64` | On physical LAN |
| Linux PC | wfan0 | `2020:abcd::/64` | TUN exposed by wfantund container |
| Linux PC | br-xxxx (Docker bridge) | `fddb:a737:317c::1` | Gateway for both containers |
| Linux Host container | eth0 | `fddb:a737:317c::x` | Talks to border router via UART; tunnels to wfantund |
| wfantund container | eth0 | `fddb:a737:317c::2` | Connects to Linux Host via IPv6 tunnel |
| TI Wi-SUN mesh node | Wi-SUN | `2020:abcd::0212:4b00:xxxx` | Address assigned by border router |

> **Note on addresses:** All addresses above are examples — adapt them to your own setup. `fd00::/64` is the ULA prefix shared between the Linux PC and FND on the physical LAN. `fddb:a737:317c::/64` is the Docker bridge subnet used by both containers. `2020:abcd::/64` is the Wi-SUN mesh prefix assigned by wfantund.


---

## 4. Part A: Configure the NMS Address

The CSMP Agent firmware needs to know the IPv6 address of your FND server at **compile time**. The address is defined in **two places** — both must match your FND address before building.

| File | Used by | What it controls |
|------|---------|-----------------|
| `Vendors/TI/application/defines/router.opts` | TI board builds (`CONFIG=SECUREBOOT`) | Passed as `-D` compiler flag, overrides the header at build time |
| `sample/CsmpAgentLib_sample.h` | Linux / FreeRTOS builds; also the fallback default | `#define CSMP_AGENT_NMS_ADDRESS` used if the `-D` flag is not present |

Update both files so the address is consistent regardless of which build target is used.

### Step A.1 - Update `router.opts` (TI board builds)

```bash
nano Vendors/TI/application/defines/router.opts
```

Find this line:

```
-DCSMP_AGENT_NMS_ADDRESS=\"fd00::3\"
```

Replace `fd00::3` with your FND server's actual IPv6 address:

```
-DCSMP_AGENT_NMS_ADDRESS=\"fd00::2\"
```

Save and close (`Ctrl+X`, `Y`, `Enter` in nano).

This file is shared by all TI boards — changing it once updates every board in the next build.

### Step A.2 - Update `CsmpAgentLib_sample.h` (all build targets)

```bash
nano sample/CsmpAgentLib_sample.h
```

Find this line near the top of the configuration block (around line 123):

```c
#define CSMP_AGENT_NMS_ADDRESS        "fd00::3"
```

Replace the address to match what you set in Step A.1:

```c
#define CSMP_AGENT_NMS_ADDRESS        "fd00::2"
```

Save and close.

> Both files must have the same FND address. For TI board builds the `router.opts` `-D` flag takes precedence at compile time, but keeping the header in sync ensures Linux and FreeRTOS builds work correctly without a separate change.

---

## 5. Part B: Build the CSMP Firmware

The build requires only two things: the SimpleLink CC13xx/CC26xx SDK and the TI ARM Clang compiler. There are **three files to edit** before your first build.

> **Setup checklist — complete these once per machine, then never again:**
>
> | # | File to edit | Variable to set | What it controls |
> |---|-------------|-----------------|-----------------|
> | 1 | `csmp-agent-lib/sample/ti_simplelink_wisun.target` | `SIMPLELINK_SDK_LINUX` | SDK path for board firmware builds |
> | 2 | `csmp-agent-lib/ti_simplelink_wisun.target` | `SIMPLELINK_SDK_LINUX` | SDK path for CSMP library build |
> | 3 | `<SDK>/imports.mak` | `SYSCONFIG_TOOL` + `TICLANG_ARMCOMPILER` | SysConfig and compiler tool paths |

---

### Step B.1 - Set the SDK path in the two build target files

There are two build target files that need the SDK path. Open each and update `SIMPLELINK_SDK_LINUX` to your actual SDK directory.

**File 1:** `csmp-agent-lib/sample/ti_simplelink_wisun.target`

```bash
nano /path/to/csmp-agent-lib/sample/ti_simplelink_wisun.target
```

Find and edit this line:

```makefile
# EDIT THIS — set to your SDK installation directory
SIMPLELINK_SDK_LINUX = /path/to/simplelink_cc13xx_cc26xx_sdk
```

**File 2:** `csmp-agent-lib/ti_simplelink_wisun.target` (the root-level file, one directory up from `sample/`)

```bash
nano /path/to/csmp-agent-lib/ti_simplelink_wisun.target
```

Find and edit the same variable:

```makefile
# EDIT THIS — same SDK path as above
SIMPLELINK_SDK_LINUX = /path/to/simplelink_cc13xx_cc26xx_sdk
```

> Both files must have the same SDK path. File 1 controls the board firmware builds. File 2 controls the CSMP library compilation.

---

### Step B.2 - Set the IANA Vendor ID to TI

The file `include/iana_pen.h` controls which IANA Private Enterprise Number (PEN) the device reports to FND in the VendorTLV. The default value is `CISCO`; change it to `TI` so the firmware identifies itself as a Texas Instruments device (PEN 294).

Open `include/iana_pen.h` and change line 32:

```c
/* Before */
#define VENDOR_ID CISCO

/* After */
#define VENDOR_ID TI
```

The `TI = 294` enum value is already defined in the same file. This single-line change ensures the `vendorId` field in every VendorTLV report matches the `"294"` value in the `cc13xx` FND device metadata file.

---

### Step B.3 - Run the build

Once the three files above are configured, build from the csmp-agent-lib root:

```bash
cd /path/to/csmp-agent-lib

# Make the build script executable (only needed once)
chmod 777 build.sh

# Build firmware for all supported boards with MCUBoot secure boot support
./build.sh ti_simplelink_wisun CONFIG=SECUREBOOT
```

The `CONFIG=SECUREBOOT` flag enables MCUBoot bootloader support, which is required for OTA firmware updates. Without it the build produces a standard `.hex` only; with it the post-build step also invokes `imgtool` to produce a signed `.bin` file that MCUBoot can validate and apply.

The build does three things in sequence:
1. Compiles the CSMP Agent library into `csmp_agent_lib_ti_simplelink_wisun.a`
2. Moves the `.a` file into the `sample/` directory
3. Loops over every board under `Vendors/TI/*/csmp_example_tirf/freertos/ticlang/`, running SysConfig, compiling the board-specific glue/HAL layer from source, and linking against the SDK's prebuilt Wi-SUN libraries — all five boards are built in one pass

### Step B.4 - Confirm the output

A successful build ends with output similar to:

```
########## start building...##########
...
Archiving build/debug/csmp_agent_lib_ti_simplelink_wisun.a
Done.
...
--- Building: .../Vendors/TI/LP_CC1312R7/csmp_example_tirf/freertos/ticlang/ ---
Generating configuration files... generation complete
Building application_main.obj ... linking ns_node_csmp.out ... building ns_node_csmp.hex
--- Building: .../Vendors/TI/LP_CC1352P7_1/csmp_example_tirf/freertos/ticlang/ ---
...
--- Building: .../Vendors/TI/LP_EM_CC1314R10/csmp_example_tirf/freertos/ticlang/ ---
...
--- Building: .../Vendors/TI/LP_EM_CC1354P10_1/csmp_example_tirf/freertos/ticlang/ ---
...
--- Building: .../Vendors/TI/LP_EM_CC1354P10_6/csmp_example_tirf/freertos/ticlang/ ---
...

All boards built. Output .hex files:
Vendors/TI/LP_CC1312R7/csmp_example_tirf/freertos/ticlang/ns_node_csmp.hex
Vendors/TI/LP_CC1352P7_1/csmp_example_tirf/freertos/ticlang/ns_node_csmp.hex
Vendors/TI/LP_EM_CC1314R10/csmp_example_tirf/freertos/ticlang/ns_node_csmp.hex
Vendors/TI/LP_EM_CC1354P10_1/csmp_example_tirf/freertos/ticlang/ns_node_csmp.hex
Vendors/TI/LP_EM_CC1354P10_6/csmp_example_tirf/freertos/ticlang/ns_node_csmp.hex
```

The files you will use for your specific board are:

| File | Purpose |
|------|---------|
| `Vendors/TI/<BOARD>/csmp_example_tirf/freertos/ticlang/ns_node_csmp.hex` | Full flash image — use with UniFlash for initial flashing |
| `Vendors/TI/<BOARD>/csmp_example_tirf/freertos/ticlang/ns_node_csmp.bin` | MCUBoot-signed binary — used as input to `add_tpdheader.py` for OTA |
| `Vendors/TI/<BOARD>/csmp_example_tirf/freertos/ticlang/ns_node_csmp-noheader.bin` | Intermediate file only — do not use directly |

For example, for LP-CC1352P7-1:
```
Vendors/TI/LP_CC1352P7_1/csmp_example_tirf/freertos/ticlang/ns_node_csmp.hex   ← flash this initially
Vendors/TI/LP_CC1352P7_1/csmp_example_tirf/freertos/ticlang/ns_node_csmp.bin   ← use this for OTA
```

### Step B.5 - Clean the build

To remove all build artifacts (library, objects, and board hex):

```bash
./build.sh clean
```

### Step B.6 - If the build fails

Common issues:

| Error | Fix |
|-------|-----|
| `tiarmclang: not found` | `TICLANG_ARMCOMPILER` not set in the SDK's `imports.mak` |
| `tiarmclang: Permission denied` | Run `chmod +x /path/to/ti-cgt-armllvm_X.X.X.LTS/bin/*` — execute bit missing from tarball extraction |
| `sysconfig_cli.sh: not found` | `SYSCONFIG_TOOL` not set in the SDK's `imports.mak` |
| `imports.mak: No such file` | `SIMPLELINK_SDK_LINUX` in `sample/ti_simplelink_wisun.target` or root `ti_simplelink_wisun.target` points to wrong path |
| `product.json: No such file` | SDK path is correct but the SDK is corrupted — re-install it |

---

## 6. Part C: Pre-provision the Device in Cisco FND

Before the mesh node can register, FND must have two things: the device type definition (created once per FND installation) and the device's EUI-64 identity (added once per physical device).

For the complete provisioning walkthrough with screenshots, refer to the TPD integration guide included in this repository:

```
docs/Cisco FND-OpenCSMP TPD Integration Guide.pdf
```

### Step C.1 - Create the CC13XX device type on the FND server

This step is done **once per FND installation**. It registers the CC13XX device type so FND knows how to categorize, display, and manage TI Wi-SUN devices.

SSH into the FND server and run:

```bash
cd /opt/cgms/bin
./addGenericEndpoints.sh
# When prompted for device type name, enter: cc13xx
```

The script creates the directory:
```
/opt/cgms/server/cgms/conf/endpoint-meta/cc13xx/
```

### Step C.2 - Deploy the CC13XX metadata file

Replace the generated template with the correct metadata for TI Wi-SUN devices. On the FND server, open the generated file for editing:

```bash
nano /opt/cgms/server/cgms/conf/endpoint-meta/cc13xx/cc13xxMeta.json
```

Replace the contents with exactly the following:

```json
{
    "device_info": {
        "device_type": "cc13xx",
        "device_function": "extender",
        "device_description": "TI CC13xx Wi-SUN FAN Router Node",
        "display_string": "CC13XX",
        "pids": ["OPENCSMP"],
        "vendorId": "294",
        "vendorName": "Texas Instruments",
        "configure_vendortlv": "true",
        "device_actions": [
            "reboot",
            "ping",
            "traceroute",
            "inventory"
        ],
        "hw_info": "CC13XX"
    }
}
```

**Why each field matters:**

| Field | Value | Why |
|-------|-------|-----|
| `device_type` | `"cc13xx"` | Must match the directory name and the type used when importing devices via CSV |
| `device_function` | `"extender"` | Matches `FUNCTION_RE = 2` reported in TLV 11 by the firmware — determines which category devices appear under in FND |
| `vendorId` | `"294"` | TI's IANA PEN — matches `VENDOR_ID = TI = 294` in `include/iana_pen.h`; FND displays this in the VendorTLV Info tab |
| `vendorName` | `"Texas Instruments"` | Shown alongside the vendor ID in FND |
| `configure_vendortlv` | `"true"` | Enables FND to push Vendor TLV 127 config to the device |
| `hw_info` | `"CC13XX"` | **Must match `hwid` in `tools/tpd_config_cc13xx.json` and the TPD binary header** — FND uses this to validate firmware images before assigning them to this device group |

> **Important:** `hw_info` and `hwid` must always be in sync. If you upload a firmware binary whose TPD header contains a different `hwid`, FND will refuse to assign it to the `cc13xx` group.

### Step C.3 - Restart FND to load the new device type

```bash
service cgms restart
```

FND reads the `endpoint-meta` directory at startup and creates the necessary database tables for the new device type. Any errors are logged to `/opt/cgms/logs/server.log`.

### Step C.4 - Import your device EUI-64s into FND

Create a CSV file with your device EUI-64s:

```
eid,devicetype
001234567890ABCD,cc13xx
001234567890ABCE,cc13xx
```

In the FND web UI: **Devices → Field Devices → Import** — upload the CSV. Each device now appears under the **Endpoints → CC13XX** category in the left panel, waiting for its first registration.

---

## 7. Part D: Set Up the Border Router (wfantund in Docker)

This part sets up **two Docker containers** that run on the Linux PC:
Refer to https://github.com/TexasInstruments/ti-wisunfantund for complete setup.

- **Linux Host container** — connects to the border router board over UART, runs the border router software stack, and communicates with the wfantund container over an IPv6 tunnel.
- **wfantund container** — runs the wfantund daemon, terminates the IPv6 tunnel from the Linux Host container, and creates the `wfan0` TUN interface on the Linux PC host.

Both containers are started from the `lprf-dallas-wisun-wfantund` repository using `docker-compose`.

---

## 8. Part E: Flash the TI Device (CSMP Agent Mesh Node)

### Step E.1 - Install TI UniFlash

Download UniFlash from [https://www.ti.com/tool/UNIFLASH](https://www.ti.com/tool/UNIFLASH) and install it on your machine. Alternatively, use the `dslite` command-line flasher that ships with UniFlash or Code Composer Studio.

### Step E.2 - Connect the mesh node board

Plug the LaunchPad that will run the CSMP Agent (your chosen board from the supported list) into your machine via USB.


### Step E.3 - Flash using TI UniFlash GUI

1. Open UniFlash
2. Click **"Start"** and select your device type
3. In the **"Program"** tab, browse to the `.hex` file for your board:  
   `Vendors/TI/<BOARD>/csmp_example_tirf/freertos/ticlang/ns_node_csmp.hex`
4. Click **"Load Image"**
5. Wait for "Program Load completed successfully"

### Step E.4 - Verify the device is running

Open a serial terminal to observe the device boot log:


You should see the device boot and Wi-SUN stack initializing. Keep this terminal open - you will see registration logs later.

---

## 9. Part F: Configure IPv6 Routing

This is the most critical part of the setup. The goal is to make packets flow in both directions:

- **Forward:** mesh node sends CoAP to `fd00::2` (FND) -> Linux host routes it -> FND receives it
- **Return:** FND replies to `2020:abcd::xxxx` -> FND routes to Linux host -> Linux host forwards into mesh via wfan0

There are two sub-scenarios depending on whether wfantund runs directly on the Linux host or inside Docker. **This guide covers the Docker case**, which requires more routing configuration.

### Step F.1 - Assign static IPv6 addresses (Linux host)

Both the Linux host and FND only have link-local IPv6 by default (`fe80::`). Link-local addresses do not route, so you must assign static ULA addresses.

On the **Linux host**:

```bash
# Assign a static ULA address to the physical LAN interface
# Replace "eno2" with your actual interface name (check with: ip link show)
sudo ip -6 addr add fd00::1/64 dev eno2

# Verify
ip -6 addr show eno2
# Expected output includes:
# inet6 fd00::1/64 scope global
```

To find your interface name if it is not `eno2`:

```bash
ip link show
# Look for the interface connected to the LAN (not lo, not docker0)
```

### Step F.2 - Assign static IPv6 address (FND VM)

On the **FND server** :

```bash
# Assign a static ULA address - same /64 as the Linux host
sudo ip -6 addr add fd00::2/64 dev ens192

# Verify
ip -6 addr show ens192
# Expected output includes:
# inet6 fd00::2/64 scope global
```

To find the correct interface on FND:

```bash
ip link show
# Look for the interface on the same LAN as the Linux host
```

### Step F.3 - Enable IPv6 forwarding on the Linux host

Without this, Linux host receives packets arriving on `eno2` but will not route them to the Docker bridge (`br-xxxx`), and vice versa.

```bash
sudo sysctl -w net.ipv6.conf.all.forwarding=1

# Verify
sysctl net.ipv6.conf.all.forwarding
# Expected: net.ipv6.conf.all.forwarding = 1
```

### Step F.4 - Open Docker firewall rules on the Linux host

Docker adds firewall rules that block packets from outside the Docker bridge. You need to explicitly allow traffic between the LAN interface and the Docker bridge.

First, find your Docker bridge interface name:

```bash
docker network ls
# Find the network used by the wfantund container (e.g., "wisunfan_network")

docker network inspect wisunfan_network | grep -i "br-"
# Note the bridge interface name, e.g., br-f76e88221080
```

Now add the rules (replace `br-f76e88221080` with your actual bridge name):

```bash
# Allow traffic from LAN into Docker bridge
sudo ip6tables -I DOCKER-USER -i eno2 -o br-f76e88221080 -j ACCEPT

# Allow traffic from Docker bridge back to LAN
sudo ip6tables -I DOCKER-USER -i br-f76e88221080 -o eno2 -j ACCEPT
```

### Step F.5 - Add Linux host route for Wi-SUN mesh subnet

The Linux host must know to forward packets destined for the Wi-SUN mesh (`2020:abcd::/64`) into the Docker bridge, where the wfantund container will deliver them via the `wfan0` TUN interface. Without this route, the Linux host drops packets that FND sends toward mesh nodes.

First, find the wfantund container's IPv6 address on the Docker bridge:

```bash
docker inspect wfantund | grep -i "globalipv6"
# e.g., fddb:a737:317c::4
```

Add the route (replace the container address and bridge name with your actual values from Step F.4):

```bash
sudo ip -6 route add 2020:abcd::/64 via fddb:a737:317c::4 dev br-9510734a7037
```

Verify:

```bash
ip -6 route show | grep 2020
# Expected:
# 2020:abcd::/64 via fddb:a737:317c::4 dev br-9510734a7037
```

> **Note:** This route is lost on reboot. To make it persistent, add it to `/etc/rc.local` or create a systemd network route unit.

### Step F.6 - Fix Docker's raw table DROP rule (critical)

Docker adds a `PREROUTING` rule in the `raw` table that silently drops any packet destined for a container address if the packet did NOT arrive on the Docker bridge. This runs before routing and before the filter table - it is invisible to most debugging tools. You must insert an ACCEPT rule before it.

Find your container's IPv6 address on the Docker bridge:

```bash
docker inspect wfantund | grep -i "globalipv6"
# Note the IPv6 address, e.g., fddb:a737:317c::2
```

Add the rule (replace the address with your container's actual IPv6):

```bash
sudo ip6tables -t raw -I PREROUTING -i eno2 -d fddb:a737:317c::2 -j ACCEPT
```

> **Why this matters:** Without this rule, every packet FND sends to the container's Docker bridge address is silently dropped at the kernel level before any other rule even sees it. This was the root cause of the "101 dropped packets" issue seen during development.

### Step F.7 - Save the ip6tables rules

Docker rewrites its own chains on every restart but leaves `DOCKER-USER` and the `raw` table alone. Save the rules so they survive reboots:

```bash
sudo apt install iptables-persistent   # if not already installed
sudo netfilter-persistent save
```

This writes rules to `/etc/iptables/rules.v6` and restores them at boot before Docker starts.

### Step F.8 - Add routes on FND

FND needs to know how to reach the Docker bridge subnet and the Wi-SUN mesh subnet. Both paths go via the Linux host's `fd00::1` address.

On the **FND server**:

```bash
# Route to Docker bridge (where the container lives)
sudo ip -6 route add fddb:a737:317c::/64 via fd00::1

# Route to Wi-SUN mesh nodes
sudo ip -6 route add 2020:abcd::/64 via fd00::1

# Verify routes are added
ip -6 route show | grep -E "fddb|2020"
# Expected:
# fddb:a737:317c::/64 via fd00::1 dev ens192
# 2020:abcd::/64 via fd00::1 dev ens192
```

### Step F.9 - Configure forwarding inside the container

Inside the wfantund container, enable IPv6 forwarding so it can forward between the `wfan0` TUN interface and the IPv6 tunnel (`eth0`) toward the Linux Host container. Also add a route back to FND's subnet.

```bash
docker exec -it wfantund bash

# Enable forwarding inside the container
sysctl -w net.ipv6.conf.all.forwarding=1

# Add route to FND's subnet via the Docker bridge gateway
# Replace fddb:a737:317c::1 with your Docker bridge gateway address
ip -6 route add fd00::/64 via fddb:a737:317c::1 dev eth0

# Verify
ip -6 route show
# Expected to include:
# fd00::/64 via fddb:a737:317c::1 dev eth0
```

> **Note:** These container settings are lost when the container restarts.

---

### Step F.10 - Set up NTP server on the Linux host (required for OTA scheduling)

The TI mesh node synchronises its clock via SNTP (RFC 4330) over the Wi-SUN network immediately after joining, before CSMP starts.  This is the same approach used by Silicon Labs EFR32 devices (`sl_wisun_ntp_timesync()`).  Without an accurate clock, the device may activate scheduled firmware updates several minutes late.

The Linux host (`fd00::1`) must run an NTP **server** — Ubuntu's default `systemd-timesyncd` is a client only and does not respond to queries.

#### Install and configure ntpd

```bash
sudo apt update
sudo apt install ntp -y
```

Edit `/etc/ntp.conf` and add the following lines in the `restrict` section:

```
# Allow Wi-SUN mesh nodes and the management network to query this server
restrict fd00:: mask ffff::          nomodify notrap
restrict 2020:abcd:: mask ffff:ffff:: nomodify notrap
```

The full restrict block should look like this:

```
restrict default kod nomodify notrap nopeer noquery
restrict 127.0.0.1
restrict ::1
restrict fd00:: mask ffff::          nomodify notrap
restrict 2020:abcd:: mask ffff:ffff:: nomodify notrap
```

#### Start and enable the service

```bash
sudo systemctl enable ntp
sudo systemctl start ntp
sudo systemctl status ntp
```

#### Allow UDP port 123 through any local firewall

```bash
# UFW (if enabled)
sudo ufw allow 123/udp

# Or directly via ip6tables
sudo ip6tables -I INPUT -p udp --dport 123 -j ACCEPT
```

#### Verify ntpd is listening on IPv6

```bash
ss -ulnp | grep 123
# Expected output includes a line with :::123 (IPv6 wildcard)
```

#### Verify ntpd has synchronised with upstream

```bash
ntpq -p
# Look for a server with * (selected) or + (candidate) in the first column.
# ntpd takes 5-10 minutes on first start to reach synchronisation.
```

#### Test that the NTP server responds

From the Linux host itself:

```bash
ntpdate -q fd00::1
# Expected: server fd00::1, stratum N, offset X.XXXX, delay Y.YYYY
```

Once ntpd is running, the TI mesh node will print the following at boot (visible on the serial terminal):

```
[INFO][sntp]: sntp: request sent to [fd00::1]:123
[INFO][sntp]: sntp: clock set to Unix=1780466595 (NTP=3989455395)
[INFO][main]: CSMP: clock synchronised via SNTP — epoch=1780466595
```

If the NTP server is not reachable, the device falls back to `CSMP_BUILD_EPOCH` (the Unix timestamp injected at compile time), which is accurate to within seconds of the actual build date.

---

## 10. Part G: Verify End-to-End Connectivity

Run these checks in order. Each one tests one hop in the chain.

### Step G.1 - Linux host can reach FND

```bash
# On Linux host
ping6 fd00::2
# Expected: 64 bytes from fd00::2  icmp_seq=1 ttl=64 time=X ms
```

### Step G.2 - FND can reach Linux host

```bash
# On FND VM
ping6 fd00::1
# Expected: 64 bytes from fd00::1  icmp_seq=1 ttl=64 time=X ms
```

### Step G.3 - FND can reach the Docker container

```bash
# On FND VM
ping6 fddb:a737:317c::2
# Expected: 64 bytes from fddb:a737:317c::2  icmp_seq=1 ttl=63 time=X ms
# (ttl=63 because it hops through the Linux host)
```

If this fails, the most likely cause is the raw table DROP rule (Step F.5) was not applied.

### Step G.4 - Container can reach FND

```bash
# Inside the wfantund container
docker exec -it wfantund ping6 fd00::2
# Expected: 64 bytes from fd00::2  icmp_seq=1 ttl=64 time=X ms
```

---

## 11. What Success Looks Like (Logs)

### wfantund container logs (border router side)

After the mesh node powers on and the Wi-SUN network is operational, you should see the mesh node join the network. Inside the wfantund container:

```
[wfantund] INFO: Node joined: 2020:abcd::0212:4b00:1234:5678
[wfantund] INFO: Routing table updated: 2020:abcd::0212:4b00:1234:5678/128
```

### Mesh node serial terminal logs (CSMP Agent)

On the serial terminal connected to the mesh node board, you should see:

```
[Wi-SUN] INFO: Stack initialized
[Wi-SUN] INFO: Joining network "CSMP_WISUN"...
[Wi-SUN] INFO: Network joined. Assigned address: 2020:abcd::0212:4b00:1234:5678
[CSMP] INFO: Starting CSMP Agent
[CSMP] INFO: NMS address: fd00::2
[CSMP] INFO: Sending registration to fd00::2 ...
[CSMP] INFO: Registration response received (200 OK)
[CSMP] INFO: Registered successfully. Next registration in 600s
```

The key line is `Registration response received (200 OK)`. This means:
1. The mesh node sent a CoAP POST to FND at `fd00::2`
2. The packet traveled: mesh node -> border router -> Linux Host container -> wfantund container -> wfan0 -> Linux PC -> FND
3. FND accepted the registration and replied
4. The reply traveled: FND -> Linux host -> container -> border router -> mesh node
5. The mesh node received the acknowledgment

### Cisco FND UI

Log into the FND web interface. Navigate to **Devices > Field Devices**. Your mesh node should appear with:
- Status: **Up**
- IP Address: `2020:abcd::0212:4b00:xxxx`
- Last Registration: recent timestamp

---

## 12. Part H: OTA Firmware Update

This section covers upgrading the firmware on a running mesh node over the air using Cisco FND. OTA requires MCUBoot to be present on the device, which is why the `CONFIG=SECUREBOOT` flag is used in the build.

The process has three phases: build and flash the initial image, build the upgrade image, and trigger the update through FND.

---

### Phase 1: Build and Flash the Initial Image

This is the baseline firmware that the device runs before any OTA. It must be flashed physically via UniFlash.

#### Step H.1 - Build the initial image

From the csmp-agent-lib root directory:

```bash
./build.sh ti_simplelink_wisun CONFIG=SECUREBOOT
```

The build produces two files per board. For LP-CC1352P7-1:
- `Vendors/TI/LP_CC1352P7_1/csmp_example_tirf/freertos/ticlang/ns_node_csmp.hex` — full flash image for UniFlash
- `Vendors/TI/LP_CC1352P7_1/csmp_example_tirf/freertos/ticlang/ns_node_csmp.bin` — MCUBoot-signed binary used for OTA

The version embedded in the initial image defaults to `1.0.0` as set in the board makefile's `imgtool` post-build step.

#### Step H.2 - Flash MCUBoot and the initial image

MCUBoot must be flashed to the device before the application. Flash in this order using UniFlash:

1. Open UniFlash and connect your board.
2. **First — add the MCUBoot image:**
   - Click **Add file** and select the MCUBoot binary from the SimpleLink SDK:
     ```
     <SDK>/examples/nortos/<BOARD>/mcuboot_app/mcuboot/freertos/ticlang/mcuboot.hex
     ```
   - Leave the address at the default (MCUBoot's linker script sets its own load address at the top of flash).

3. **Second — add the CSMP application image:**
   - Click **Add file** and select:
     ```
     Vendors/TI/LP_CC1352P7_1/csmp_example_tirf/freertos/ticlang/ns_node_csmp.hex
     ```

4. Click **Load Images** to flash both files to the device.

5. The device boots. MCUBoot runs first, validates the application, then hands control to the CSMP firmware. You should see the Wi-SUN stack initialize on the serial terminal and the device register with FND as described in Section 11.

---

### Phase 2: Build the Upgrade Image

The upgrade image must have a **strictly higher version** than the currently running image. MCUBoot rejects any image with a version equal to or lower than what is already running.

#### Step H.3 - Update the version string in `CsmpAgentLib_sample.c`

Open `sample/CsmpAgentLib_sample.c` and find the `default_run_slot_image` line (around line 49):

```c
Csmp_Slothdr default_run_slot_image = {{0x61,0xe7,...},
"opencsmp-node-1.0.00","1.0.00", "CC13XX", 27904, 0, 0, 0, 0, 0, 0, {0},0, 0};
```

Update the filename and version strings to match what you are about to build. For example, if the upgrade version will be `2.0.0`:

```c
Csmp_Slothdr default_run_slot_image = {{0x61,0xe7,...},
"opencsmp-node-2.0.00","2.0.00", "CC13XX", 27904, 0, 0, 0, 0, 0, 0, {0},0, 0};
```

> This controls what version FND reads from **TLV 75 FirmwareImageInfo** after the OTA completes and the device reboots. It must match the `--version` argument in the next step.

#### Step H.4 - Update the imgtool version in the board makefile

Open the makefile for your board. For LP-CC1352P7-1:

```bash
nano Vendors/TI/LP_CC1352P7_1/csmp_example_tirf/freertos/ticlang/makefile
```

Find the `imgtool sign` line in the `postbuild` section:

```makefile
$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/tools/common/mcuboot/imgtool sign \
    --header-size 0x80 --align 4 --slot-size 0xA8000 --pad \
    --version 1.0.0 \
    --pad-header \
    --key $(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mcuboot/root-ec-p256.pem \
    $(NAME)-noheader.bin $(NAME).bin;
```

Change `--version 1.0.0` to a version **higher** than what is currently running on the device:

```makefile
    --version 2.0.0 \
```

> The version here must be strictly greater than the running version. `2.0.0 > 1.0.0` ✓. MCUBoot compares major → minor → revision in order. The build number is ignored.

#### Step H.5 - Build the upgrade image

```bash
./build.sh ti_simplelink_wisun CONFIG=SECUREBOOT
```

The output `.bin` is now signed with version `2.0.0`:
```
Vendors/TI/LP_CC1352P7_1/csmp_example_tirf/freertos/ticlang/ns_node_csmp.bin
```

#### Step H.6 - Update `tpd_config_cc13xx.json` to match the upgrade version

Before wrapping the binary, update `tools/tpd_config_cc13xx.json` so the metadata embedded in the TPD header matches the version you just built. FND reads this header to display the image name and version in its firmware list.

```bash
nano tools/tpd_config_cc13xx.json
```

The file looks like this — edit `major`, `minor`, `build`, and `name` to match the version set in Step H.4:

```json
{
    "major": "2",
    "minor": "0",
    "build": "0",
    "name": "opencsmp-node-2.0.00",
    "hwid": "CC13XX",
    "sub_hwid": "0",
    "branch": "main",
    "commit": "0000000",
    "date": "Jun 02 2026",
    "kernelrev": "WISUN_2.0"
}
```

Fields to update every time you build a new upgrade image:

| Field | What to set | Example |
|-------|-------------|---------|
| `major` | Major version number (string) | `"2"` |
| `minor` | Minor version number (string) | `"0"` |
| `build` | Build/patch number (string) | `"0"` |
| `name` | Human-readable image name shown in FND | `"opencsmp-node-2.0.00"` |
| `date` | Build date (cosmetic only) | `"Jun 02 2026"` |

> **Keep `hwid`, `sub_hwid`, `branch`, and `kernelrev` unchanged.** `hwid` must always stay `"CC13XX"` to match the `hw_info` field in `cc13xxMeta.json` on the FND server. Changing it will cause FND to reject the image when you try to assign it to the `cc13xx` device group.

Save and close the file before running `add_tpdheader.py`.

#### Step H.7 - Wrap the image with the CSMP TPD header

FND requires the firmware binary to have a 256-byte CSMP metadata header prepended before it can be uploaded. Use the `add_tpdheader.py` tool for this.

Run from the csmp-agent-lib root directory:

```bash
python3 tools/add_tpdheader.py \
    Vendors/TI/LP_CC1352P7_1/csmp_example_tirf/freertos/ticlang/ns_node_csmp.bin \
    ota_upgrade_v2.bin \
    tools/tpd_config_cc13xx.json
```

Replace the arguments with your actual paths and preferred output filename:
- **Argument 1** — path to the `.bin` built in Step H.5
- **Argument 2** — name for the output file that you will upload to FND (any name ending in `.bin`)
- **Argument 3** — TPD config file (`tools/tpd_config_cc13xx.json` contains the `hwid: "CC13XX"` and version metadata)

This creates `ota_upgrade_v2.bin` in the current directory.

#### Step H.8 - Transfer the image to the FND server

Copy the wrapped binary to the machine where you access FND (or to any machine with a browser that can reach the FND web UI):

```bash
scp ota_upgrade_v2.bin user@fnd-server:/tmp/
```

---

### Phase 3: Trigger the OTA from FND

#### Step H.9 - Upload the firmware image to FND

1. Log in to the Cisco FND web interface.
2. Navigate to **CONFIG → Firmware Update**.
3. In the left panel, click **Images**, then select **PLC-RF** from the firmware image categories.
4. Click the **+** button next to the "FIRMWARE IMAGES" heading.
5. In the dialog that appears, click **Browse**, select `ota_upgrade_v2.bin` from your machine.
6. Click **Add File**. FND reads the 256-byte header and registers the image name and version.

#### Step H.10 - Assign the image to a device group and start upload

1. In the left panel, click **Groups**.
2. Select the device group that contains your mesh node (e.g., `default-cc13xx`).
3. In the right panel, click **Upload Image**.
4. In the dialog:
   - **Select Type:** choose `PLC-RF`
   - **Select an Image:** choose the image you uploaded in Step H.8 (`ota_upgrade_v2.bin`)
5. Click **Upload Image** to start sending blocks to the device.

#### Step H.11 - Monitor the OTA progress

On the same firmware group page, click the **Logs** tab. You will see progress similar to:

```
Sent block 1/1060
Sent block 2/1060
...
Sent block 1060/1060
Upload Finished
```

The **Status** column on the Firmware Management page also updates as the transfer progresses. Click **Refresh** periodically to get the latest status. The current status values are:

| Status | Meaning |
|--------|---------|
| `Upload Finished` | All blocks sent to the device; device has confirmed receipt |
| `Reload Scheduled` | FND has sent a LoadRequest; device activation timer is running |
| `Reload Scheduling Finished` | Device acknowledged the load request |

> You can also monitor block reception on the device's serial terminal. Look for `OTA: block N written` and `OTA: download complete!` log lines.

#### Step H.12 - Reboot the device to apply the new firmware

After the upload is complete, schedule a reboot via FND so MCUBoot can apply the image from external flash.

1. Navigate to **Config → Firmware Update**.
2. Scroll down to the **Image List** table.
3. Scroll horizontally to the last column — the **Actions** column.
4. Click the **calendar icon** next to your device to schedule a reboot.
5. In the scheduler dialog, note the FND server's time zone and set the reboot time accordingly.

The device reboots at the scheduled time. MCUBoot runs, finds the new image in external flash, verifies the ECDSA-P256 signature, copies it to internal flash, and boots it.

#### Step H.13 - Verify the upgrade

After the device rejoins the Wi-SUN network and re-registers with FND (allow 60–90 seconds):

1. In **Devices → Field Devices**, open the device detail page.
2. Check the **Firmware** tab — the running firmware version should now show `2.0.0` (or whatever version you set in Steps H.3–H.4).
3. Alternatively, run `getoadfwver` from the border router if using pySpinel, or check the TLV 75 FirmwareImageInfo response in FND's Troubleshooting tab.

---

### OTA Quick Reference

| Step | Action | Where |
|------|--------|-------|
| H.3 | Update version in `default_run_slot_image` | `sample/CsmpAgentLib_sample.c` line ~49 |
| H.4 | Update `--version` in imgtool command | `Vendors/TI/<BOARD>/.../makefile` postbuild section |
| H.5 | Build | `./build.sh ti_simplelink_wisun CONFIG=SECUREBOOT` |
| H.6 | Update `tpd_config_cc13xx.json` version fields | `tools/tpd_config_cc13xx.json` — match major/minor/build/name to H.4 version |
| H.7 | Wrap with TPD header | `python3 tools/add_tpdheader.py <in.bin> <out.bin> tools/tpd_config_cc13xx.json` |
| H.8 | Copy to FND host | `scp <out.bin> user@fnd-server:/tmp/` |
| H.9 | Upload image to FND | CONFIG → Firmware Update → Images → PLC-RF → + |
| H.10 | Assign to group | CONFIG → Firmware Update → Groups → Upload Image |
| H.11 | Monitor | Logs tab on the group firmware page |
| H.12 | Reboot device | Config → Firmware Update → Image List → Actions column → calendar icon |
| H.13 | Verify | Device detail → Firmware tab shows new version |

