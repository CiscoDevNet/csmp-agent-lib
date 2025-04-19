#!/usr/bin/env python3

# add_tpdheader.py
#
# Helper script to add Cisco FND compatible header
# to TPD firmware images for FND firmware upgrade
#
# usage: add_tpdheader.py [-h] input_file output_file config_file
#        input_file   Input TPD binary file
#        output_file  Output binary file with TPD header
#        config_file  JSON configuration file

import os
import struct
import argparse
import json
from datetime import date

class TPD_CRC(object):
    @staticmethod
    def __crc32_msb_byte(byte, rpolynom, crc):
        crc = crc ^ ((byte << 24) & 0xFFFFFFFF)
        for _ in range(8):
            if crc & 0x80000000 != 0:
                crc = ((crc << 1) & 0xFFFFFFFF) ^ rpolynom
            else:
                crc = ((crc << 1) & 0xFFFFFFFF)
        return crc

    @staticmethod
    def tpd_crc32(buf, rpolynom=0x04C11DB7, crc=0):
        for i in range(len(buf)):
            crc = TPD_CRC.__crc32_msb_byte(buf[i], rpolynom, crc)
        return crc

class TPD_AppHeader(object):
    def __init__(self):
        self.hdr = {'hdr_version': 2,
                    'hdr_len': 256,
                    'git_flag': 1
                    }
        self.crc = False

    def set_major(self, x):
        self.hdr['major'] = int(x)
    def set_minor(self, x):
        self.hdr['minor'] = int(x)
    def set_build(self, x):
        self.hdr['build'] = int(x)
    def set_name(self, x):
        self.hdr['name'] = x
    def set_hwid(self, x):
        self.hdr['hwid'] = x
    def set_branch(self, x):
        self.hdr['branch'] = x
    def set_commit(self, x):
        self.hdr['commit'] = x
    def set_date(self, x):
        self.hdr['date'] = x
    def set_sub_hwid(self, x):
        self.hdr['sub_hwid'] = x
    def set_kernel_rev(self, x):
        self.hdr['kernel_rev'] = x
    def set_filesize(self, x):
        self.filesize = x

    def dumphex(self):
        print(" ".join(c.hex().upper() for c in self.serialize()))

    def add_tpd_crc(self):
        self.crc = True
    def del_tpd_crc(self):
        self.crc = False

    def import_header(self, bytes):
        if len(bytes) != 256:
            raise ValueError("Header Length should be 256 bytes")

        i = 0
        self.hdr['hdr_version'] = struct.unpack("<I", bytes[i:i+4])[0]
        i += 4
        self.hdr['hdr_len'] = struct.unpack("<I", bytes[i:i+4])[0]
        i += 4
        self.hdr['major'] = struct.unpack("<I", bytes[i:i+4])[0]
        i += 4
        self.hdr['minor'] = struct.unpack("<I", bytes[i:i+4])[0]
        i += 4
        self.hdr['build'] = struct.unpack("<I", bytes[i:i+4])[0]
        i += 4
        # skip app_len
        i += 4
        self.hdr['name'] = bytes[i:i+32].split(b"\x00")[0].decode()
        i += 32
        self.hdr['branch'] = bytes[i:i+32].split(b"\x00")[0].decode()
        i += 32
        self.hdr['commit'] = bytes[i:i+8].split(b"\x00")[0].decode()
        i += 8
        self.hdr['git_flag'] = struct.unpack("<I", bytes[i:i+4])[0]
        i += 4
        self.hdr['date'] = bytes[i:i+16].split(b"\x00")[0].decode()
        i += 16
        self.hdr['hwid'] = bytes[i:i+32].split(b"\x00")[0].decode()
        i += 32
        self.hdr['sub_hwid'] = bytes[i:i+32].split(b"\x00")[0].decode()
        i += 32
        self.hdr['kernel_rev'] = bytes[i:i+16].split(b"\x00")[0].decode()
        i += 16

    def serialize(self):
        app_hdr = bytearray()

        app_hdr += struct.pack("<I", self.hdr['hdr_version'])
        app_hdr += struct.pack("<I", self.hdr['hdr_len'])
        app_hdr += struct.pack("<I", self.hdr['major'])
        app_hdr += struct.pack("<I", self.hdr['minor'])
        app_hdr += struct.pack("<I", self.hdr['build'])
        app_hdr += struct.pack("<I", self.filesize + self.hdr['hdr_len']) 
        app_hdr += self.hdr['name'].encode()[:32] + (b"\x00" * (32 - len(self.hdr['name'])))
        app_hdr += self.hdr['branch'].encode()[:32] + (b"\x00" * (32 - len(self.hdr['branch'])))
        app_hdr += self.hdr['commit'].encode()[:8] + (b"\x00" * (8 - len(self.hdr['commit'])))
        app_hdr += struct.pack("<I", self.hdr['git_flag'])
        app_hdr += self.hdr['date'].encode()[:16] + (b"\x00" * (16 - len(self.hdr['date'])))
        app_hdr += self.hdr['hwid'].encode()[:32] + (b"\x00" * (32 - len(self.hdr['hwid'])))
        app_hdr += self.hdr['sub_hwid'].encode()[:32] + (b"\x00" * (32 - len(self.hdr['sub_hwid'])))
        app_hdr += self.hdr['kernel_rev'].encode()[:16] + (b"\x00" * (16 - len(self.hdr['kernel_rev'])))

        app_hdr += b"\x00" * (252 - len(app_hdr)) # 252 because last 4 bytes for Header CRC

        if self.crc:
            return app_hdr + struct.pack("<I", TPD_CRC.tpd_crc32(app_hdr))
        return app_hdr

def main():
    parser = argparse.ArgumentParser(description='Add Cisco FND compatible header to TPD firmware image')
    parser.add_argument('input_file', help="Input TPD binary file")
    parser.add_argument('output_file', help="Output binary file with TPD header")
    parser.add_argument('config_file', help="JSON configuration file")
    args = parser.parse_args()

    try:
        with open(args.config_file, 'r') as f:
            config = json.load(f)
    except FileNotFoundError:
        print(f"Error: Config file not found ({args.config_file})")
        return
    except json.JSONDecodeError as e:
        print(f"Error: Failed to decode JSON ({e})")
        return

    h = TPD_AppHeader()
    
    h.set_major(config.get('major', "99"))
    h.set_minor(config.get('minor', "99"))
    h.set_build(config.get('build', "99"))
    h.set_name(config.get('name', "TPD Firmware"))
    h.set_hwid(config.get('hwid', "OPENCSMP"))
    h.set_sub_hwid(config.get('sub_hwid', "\x00"))
    h.set_branch(config.get('branch', "None"))
    h.set_commit(config.get('commit', "fffffff"))
    h.set_date(config.get('date', date.today().strftime("%b %d %Y")))
    h.set_kernel_rev(config.get('kernelrev', "None"))
    try:
        h.set_filesize(os.path.getsize(args.input_file))
    except FileNotFoundError:
        print(f"Error: Input file not found ({args.input_file})")
        return

    h.add_tpd_crc()

    new_bin = h.serialize()

    with open(args.input_file, 'rb') as fin:
        data = fin.read(1024)
        while data:
            new_bin += data
            data = fin.read(1024)

    with open(args.output_file, 'wb') as fout:
        fout.write(new_bin)

if __name__ == "__main__":
    main()
