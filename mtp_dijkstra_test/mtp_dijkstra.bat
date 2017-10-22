@echo off
set "WAIT_ONE_SEC=ping 127.0.0.1 -n 2 > nul"
set START=10 && set END=100 && set DENS=%1&& set OUT_DOT=0 && set OUT_W=0
echo NUM_THREADS 500 500 500 700 700 700 900 900 900 > "mtp_dijkstra_%DENS%.csv"

set THRS=1
echo Running %THRS% threads...
echo|set /p=%THRS% >> "mtp_dijkstra_%DENS%.csv"
set N_VERT=500
mpiexec -n %THRS% mtp_dijkstra %N_VERT% %START% %END% %DENS% %OUT_DOT% %OUT_W% >> "mtp_dijkstra_%DENS%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_dijkstra %N_VERT% %START% %END% %DENS% %OUT_DOT% %OUT_W% >> "mtp_dijkstra_%DENS%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_dijkstra %N_VERT% %START% %END% %DENS% %OUT_DOT% %OUT_W% >> "mtp_dijkstra_%DENS%.csv"
%WAIT_ONE_SEC%
set N_VERT=700
mpiexec -n %THRS% mtp_dijkstra %N_VERT% %START% %END% %DENS% %OUT_DOT% %OUT_W% >> "mtp_dijkstra_%DENS%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_dijkstra %N_VERT% %START% %END% %DENS% %OUT_DOT% %OUT_W% >> "mtp_dijkstra_%DENS%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_dijkstra %N_VERT% %START% %END% %DENS% %OUT_DOT% %OUT_W% >> "mtp_dijkstra_%DENS%.csv"
%WAIT_ONE_SEC%
set N_VERT=900
mpiexec -n %THRS% mtp_dijkstra %N_VERT% %START% %END% %DENS% %OUT_DOT% %OUT_W% >> "mtp_dijkstra_%DENS%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_dijkstra %N_VERT% %START% %END% %DENS% %OUT_DOT% %OUT_W% >> "mtp_dijkstra_%DENS%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_dijkstra %N_VERT% %START% %END% %DENS% %OUT_DOT% %OUT_W% >> "mtp_dijkstra_%DENS%.csv"
%WAIT_ONE_SEC%
echo.>> "mtp_dijkstra_%DENS%.csv"

set THRS=2
echo Running %THRS% threads...
echo|set /p=%THRS% >> "mtp_dijkstra_%DENS%.csv"
set N_VERT=500
mpiexec -n %THRS% mtp_dijkstra %N_VERT% %START% %END% %DENS% %OUT_DOT% %OUT_W% >> "mtp_dijkstra_%DENS%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_dijkstra %N_VERT% %START% %END% %DENS% %OUT_DOT% %OUT_W% >> "mtp_dijkstra_%DENS%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_dijkstra %N_VERT% %START% %END% %DENS% %OUT_DOT% %OUT_W% >> "mtp_dijkstra_%DENS%.csv"
%WAIT_ONE_SEC%
set N_VERT=700
mpiexec -n %THRS% mtp_dijkstra %N_VERT% %START% %END% %DENS% %OUT_DOT% %OUT_W% >> "mtp_dijkstra_%DENS%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_dijkstra %N_VERT% %START% %END% %DENS% %OUT_DOT% %OUT_W% >> "mtp_dijkstra_%DENS%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_dijkstra %N_VERT% %START% %END% %DENS% %OUT_DOT% %OUT_W% >> "mtp_dijkstra_%DENS%.csv"
%WAIT_ONE_SEC%
set N_VERT=900
mpiexec -n %THRS% mtp_dijkstra %N_VERT% %START% %END% %DENS% %OUT_DOT% %OUT_W% >> "mtp_dijkstra_%DENS%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_dijkstra %N_VERT% %START% %END% %DENS% %OUT_DOT% %OUT_W% >> "mtp_dijkstra_%DENS%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_dijkstra %N_VERT% %START% %END% %DENS% %OUT_DOT% %OUT_W% >> "mtp_dijkstra_%DENS%.csv"
%WAIT_ONE_SEC%
echo.>> "mtp_dijkstra_%DENS%.csv"

set THRS=4
echo Running %THRS% threads...
echo|set /p=%THRS% >> "mtp_dijkstra_%DENS%.csv"
set N_VERT=500
mpiexec -n %THRS% mtp_dijkstra %N_VERT% %START% %END% %DENS% %OUT_DOT% %OUT_W% >> "mtp_dijkstra_%DENS%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_dijkstra %N_VERT% %START% %END% %DENS% %OUT_DOT% %OUT_W% >> "mtp_dijkstra_%DENS%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_dijkstra %N_VERT% %START% %END% %DENS% %OUT_DOT% %OUT_W% >> "mtp_dijkstra_%DENS%.csv"
%WAIT_ONE_SEC%
set N_VERT=700
mpiexec -n %THRS% mtp_dijkstra %N_VERT% %START% %END% %DENS% %OUT_DOT% %OUT_W% >> "mtp_dijkstra_%DENS%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_dijkstra %N_VERT% %START% %END% %DENS% %OUT_DOT% %OUT_W% >> "mtp_dijkstra_%DENS%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_dijkstra %N_VERT% %START% %END% %DENS% %OUT_DOT% %OUT_W% >> "mtp_dijkstra_%DENS%.csv"
%WAIT_ONE_SEC%
set N_VERT=900
mpiexec -n %THRS% mtp_dijkstra %N_VERT% %START% %END% %DENS% %OUT_DOT% %OUT_W% >> "mtp_dijkstra_%DENS%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_dijkstra %N_VERT% %START% %END% %DENS% %OUT_DOT% %OUT_W% >> "mtp_dijkstra_%DENS%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_dijkstra %N_VERT% %START% %END% %DENS% %OUT_DOT% %OUT_W% >> "mtp_dijkstra_%DENS%.csv"
%WAIT_ONE_SEC%
echo.>> "mtp_dijkstra_%DENS%.csv"
