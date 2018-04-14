all:
	gcc -I/opt/vc/include -I/opt/vc/include/interface/vmcs_host/linux -I/opt/vc/include/interface/vcos/pthreads graphics.c -o graphics -lshapes
	gcc serial.c rs232.c -Wall -Wextra -o2 -o serial
	gcc -Wall -o client client.c
	gcc -Wall -o server server.c -lm -pthread