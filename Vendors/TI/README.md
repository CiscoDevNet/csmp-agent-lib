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

The CSMP Agent firmware needs to know the IPv6 address of your FND server at **compile time**. This setting applies to all boards from a single shared file.

### Step A.1 - Open router.opts

```bash
# Run from the csmp-agent-lib root directory
cd /path/to/csmp-agent-lib
nano Vendors/TI/application/defines/router.opts
```

**File location:** `csmp-agent-lib/Vendors/TI/application/defines/router.opts`

This file is shared by all boards. Changing it once updates the NMS address for every board in the next build.

### Step A.2 - Set the FND address

Find this line:

```
-DCSMP_AGENT_NMS_ADDRESS=\"fd00::3\"
```

Replace `fd00::3` with your FND server's actual IPv6 address:

```
-DCSMP_AGENT_NMS_ADDRESS=\"<your-FND-IPv6-address>\"
```

For example, if FND is at `fd00::2`:

```
-DCSMP_AGENT_NMS_ADDRESS=\"fd00::2\"
```

Save and close (`Ctrl+X`, `Y`, `Enter` in nano).

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

### Step B.2 - Run the build

Once the three files above are configured, build from the csmp-agent-lib root:

```bash
cd /path/to/csmp-agent-lib

# Make the build script executable (only needed once)
chmod 777 build.sh

# Build firmware for all supported boards
./build.sh ti_simplelink_wisun
```

The build does three things in sequence:
1. Compiles the CSMP Agent library into `csmp_agent_lib_ti_simplelink_wisun.a`
2. Moves the `.a` file into the `sample/` directory
3. Loops over every board under `Vendors/TI/*/csmp_example_tirf/freertos/ticlang/`, running SysConfig, compiling the board-specific glue/HAL layer from source, and linking against the SDK's prebuilt Wi-SUN libraries — all five boards are built in one pass

### Step B.3 - Confirm the output

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

The file you will flash to your specific board is:

```
Vendors/TI/<BOARD>/csmp_example_tirf/freertos/ticlang/ns_node_csmp.hex
```

For example, for LP-CC1352P7-1:
```
Vendors/TI/LP_CC1352P7_1/csmp_example_tirf/freertos/ticlang/ns_node_csmp.hex
```

### Step B.4 - Clean the build

To remove all build artifacts (library, objects, and board hex):

```bash
./build.sh clean
```

### Step B.5 - If the build fails

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

Before the mesh node can register, FND must have the device's identity in its database.

For full device provisioning steps, refer to the TPD integration guide included in this repository:

```
docs/Cisco FND-OpenCSMP TPD Integration Guide.pdf
```

When adding the device in FND, register it as device type **CC13XX**.

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

### Step F.5 - Fix Docker's raw table DROP rule (critical)

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

### Step F.6 - Save the ip6tables rules

Docker rewrites its own chains on every restart but leaves `DOCKER-USER` and the `raw` table alone. Save the rules so they survive reboots:

```bash
sudo apt install iptables-persistent   # if not already installed
sudo netfilter-persistent save
```

This writes rules to `/etc/iptables/rules.v6` and restores them at boot before Docker starts.

### Step F.7 - Add routes on FND

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

### Step F.8 - Configure forwarding inside the container

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

