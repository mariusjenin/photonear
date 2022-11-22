#!/bin/sh
bindir=$(pwd)
cd /home/mariusjenin/Documents/Master/GIT/photonear/main/
export 

if test "x$1" = "x--debugger"; then
	shift
	if test "xYES" = "xYES"; then
		echo "r  " > $bindir/gdbscript
		echo "bt" >> $bindir/gdbscript
		/usr/bin/gdb -batch -command=$bindir/gdbscript --return-child-result /home/mariusjenin/Documents/Master/GIT/photonear/cmake-build-debug/main 
	else
		"/home/mariusjenin/Documents/Master/GIT/photonear/cmake-build-debug/main"  
	fi
else
	"/home/mariusjenin/Documents/Master/GIT/photonear/cmake-build-debug/main"  
fi
