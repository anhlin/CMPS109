// $Id: cix.cpp,v 1.6 2018-07-26 14:18:32-07 - - $

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <unordered_map>
#include <iterator>
using namespace std;

#include <libgen.h>
#include <sys/types.h>
#include <unistd.h>

#include "protocol.h"
#include "logstream.h"
#include "sockets.h"

logstream log (cout);

struct cix_exit: public exception {};

unordered_map<string,cix_command> command_map {
   {"exit", cix_command::EXIT},
   {"help", cix_command::HELP},
   {"ls"  , cix_command::LS  },
   {"get" , cix_command::GET },
   {"put" , cix_command::PUT },
   {"rm"  , cix_command::RM  },
};

static const string help = R"||(
exit         - Exit the program.  Equivalent to EOF.
get filename - Copy remote file to local host.
help         - Print help summary.
ls           - List names of files on remote server.
put filename - Copy local file to remote host.
rm filename  - Remove file from remote server.
)||";

void cix_help() {
   cout << help;
}

void cix_ls (client_socket& server) {
   cix_header header;
   header.command = cix_command::LS;
   log << "sending header " << header << endl;
   send_packet (server, &header, sizeof header);
   recv_packet (server, &header, sizeof header);
   log << "received header " << header << endl;
   if (header.command != cix_command::LSOUT) {
      log << "sent LS, server did not return LSOUT" << endl;
      log << "server returned " << header << endl;
   }else {
      char buffer[header.nbytes + 1];
      recv_packet (server, buffer, header.nbytes);
      log << "received " << header.nbytes << " bytes" << endl;
      buffer[header.nbytes] = '\0';
      cout << buffer;
   }
}

void cix_get (client_socket& server, string filename) {
  cix_header header; 
  memset(&header, 0, sizeof header); 
  header.command = cix_command::GET;
  snprintf(header.filename, sizeof (header.filename), 
                                     "%s", filename.c_str()); 
  log << "sending header " << header << endl; 
  send_packet (server, &header, sizeof header); 
  recv_packet (server, &header, sizeof header); 
  log << "received hedaer " << header << endl;  
  if(header.command != cix_command::FILE){
    log << "sent GET, server did not return FILE" << endl; 
    log << "server returned " << header << endl;
  }else {
    char buffer[header.nbytes];
    recv_packet(server, buffer, header.nbytes); 
    log << "received " << header.nbytes << " bytes" << endl; 
    ofstream ofile (filename, ofstream::out); 
    ofile.write(buffer, sizeof buffer); 
    ofile.close();
  }
}

void cix_rm (client_socket& server, string filename) {
  cix_header header; 
  header.command = cix_command::RM; 
  header.nbytes = 0; 
  snprintf(header.filename, sizeof(header.filename),
                                    "%s", filename.c_str());  
  log << "sending header " << header << endl; 
  send_packet (server, &header, sizeof header); 
  recv_packet (server, &header, sizeof header); 
  log << "received header " << header << endl; 
  if (header.command != cix_command::ACK) {
    log << "sent RM, server did not return ACK" << endl;
    log << "server returned " << header << endl; 
  }else {
    log << filename << " removed" << endl; 
  }
}

void cix_put (client_socket& server, string filename) {
  cix_header header; 
  header.command = cix_command::PUT;
  snprintf(header.filename, sizeof (header.filename), 
                                     "%s", filename.c_str());
  ifstream load (header.filename, ifstream::binary);
  if (!load){
    log << "put: " << filename << ": " << strerror (errno) << endl;
    return;
  }
  load.seekg (0, load.end); 
  int bytes = load.tellg();
  load.seekg (0, load.beg); 
  char*  buffer = new char [bytes]; 
  load.read (buffer, bytes); 
  load.close(); 

  header.nbytes = bytes; 
  log << "sending header " << header << endl; 
  send_packet (server, &header, sizeof header); 
  log << "sending payload " << endl;
  send_packet (server, buffer, bytes); 
  recv_packet (server, &header, sizeof header); 
  log << "received header " << header << endl;  

  if (header.command != cix_command::ACK) {
    log << "sent PUT, server did not return ACK" << endl; 
    log << "server returned " << header << endl; 
  }else {
    log << filename << " copied" << endl;
  }
}

void usage() {
   cerr << "Usage: " << log.execname() << " [host] [port]" << endl;
   throw cix_exit();
}

int main (int argc, char** argv) {
   log.execname (basename (argv[0]));
   log << "starting" << endl;
   vector<string> args (&argv[1], &argv[argc]);
   if (args.size() > 2) usage();
   string host = get_cix_server_host (args, 0);
   in_port_t port = get_cix_server_port (args, 1);
   log << to_string (hostinfo()) << endl;
   try {
      log << "connecting to " << host << " port " << port << endl;
      client_socket server (host, port);
      log << "connected to " << to_string (server) << endl;
      for (;;) {
         string line;
         getline (cin, line);
         string filename; 
         istringstream iss(line); 
         vector<string> tokens((istream_iterator<string>(iss)),
           istream_iterator<string>());
         string command = tokens.at(0); 
         if(tokens.size() > 1) filename = tokens.at(1); 
         cout << filename << endl;
         if (cin.eof()) throw cix_exit();
         log << "command " << line << endl;
         const auto& itor = command_map.find (command);
         cix_command cmd = itor == command_map.end()
                         ? cix_command::ERROR : itor->second;
         switch (cmd) {
            case cix_command::EXIT:
               throw cix_exit();
               break;
            case cix_command::HELP:
               cix_help();
               break;
            case cix_command::LS:
               cix_ls (server);
               break;
            case cix_command::RM:
               cix_rm (server, filename);
               break;
            case cix_command::PUT:
               cix_put (server, filename);
               break;
            case cix_command::GET:
               cix_get (server, filename); 
               break;
            default:
               log << line << ": invalid command" << endl;
               break;
         }
      }
   }catch (socket_error& error) {
      log << error.what() << endl;
   }catch (cix_exit& error) {
      log << "caught cix_exit" << endl;
   }
   log << "finishing" << endl;
   return 0;
}

