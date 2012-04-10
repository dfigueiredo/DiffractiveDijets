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

void Run(std::string pathin_){

std::cout << "\nsrmUtility Cleanner Program" << std::endl;
std::cout << "---------------------------------------\n" << std::endl;


  std::cout << "Please, insert your voms-proxy-init password: " << std::endl;
  system("voms-proxy-init -voms cms:/cms -valid 120:00");
  std::cout << " " << std::endl;

struct stat po;
          
  
std::string srmlscommand;
std::string srmlscommand2;
std::string srmdelcommand;
std::string finalcommand;
std::string checkcom;
std::string outcommand;
std::string filename;
std::vector<std::string> list;

srmlscommand = "srmls -count=999 srm://se-dcache.hepgrid.uerj.br:8443/srm/managerv2?SFN=" + pathin_+ "> tempfile_Listdel.txt";
srmlscommand2 = "srmls -count=999 -offset=999 srm://se-dcache.hepgrid.uerj.br:8443/srm/managerv2?SFN=" + pathin_ + " >> tempfile_Listdel.txt";
finalcommand = "srmrmdir -recursive=true srm://se-dcache.hepgrid.uerj.br:8443/srm/managerv2?SFN="+ pathin_;


system (srmlscommand.c_str());
system (srmlscommand2.c_str());

std::ifstream infile("tempfile_Listdel.txt");
std::string word;

   if(infile.is_open()){

    while (infile >> word) {

       if (!(word.size() < pathin_.size()) ){ 
        list.push_back(word);
       }
    }

    
    for (unsigned n=1; n < list.size(); ++n) {
        
      double progress = 10.0*n/(1.0*list.size());
   
        if ( !list.at(n).find(pathin_) && (list.at(n).size() > pathin_.size() + 5) && !list.at(n).empty() )  {

          
           srmdelcommand = "srmrm srm://se-dcache.hepgrid.uerj.br:8443/srm/managerv2?SFN=" + list.at(n);
           std::cout<< srmdelcommand << std::endl;
           system(srmdelcommand.c_str());
           std::cout << "File "<< list.at(n) << " was erased." << std::endl;
   
        struct stat buffer;
                           
      }

    }

    std::cout << "\nMain Folder will be removed.\n" << std::endl;
    system(finalcommand.c_str());
    std::cout << finalcommand << std::endl;
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
    
   if (argc > 1 && std::strcmp(s1,argv[1]) != 0)  pathin_ = argv[1];
                                                      
   Run(pathin_);
   return 0;

}
