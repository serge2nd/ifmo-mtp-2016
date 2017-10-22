@echo off
set "WAIT_ONE_SEC=ping 127.0.0.1 -n 2 > nul"
set START=10 && set END=1000 && set Q=%1&& set EPS=1e-7 && set ITERS=100
echo NUM_THREADS 300 300 300 300 300 500 500 500 500 500 700 700 700 700 700 900 900 900 900 900 1200 1200 1200 1200 1200 > "mtp_jacobi_%Q%.csv"

set THRS=1
echo Running %THRS% threads...
echo|set /p=%THRS% >> "mtp_jacobi_%Q%.csv"
set SIZE=300
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
set SIZE=500
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
set SIZE=700
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
set SIZE=900
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
set SIZE=1200
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
echo.>> "mtp_jacobi_%Q%.csv"

set THRS=2
echo Running %THRS% threads...
echo|set /p=%THRS% >> "mtp_jacobi_%Q%.csv"
set SIZE=300
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
set SIZE=500
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
set SIZE=700
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
set SIZE=900
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
set SIZE=1200
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
echo.>> "mtp_jacobi_%Q%.csv"

set THRS=4
echo Running %THRS% threads...
echo|set /p=%THRS% >> "mtp_jacobi_%Q%.csv"
set SIZE=300
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
set SIZE=500
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
set SIZE=700
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
set SIZE=900
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
set SIZE=1200
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
mpiexec -n %THRS% mtp_jacobi %SIZE% %START% %END% %Q% %EPS% %ITERS% >> "mtp_jacobi_%Q%.csv"
%WAIT_ONE_SEC%
echo.>> "mtp_jacobi_%Q%.csv"
