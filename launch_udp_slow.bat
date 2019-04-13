@echo off
start bin/debug/latencysample.exe --server --udp --network-update-time 0.5 
start bin/debug/latencysample.exe --client --udp --network-update-time 0.5 --x 800
