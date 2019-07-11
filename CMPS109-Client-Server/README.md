# CMPS109-Client-Server
This project will implement a client/server application using sockets. A daemon
(cixd) listens on a socket for client connection requests. Each connection will cause
the server to fork a child process to serve the client. The daemon runs in an infinite
loop listening. The server exits when the client disconnects. A client (cix) connects
to a server and can send files, receive files, and get a summary listing of all files
present.
