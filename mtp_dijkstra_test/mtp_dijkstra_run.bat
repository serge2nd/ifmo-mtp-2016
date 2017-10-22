@echo off
mpiexec -n %2 mtp_dijkstra.exe %3 %4 %5 %6 %7 %8
if %1==y (
	if ERRORLEVEL 0 (
		start gvedit %5
	)
)