# Changelog

Notable changes to the CoAP CSMP Wireshark dissector are documented here.

## [2.0.0]

### Added

- Added a comprehensive `README.md` covering installation, configuration,
  usage, troubleshooting, protocol context, and the TLV-extension workflow.
- Added `CHANGELOG.md` with detailed release history.
- Added a PDF companion to the installation, user, and developer guide.

### Changed

- Cleaned up the Lua dissector implementation, comments, and presentation
  without changing the supported CSMP wire formats.
- Clarified the RFC and draft-12 labels shown for the default CSMP UDP ports in
  the protocol preferences.
- Expanded and reorganized the project documentation for end users,
  developers, test teams, support engineers, and field engineers.

### Validation

- Completed feature and regression testing of the dissector.
- Tested Auto, Force RFC, and Force Draft CoAP mode selection.
- Tested `add_tlv.sh`, including its validation and ordered-insertion workflow.

## [1.9.0]

### Added

- Added cross-platform installers for macOS, Linux, Windows PowerShell, and
  Windows Command Prompt; the Command Prompt installer was tested on Windows
  7.
- Added automatic Wireshark personal-plugin-directory detection, custom target
  selection, dry-run support, and installation verification.
- Added `add_tlv.sh` for safely adding Protobuf-backed TLVs with input
  validation, duplicate detection, dry-run support, and rollback protection.

### Changed

- Renamed the plugin to `coap_csmp_dissector.lua`.
- Inserted new Protobuf definitions in ascending TLV ID order.

### Fixed

- Prevented Empty CoAP messages from being interpreted as CSMP payloads.
- Added malformed-packet diagnostics for invalid RFC Empty messages.
- Ignored trailing data in noncanonical draft-12 Empty messages and reported a
  protocol warning.

### Documentation

- Expanded installation, configuration, troubleshooting, and TLV-extension
  instructions.

## [1.8.0]

### Added

- Added a protocol preference for automatic, forced RFC, or forced draft-12
  CoAP wire-format selection.
- Added protocol-preference information for the CSMP UDP ports and project
  documentation.

### Changed

- Hardened automatic mode detection using destination ports, source ports for
  responses, and the header as a fallback for Decode As or nonstandard ports.

## [1.7.0]

### Added

- Added CoAP option parsing and option metadata display.
- Added RFC token decoding and draft token-option decoding.
- Added URI path reconstruction.
- Added payload-marker and payload extraction.
- Added malformed-packet diagnostics for truncated headers, tokens, options,
  VarInts, and TLV payload overruns.

### Fixed

- Added uint32 overflow and truncation handling for CSMP VarInts.
- Added CoAP version validation.

## [1.6.0]

### Changed

- Consolidated TLV display names and Protobuf message names into the common
  `protobufMessageMap` table.

## [1.5.0]

### Added

- Expanded supported CSMP TLV definitions and updated `csmp.proto`.

## [1.4.0]

### Added

- Added CoAP RFC and draft-12 mode differentiation.
- Added CoAP fixed-header field parsing.

## [1.3.0]

### Added

- Added custom CoAP header decoding to the CSMP dissector.

## [1.2.0]

### Added

- Added support for the CoAP draft-12 wire format on UDP port `61624`.

## [1.1.0]

### Added

- Added selected CSMP TLV definitions and corresponding Protobuf decoding.

### Changed

- Updated `csmp.proto` with the selected TLV messages.

## [1.0.0]

### Added

- Initial Wireshark Lua dissector release.
- Added support for the RFC CoAP wire format on UDP port `61628`.
- Added CSMP TLV parsing and Wireshark Protobuf dissection.
