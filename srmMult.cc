//
// SRM Multiple Command
//

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <cstring>
#include <iostream>
#include <fstream>
#include <cctype>
#include <iomanip>
#include <sys/stat.h>

void Run(std::string pathin_, std::string pathout_){

std::cout << "\nsrmUtility Program" << std::endl;
std::cout << "--------------------\n" << std::endl;


  std::cout << "Please, insert your voms-proxy-init password: " << std::endl;
  system("voms-proxy-init -voms cms:/cms -valid 120:00");
  std::cout << " " << std::endl;

struct stat po;
          
 if (stat(pathout_.c_str(), &po) != 0) {
    std::cout << "\n\nI/O ERROR: Please, check if the path of the output directory is correct." << std::endl;
    std::cout << "There is no output directory: " << pathout_.c_str() << "\n\n" << std::endl;
    return;
 }   

    
std::string srmlscommand;
std::string srmlscommand2;
std::string srmcpcommand;
std::string checkcom;
std::string outcommand;
std::string filename;
std::string pathoutcomplete;
std::vector<std::string> list;

srmlscommand = "srmls -count=999 srm://se-dcache.hepgrid.uerj.br:8443/srm/managerv2?SFN=" + pathin_+ "> tempfile_List.txt";
srmlscommand2 = "srmls -count=999 -offset=999 srm://se-dcache.hepgrid.uerj.br:8443/srm/managerv2?SFN=" + pathin_ + " >> tempfile_List.txt";

system (srmlscommand.c_str());
system (srmlscommand2.c_str());

std::ifstream infile("tempfile_List.txt");
std::string word;

   if(infile.is_open()){

    while (infile >> word) {

       if (!(word.size() < pathin_.size()) ){ 
        list.push_back(word);
       }
    }

    
    for (unsigned n=1; n <= list.size(); ++n) {
        
      double progress = 10.0*n/(1.0*list.size());
   
        if ( !list.at(n).find(pathin_) && (list.at(n).size() > pathin_.size() + 5) && !list.at(n).empty() )  {

          pathoutcomplete = "file://///" + pathout_;
          srmcpcommand = "srmcp -2 -streams_num=1 srm://se-dcache.hepgrid.uerj.br:8443/srm/managerv2?SFN=" + list.at(n) + " " + pathoutcomplete;
          checkcom = "ls " + pathout_+list.at(n).erase(0,pathin_.length()+1);
          filename = pathout_ + list.at(n);
  
          system(srmcpcommand.c_str());

         
        struct stat buffer;
           
        if (stat(filename.c_str(), &buffer) == 0) {
          std::cout <<"\n<>o<>o<>o<>o<>o<> STATUS <>o<>o<>o<>o<>o<>\n" << std::endl;
          std::cout << std::setprecision (4) << 10*progress<<" % completed." << std::endl;
          std::cout << "\n<  OK  >        " << filename.c_str() << ".\n" << std::endl; 
        }
            
        else  {
          std::cout <<"\n<<------------------------------------ REPEAT OPERATION ------------------------------------>>" << std::endl;
          std::cout << " Copying File: " << filename << std::endl;
          std::cout <<  "<<-------------------------------------------------------------------------------------------->>\n" << std::endl;
        }  

      }

    }

   std::cout << "\nTotal of files Listed from SRM SE: " << list.size() << "\n" << std::endl;
   infile.close();

  }

  else{
   std::cout<<"Unable to open list of storage files."<<std::endl;
  }


}


int main(int argc, char **argv)
{

   const char *s1="*";
   std::string pathin_;
   std::string pathout_;
   
   if (argc > 1 && std::strcmp(s1,argv[1]) != 0)  pathin_ = argv[1];
   if (argc > 2 && std::strcmp(s1,argv[2]) != 0)  pathout_  = argv[2];
                                                      
   Run(pathin_, pathout_);
   return 0;

}
