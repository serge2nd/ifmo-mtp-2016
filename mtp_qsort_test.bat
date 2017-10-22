@echo NUM_THREADS 5000 10000 15000 20000 30000 40000 50000 > mtp_qsort.csv
set "MTP_QSORT=Release\mtp_qsort.exe"

@echo|set /p=1 >> mtp_qsort.csv
set THRS=1
mpiexec -n %THRS% %MTP_QSORT% test 5000 0 >> mtp_qsort.csv
mpiexec -n %THRS% %MTP_QSORT% test 10000 0 >> mtp_qsort.csv
mpiexec -n %THRS% %MTP_QSORT% test 15000 0 >> mtp_qsort.csv
mpiexec -n %THRS% %MTP_QSORT% test 20000 0 >> mtp_qsort.csv
mpiexec -n %THRS% %MTP_QSORT% test 30000 0 >> mtp_qsort.csv
mpiexec -n %THRS% %MTP_QSORT% test 40000 0 >> mtp_qsort.csv
mpiexec -n %THRS% %MTP_QSORT% test 50000 0 >> mtp_qsort.csv

@echo.>> mtp_qsort.csv
@echo|set /p=2 >> mtp_qsort.csv
set THRS=2
mpiexec -n %THRS% %MTP_QSORT% test 5000 0 >> mtp_qsort.csv
mpiexec -n %THRS% %MTP_QSORT% test 10000 0 >> mtp_qsort.csv
mpiexec -n %THRS% %MTP_QSORT% test 15000 0 >> mtp_qsort.csv
mpiexec -n %THRS% %MTP_QSORT% test 20000 0 >> mtp_qsort.csv
mpiexec -n %THRS% %MTP_QSORT% test 30000 0 >> mtp_qsort.csv
mpiexec -n %THRS% %MTP_QSORT% test 40000 0 >> mtp_qsort.csv
mpiexec -n %THRS% %MTP_QSORT% test 50000 0 >> mtp_qsort.csv

@echo.>> mtp_qsort.csv
@echo|set /p=4 >> mtp_qsort.csv
set THRS=4
mpiexec -n %THRS% %MTP_QSORT% test 5000 0 >> mtp_qsort.csv
mpiexec -n %THRS% %MTP_QSORT% test 10000 0 >> mtp_qsort.csv
mpiexec -n %THRS% %MTP_QSORT% test 15000 0 >> mtp_qsort.csv
mpiexec -n %THRS% %MTP_QSORT% test 20000 0 >> mtp_qsort.csv
mpiexec -n %THRS% %MTP_QSORT% test 30000 0 >> mtp_qsort.csv
mpiexec -n %THRS% %MTP_QSORT% test 40000 0 >> mtp_qsort.csv
mpiexec -n %THRS% %MTP_QSORT% test 50000 0 >> mtp_qsort.csv
