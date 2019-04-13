@echo off
start bin/debug/latencysample.exe --server --tcp --network-update-time 0.5 
start bin/debug/latencysample.exe --client --tcp --network-update-time 0.5 --x 800
