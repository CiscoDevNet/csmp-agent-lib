# Changelog

All notable changes to OpenCSMP are documented here.

This history was reconstructed from the repository's Git tags and commit history through [`6dbd890`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/6dbd890). The release sections and the changes inside each section are ordered by the commit date recorded by Git, newest-first.

The annotated tags from `v0.1.0` through `v1.0.0` have tagger metadata dated 2025-07-10, so those tag-creation dates are not used as release dates. Release dates below refer to the tagged commit's Git author date (`%aI`); the dated entries use the same convention.

## Latest

Changes on `main` after [`v1.1.0`](https://github.com/CiscoDevNet/csmp-agent-lib/releases/tag/v1.1.0).

- **2026-08-24** — Expanded and reorganized the README with OpenCSMP branding, architecture, build, integration, security, testing, and Wireshark guidance; added branding assets and CSMP packet captures ([`6dbd890`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/6dbd890)).

## [1.1.0] — 2026-08-17

Tag target: [`2548fd1`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/2548fd1) (author date 2026-08-17; committer date 2026-08-19)

- **2026-08-17** — Completed the CSMP dissector v2.0.0 documentation, cleanup, feature tests, and regression tests ([`2548fd1`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/2548fd1)).
- **2026-08-14** — Added further dissector enhancements, hardening, and helper scripts ([`4a52a33`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/4a52a33)).
- **2026-08-11** — Enhanced, hardened, and cleaned up the Wireshark dissector ([`4c9aa9c`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/4c9aa9c)).
- **2026-07-24** — Added Renesas Wi-SUN FAN platform integration ([`7e6ba60`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/7e6ba60)).
- **2026-07-15** — Added additional CSMP TLVs and corresponding Protobuf updates; cleaned up related code ([`acdc566`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/acdc566)).
- **2026-07-13** — Added the CoAP/CSMP Wireshark dissector ([`75db486`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/75db486)).
- **2026-07-03** — Added support for application-managed external receive tasks ([`be461cd`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/be461cd)).
- **2026-07-02** — Added externally callable CoAP processing functions ([`c323c99`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/c323c99)), MbedTLS signature verification ([`972e8e2`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/972e8e2)), and the OSAL socket-close abstraction ([`ce9bb95`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/ce9bb95)).
- **2025-08-01** — Added the remaining documentation files ([`a7952a3`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/a7952a3), [`a0a81e0`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/a0a81e0)).
- **2025-07-26 to 2025-07-24** — Organized the documentation and image assets, added the project logo, and renamed the FND/TPD documents ([`64a3a2a`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/64a3a2a), [`61e1d3b`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/61e1d3b), [`ae1e0f7`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/ae1e0f7), [`0360729`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/0360729), [`4535f96`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/4535f96), [`646da5a`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/646da5a), [`63baae5`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/63baae5)).
- **2025-07-23** — Added and refined project images, documentation structure, and Wireshark guidance ([`134c49d`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/134c49d), [`85cd19a`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/85cd19a), [`7556aa4`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/7556aa4), [`b30a852`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/b30a852), [`cdf87db`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/cdf87db), [`b7b2ed6`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/b7b2ed6), [`c84e85b`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/c84e85b)).

## [1.0.0] — 2025-06-26

Tag target: [`0983798`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/0983798)

- **2025-06-26** — Added the OSAL firmware-update implementation and TLV32 reboot support for Silicon Labs EFR32 Wi-SUN devices ([`0983798`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/0983798)).

## [0.9.0] — 2025-04-25

Tag target: [`7b3213e`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/7b3213e)

- **2025-04-25** — Added Linux OSAL APIs for firmware reading, firmware writing, system reboot, and TLV32 reboot handling ([`7b3213e`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/7b3213e)).

## [0.8.0] — 2025-03-31

Tag target: [`28a01f0`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/28a01f0)

- **2025-03-31** — Added Cisco FND-compatible TPD headers and the associated firmware-format specification ([`28a01f0`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/28a01f0)).

## [0.7.0] — 2024-10-25

Tag target: [`597f3b3`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/597f3b3)

- **2024-10-25** — Completed the firmware-upgrade, Vendor TLV, and Linux OSAL integration ([`597f3b3`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/597f3b3)), after adding the second phase of FND firmware-upgrade integration ([`325d802`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/325d802)).
- **2024-10-01** — Added the repository security policy ([`5b765e8`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/5b765e8)).
- **2024-09-13** — Added firmware-upgrade and Vendor TLV FND integration and related fixes ([`2eca7e8`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/2eca7e8)).

## [0.6.0] — 2024-07-29

Tag target: [`9b634d5`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/9b634d5)

- **2024-07-29** — Added initial Silicon Labs EFR32 Wi-SUN platform support ([`9b634d5`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/9b634d5)).

## [0.5.0] — 2024-06-03

Tag target: [`5f9b16c`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/5f9b16c)

- **2024-06-03** — Added the FreeRTOS-on-Linux OSAL port ([`5f9b16c`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/5f9b16c)).
- **2024-05-17** — Cleaned up and standardized the OSAL API ([`558f3e7`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/558f3e7)).
- **2024-05-03** — Added the OSAL allocator API ([`28e5d58`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/28e5d58)).
- **2024-04-18** — Removed committed binaries and added repository ignore rules ([`dafd2bc`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/dafd2bc)).

## [0.4.0] — 2024-01-24

Tag target: [`2682c37`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/2682c37)

- **2024-01-24** — Merged the Linux OSAL work into the release line ([`2682c37`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/2682c37)); the initial implementation and platform abstraction files were added in ([`23fd9d8`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/23fd9d8), [`f6960ac`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/f6960ac)).

## [0.3.0] — 2023-09-29

Tag target: [`4f2c2f8`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/4f2c2f8)

- **2023-09-29** — Merged the encoding-fix branch ([`4f2c2f8`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/4f2c2f8)) after fixing file-encoding issues ([`a712f22`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/a712f22)).
- **2023-09-28** — Added Vendor and Signature Settings TLVs ([`bfc5af2`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/bfc5af2)), after adding Signature and CGMS TLVs ([`945ef69`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/945ef69)).

## [0.2.0] — 2023-06-05

Tag target: [`0131ab2`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/0131ab2)

- **2023-06-05** — Completed the Developer Guide addition ([`0131ab2`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/0131ab2)).
- **2023-06-01** — Refreshed the README and added the documentation directory and Developer Guide content ([`67af915`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/67af915), [`f111b40`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/f111b40), [`0f6fee2`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/0f6fee2), [`88f0efd`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/88f0efd), [`42aaf34`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/42aaf34), [`970ca04`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/970ca04)).
- **2023-05-24 to 2023-05-18** — Resolved compiler warnings and a debug-print integer type mismatch ([`754feb1`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/754feb1), [`2661632`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/2661632), [`773e1a9`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/773e1a9), [`faf12b2`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/faf12b2), [`10d1c5a`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/10d1c5a)).
- **2023-05-17** — Fixed a segmentation fault caused by a function prototype/body mismatch ([`2db7eb5`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/2db7eb5)).
- **2023-05-02** — Updated the README ([`cd4d424`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/cd4d424)).
- **2022-12-06** — Enabled strict compiler warnings ([`d42a3ea`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/d42a3ea)).

## [0.1.0] — 2022-03-01

Tag target: [`f1697dc`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/f1697dc)

- **2022-03-01** — Merged the license branch; this is the commit targeted by the `v0.1.0` tag ([`f1697dc`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/f1697dc)).
- **2021-12-09** — Published the first stable source release, as recorded in the legacy changelog ([`b6f8657`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/b6f8657)).
- **2021-11-23** — Created the initial repository ([`04dd25d`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/04dd25d)).

## Versioning and date notes

- Tags present in the repository: `v0.1.0`, `v0.2.0`, `v0.3.0`, `v0.4.0`, `v0.5.0`, `v0.6.0`, `v0.7.0`, `v0.8.0`, `v0.9.0`, `v1.0.0`, and `v1.1.0`.
- The `v1.1.0` tag includes the Wireshark dissector milestone named `v2.0.0` in its commit message; the repository release remains `v1.1.0`.
- The legacy changelog calls the 2021-12-09 source snapshot `csmp-agent-lib-1.0.0`. The repository's current Git tag for the initial release line is `v0.1.0`, targeting [`f1697dc`](https://github.com/CiscoDevNet/csmp-agent-lib/commit/f1697dc). Both records are preserved rather than silently merging the two dates or version names.

[Unreleased]: https://github.com/CiscoDevNet/csmp-agent-lib/compare/v1.1.0...main
[1.1.0]: https://github.com/CiscoDevNet/csmp-agent-lib/releases/tag/v1.1.0
[1.0.0]: https://github.com/CiscoDevNet/csmp-agent-lib/releases/tag/v1.0.0
[0.9.0]: https://github.com/CiscoDevNet/csmp-agent-lib/releases/tag/v0.9.0
[0.8.0]: https://github.com/CiscoDevNet/csmp-agent-lib/releases/tag/v0.8.0
[0.7.0]: https://github.com/CiscoDevNet/csmp-agent-lib/releases/tag/v0.7.0
[0.6.0]: https://github.com/CiscoDevNet/csmp-agent-lib/releases/tag/v0.6.0
[0.5.0]: https://github.com/CiscoDevNet/csmp-agent-lib/releases/tag/v0.5.0
[0.4.0]: https://github.com/CiscoDevNet/csmp-agent-lib/releases/tag/v0.4.0
[0.3.0]: https://github.com/CiscoDevNet/csmp-agent-lib/releases/tag/v0.3.0
[0.2.0]: https://github.com/CiscoDevNet/csmp-agent-lib/releases/tag/v0.2.0
[0.1.0]: https://github.com/CiscoDevNet/csmp-agent-lib/releases/tag/v0.1.0
