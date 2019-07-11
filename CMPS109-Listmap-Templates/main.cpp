// $Id: main.cpp,v 1.11 2018-01-25 14:19:29-08 - - $
//Anthony Lin (anhlin)
//Mihir malik (mimalik)

#include <cstdlib>
#include <exception>
#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <regex>
using namespace std;

#include "listmap.h"
#include "xpair.h"
#include "util.h"

using str_str_map = listmap<string,string>;
using str_str_pair = str_str_map::value_type;

void scan_options (int argc, char** argv) {
   opterr = 0;
   for (;;) {
      int option = getopt (argc, argv, "@:");
      if (option == EOF) break;
      switch (option) {
         case '@':
            debugflags::setflags (optarg);
            break;
         default:
            complain() << "-" << char (optopt) << ": invalid option"
                       << endl;
            break;
      }
   }
}

void instructionCall(istream& in, string fname, str_str_map& map){
  str_str_pair pair;
  regex comment_regex {R"(^\s*(#.*)?$)"};
  regex key_value_regex {R"(^\s*(.*?)\s*=\s*(.*?)\s*$)"};
  regex key_delete_regex {R"(^\s*(.*?)\s*=\s*(\s*$)$)"};
  regex trimmed_regex {R"(^\s*([^=]+?)\s*$)"};
  regex equals_regex {R"(^\s*([=])$)"};
  regex print_value_regex {R"(^=\s*(.*?)\s*$)"};

  string cmnd;
  int linenum = 0;
  for(;;){
    getline(in,cmnd);
    ++linenum;
    if (in.eof()) break;
    smatch result;
    if (regex_search (cmnd, result, comment_regex)){
      cout << fname << ": " << linenum << ": " << cmnd << endl;
      continue;
    }
    if(regex_search (cmnd, result, equals_regex)){
      cout << fname << ": " << linenum << ": " << cmnd << endl;
      map.print_map();
    }
    else if(regex_search (cmnd, result, key_delete_regex)){
      cout << fname << ": " << linenum << ": " << cmnd << endl;
      map.delete_key(result[1]);
    }
    else if (regex_search (cmnd, result, print_value_regex)){
      cout << fname << ": " << linenum << ": " << cmnd << endl;
      map.print_value(result[1]);
    }
    else if (regex_search (cmnd, result, key_value_regex)){
      str_str_pair pair(result[1], result[2]);
      map.insert(pair);
      cout << fname << ": " << linenum << ": " << cmnd << endl;
      cout << result[1]<< " = " << result[2] << endl;
    }
    else if (regex_search (cmnd, result, trimmed_regex)){
      cout << fname << ": " << linenum << ": " << cmnd << endl;
      map.print_key(result[1]);
    }
    else if (regex_search (cmnd, result, equals_regex)){
      cout << fname << ": " << linenum << ": " << cmnd << endl;
    }
  }
}

int main (int argc, char** argv) {
   string blank_cin = "-";
   string fname;
   str_str_map map;
   sys_info::execname(argv[0]); 
   if(argc>1){
     for(int i=1; i<argc; ++i){
       string fname = argv[i];
       if(fname == blank_cin) instructionCall(cin, blank_cin, map);

       else{
         ifstream fin(fname);
         if(fin.fail()){
           cerr<<sys_info::execname()<<": "<<fname
                   <<": No such file or direcotry"<<endl;
           sys_info::exit_status(1);
         }
         else{
           instructionCall(fin,fname, map);
           fin.close();
         }
       }
     }
   }

   else instructionCall(cin,blank_cin, map);

   return sys_info::exit_status();
}
