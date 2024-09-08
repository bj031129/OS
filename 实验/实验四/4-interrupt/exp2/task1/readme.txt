# ls
Makefile  workqueue_test.c
# make
# insmod workqueue_test.ko times=5
# lsmod | grep workqueue_test
workqueue_test        262144  0
# rmmod workqueue_test
# dmesg | tail -n13
[ 4321.223910] Initializing Workqueue Module
[ 4321.224369] 0:
[ 4321.224483] Immediate Work: Current time: 2024-05-24 09:13:41
[ 4326.336540] 1:
[ 4326.336676] Immediate Work: Current time: 2024-05-24 09:13:46
[ 4331.456516] 2:
[ 4331.456649] Immediate Work: Current time: 2024-05-24 09:13:51
[ 4331.457005] Delayed Work: Current time: 2024-05-24 09:13:51
[ 4336.576501] 3:
[ 4336.576714] Immediate Work: Current time: 2024-05-24 09:13:56
[ 4341.696471] 4:
[ 4341.696610] Immediate Work: Current time: 2024-05-24 09:14:02
[ 4364.855374] Exiting Workqueue Module
# make clean