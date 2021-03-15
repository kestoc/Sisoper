#include <unistd.h>
#include <sys/wait.h>
#include <bits/stdc++.h>

using namespace std;

vector<string> parse(string s){
  vector<string> v;
  istringstream ss(s);
  string aux;

  while( ss >> aux ){
    v.push_back(aux);
  }

  return v;
}

string GetStdoutFromCommand2(string command) {
  char buffer[1024];
  string result = "";
  // Open pipe to file
  FILE* pipe = popen(command.c_str(), "r");
  if (!pipe) {
    return result;
  }
  // read till end of process:
  while (!feof(pipe)) {
    // use buffer to read and add to result
    if (fgets(buffer, 1024, pipe) != NULL){
      result = buffer;
      result.erase(remove(result.begin(), result.end(), '\n'), result.end());
      return result;
    }
  }
  pclose(pipe);
  return result;
}

vector<string> GetStdoutFromCommand(string command) {
   char buffer[1024];
   vector<string> ans;
   string result = "";
   // Open pipe to file
   FILE* pipe = popen(command.c_str(), "r");
   if (!pipe) {
      return ans;
   }
   // read till end of process:
   while (!feof(pipe)) {
      // use buffer to read and add to result
      if (fgets(buffer, 1024, pipe) != NULL){
        result = buffer;
        result.erase(remove(result.begin(), result.end(), '\n'), result.end());
        ans.push_back(result);
      }
   }
   pclose(pipe);
   return ans;
}

int main(){
    string dir1, dir2;
    vector<string> files;
    int fd[2]; //fd[0] -> lectura, fd[1] -> escritura

    if(pipe(fd) == -1){
        printf("Error al ejecutar pipe\n");
    }
    else{
        cout << "Directorio 1:\t"; 
        cin >> dir1;
        cout << "Directorio 2:\t" ;
        cin >> dir2;
        
        int id = fork();
        
        if(id < 0){
            printf("Error al crear fork\n");
        }
        else if(id == 0){
            //Hijo, lee los archivos en dir1 con extencion .c y .h ; ls -la *.c para listar los archivos
            close(fd[0]);
            //Se listan por comando linux los archivos con esa extencion y se almacenan en un vector
            files = GetStdoutFromCommand("cd " + dir1 + " && ls -la *.c *.h");
            for(int i = 0; i < files.size(); i++){
                //Se convierte el string en un char* y se escribe en el fileDescriptor
                write(fd[1],files[i].c_str(),files[i].length()+1);   
            }
            close(fd[1]);
            wait(NULL); //Se mata el proceso hijo una vez finalizada la tarea y continua con el padre
        }
        else{
            //Padre, copia los archivos al dir2
            close(fd[1]);
            char c;
            string s = "";

            while( read(fd[0], &c, 1) > 0 ){ //Lee hasta EOF
                //Si aun no es el final de la cadena, almacena en un string
                if(c != 0){
                    s += c;
                }
                else{
                    if(s != ""){
                        //Descompongo en un vector la informacion del archivo
                        vector<string> line = parse(s);
                        string fileAct = line[line.size()-1];
                        
                        //Busco si el archivo esta en dir2
                        string finded = GetStdoutFromCommand2("cd " + dir2 + " && ls -la " + fileAct);
                        
                        //Si no esta el archivo en dir2 lo copio desde dir1 a dir2
                        if(finded == ""){
                            string copy = "cp " + dir1 + "/" + fileAct + " " + dir2;
                            system((char*)copy.c_str());
                        }
                        else{ //Si esta, busco dejar en dir2 el que tenga fecha de modificacion mas actual
                            string condition = GetStdoutFromCommand2("[ " + dir1 + "/" + fileAct + " -nt " + dir2 + "/" + fileAct + " ] && echo \"yes\"");
                            if(condition == "yes"){
                              string newCopy = "rm " + dir2 + "/" + fileAct + " && cp " + dir1 + "/" + fileAct + " " + dir2;
                              system((char*)newCopy.c_str());                            
                            }
                        }
                    }
                    s = "";
                }
            }
            close(fd[0]);
        }
    }
    return 0;
}