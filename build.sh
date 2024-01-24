#!/bin/sh

###set Global PATHs
TOPDIR=`pwd`

###set PATH only used in this script
DESTDIR=$TOPDIR
SAMPLEDIR=$TOPDIR/sample
TLVPROTODIR=$TOPDIR/src/csmpagent/tlvs

export OS=$1

build_header()
{
  echo "########## generating protobuf header files...##########"
  make $OS -C $TLVPROTODIR
}

build_sample()
{
  make $OS -C $SAMPLEDIR
}

build_lib()
{
  make $OS -C $DESTDIR
  rm *.o
  mv csmp_agent_lib.a sample/
}

clean_all()
{
  make clean -C $DESTDIR
  make clean -C $SAMPLEDIR
#  make clean -C $TLVPROTODIR
}

if [ "$1"x = "clean"x ];then
  echo "########## start cleaning...##########"
  clean_all;
else
  echo "########## start building...##########"
  clean_all;
#  build_header;
  build_lib;
  build_sample;
fi
