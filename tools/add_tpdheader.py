#!C:\Python27\python.exe
#!/usr/bin/python

# add_tpdheader.py
# Helper script to add a Cisco FND compatible header
# to TPD firmware image files for firmware upgrade
#
# usage: add_tpdheader.py [-h] [-i MAJOR] [-j MINOR] [-k BUILD] [-n NAME]
#                         [-m HWID] [-s SUB_HWID] [-b BRANCH] [-c COMMIT]
#                         [-d DATE] [-r KERNELREV]
#                         bin_file

import os, sys
import struct
import argparse
import time
from datetime import date

class TPD_CRC(object):
    @staticmethod
    def __crc32_msb_byte(byte, rpolynom, crc):
        crc = crc ^ ((byte << 24) & 0xFFFFFFFF)
        for i in range(8):
            if crc & 0x80000000 != 0:
                crc = ((crc << 1) & 0xFFFFFFFF) ^ rpolynom
            else:
                crc = ((crc << 1) & 0xFFFFFFFF)
        return crc

    @staticmethod
    def tpd_crc32(buf, rpolynom=0x04C11DB7, crc=0L):
        for i in range(len(buf)):
            crc = TPD_CRC.__crc32_msb_byte(struct.unpack("B", buf[i])[0], rpolynom, crc)
        return crc

class TPD_AppHeader(object):
    def __init__(self):
        self.hdr = {'hdr_version' : 2,
                    'hdr_len' : 256,
                    'git_flag' : 1
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
        print " ".join.encode(c.encode('hex').upper() for c in self.serialize())

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
        self.hdr['name'] = bytes[i:i+32].split("\x00")[0]
        i += 32
        self.hdr['branch'] = bytes[i:i+32].split("\x00")[0]
        i += 32
        self.hdr['commit'] = bytes[i:i+8].split("\x00")[0]
        i += 8
        self.hdr['git_flag'] = struct.unpack("<I", bytes[i:i+4])[0]
        i += 4
        self.hdr['date'] = bytes[i:i+16].split("\x00")[0]
        i += 16
        self.hdr['hwid'] = bytes[i:i+32].split("\x00")[0]
        i += 32
        self.hdr['sub_hwid'] = bytes[i:i+32].split("\x00")[0]
        i += 32
        self.hdr['kernel_rev'] = bytes[i:i+16].split("\x00")[0]
        i += 16

    def serialize(self):
        app_hdr = ""

        app_hdr += struct.pack("<I", self.hdr['hdr_version'])
        app_hdr += struct.pack("<I", self.hdr['hdr_len'])
        app_hdr += struct.pack("<I", self.hdr['major'])
        app_hdr += struct.pack("<I", self.hdr['minor'])
        app_hdr += struct.pack("<I", self.hdr['build'])
        app_hdr += struct.pack("<I", self.filesize + self.hdr['hdr_len'] + 4)
        app_hdr += self.hdr['name'][0:32] + ("\x00" * (32 - len(self.hdr['name'])))
        app_hdr += self.hdr['branch'][0:32] + ("\x00" * (32 - len(self.hdr['branch'])))
        app_hdr += self.hdr['commit'][0:8] + ("\x00" * (8 - len(self.hdr['commit'])))
        app_hdr += struct.pack("<I", self.hdr['git_flag'])
        app_hdr += self.hdr['date'][0:16] + ("\x00" * (16 - len(self.hdr['date'])))
        app_hdr += self.hdr['hwid'][0:32] + ("\x00" * (32 - len(self.hdr['hwid'])))
        app_hdr += self.hdr['sub_hwid'][0:32] + ("\x00" * (32 - len(self.hdr['sub_hwid'])))
        app_hdr += self.hdr['kernel_rev'][0:16] + ("\x00" * (16 - len(self.hdr['kernel_rev'])))

        app_hdr += "\x00" * (252 - len(app_hdr)) # 252 because last 4 bytes for Header CRC

        if self.crc:
            return app_hdr + struct.pack("<I", TPD_CRC.tpd_crc32(app_hdr))
        return app_hdr


def main():
    now = date.today()
    parser = argparse.ArgumentParser(description='Add FND TPD header to TPD firmware binary file')
    parser.add_argument('bin_file', help="TPD Firmware binary file")
    parser.add_argument('-i', '--major', default="99", help='Major Firmware Version')
    parser.add_argument('-j', '--minor', default="99", help='Minor Firmware Version')
    parser.add_argument('-k', '--build', default="99", help='Build Firmware Version')
    parser.add_argument('-n', '--name', default="TPD Firmware", help='Firmware Name')
    parser.add_argument('-m', '--hwid', default="OPENCSMP", help='Firmware Hardware ID')
    parser.add_argument('-s', '--sub_hwid', default="\x00", help='Sub Hardware ID')
    parser.add_argument('-b', '--branch', default="None", help='Git Branch')
    parser.add_argument('-c', '--commit', default="fffffff", help='Git Commit')
    parser.add_argument('-d', '--date', default=now.strftime("%b %d %Y"), help='Build Date')
    parser.add_argument('-r', '--kernelrev', default="None", help='Kernel Version')
    args = parser.parse_args()

    h = TPD_AppHeader()
    
    h.set_major(args.major)
    h.set_minor(args.minor)
    h.set_build(args.build)
    h.set_name(args.name)
    h.set_hwid(args.hwid)
    h.set_sub_hwid(args.sub_hwid)
    h.set_branch(args.branch)
    h.set_commit(args.commit)
    h.set_date(args.date)
    h.set_kernel_rev(args.kernelrev)
    h.set_filesize(os.path.getsize(args.bin_file))

    h.add_tpd_crc()

    new_bin = h.serialize()

    with open(args.bin_file, 'rb') as fin:
        data = fin.read(1024)
        while data != "":
            new_bin += data
            data = fin.read(1024)

    with open(args.bin_file, 'wb') as fout:
        fout.write(new_bin)


if __name__ == "__main__":
    main()
