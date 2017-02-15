#!/bin/bash
time ./unbuffered_io_performance fn read 128
echo "\n"
time ./unbuffered_io_performance fn read 256
echo "\n"
time ./unbuffered_io_performance fn read 512
echo "\n"
time ./unbuffered_io_performance fn read 1024
echo "\n"
time ./unbuffered_io_performance fn read 2048
echo "\n"
time ./unbuffered_io_performance fn read 4096
echo "\n"
time ./unbuffered_io_performance fn read 8192
echo "\n"
time ./unbuffered_io_performance fn read 16384
echo "\n"
time ./unbuffered_io_performance fn read 32768
echo "\n"
time ./unbuffered_io_performance fn read 65536
echo "\n"
time ./unbuffered_io_performance fn read 131072
echo "\n"

time ./unbuffered_io_performance fn write 128
echo "\n"
time ./unbuffered_io_performance fn write 256
echo "\n"
time ./unbuffered_io_performance fn write 512
echo "\n"
time ./unbuffered_io_performance fn write 1024
echo "\n"
time ./unbuffered_io_performance fn write 2048
echo "\n"
time ./unbuffered_io_performance fn write 4096
echo "\n"
time ./unbuffered_io_performance fn write 8192
echo "\n"
time ./unbuffered_io_performance fn write 16384
echo "\n"
time ./unbuffered_io_performance fn write 32768
echo "\n"
time ./unbuffered_io_performance fn write 65536
echo "\n"
time ./unbuffered_io_performance fn write 131072
echo "\n"
