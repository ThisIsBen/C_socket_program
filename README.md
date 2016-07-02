# C_socket_program
The followings are some commands that you can use to achieve some cool purposes:
C :Create a new file on server.
R :Remove an existing file on server.
L :List out all the files on server.
D :Download a file from server.
E :Edit an existing file on server with \"vi\" editor.
Copy :Copy an existing file on server.
V :View and edit an existing file on client side with \"vi\" editor.


How to run this program:
Use 127.0.0.1 local host with port :8888
Server end:
 cc -pthread server.c
 ./a.out


Client end:
 cc client.c
 ./a.out
