/* 
Integrantes: Santiago Collantes.
             Kevin Ocampo.

Tarea: Obtener cierta informacion sobre la maquina que ejecuta el codigo.

Herramientas: Linux Ubuntu 20.04, Apache2, VirtualBox, Vagrant, C++, HTML.

Compilacion (en Linux): sudo g++ -o /usr/lib/cgi-bin/script script.cpp
Link de la pagina: http://192.168.56.120/cgi-bin/script (La ip corresponde a la IP dada por el VagrantFile)

Configuracion Apache para el despliegue:
    - sudo apt-get install apache2
    - sudo a2enmod cgi
    - systemctl restart apache2
    - sudo nano /etc/apache2/apache2.conf
    - <Directory "/usr/lib/cgi-bin"> 
        AllowOverride None
        Options +ExecCGI -MultiViews +SymLinksIfOwnerMatch
        Require all granted
      </Directory>
    - sudo service apache2 restart
*/

#include <iostream>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace std;

string getNum(string line){
    string res = "";
    int idx = line.find("intr");
    for(int i = idx+5;i < line.size();i++){
        if(line[i] == ' '){
            return res;               
        }
        res += line[i];
    }
    return res;
}

string GetStdoutFromCommand(string cmd) {

  string data;
  FILE * stream;
  const int max_buffer = 256;
  char buffer[max_buffer];
  cmd.append(" 2>&1");

  stream = popen(cmd.c_str(), "r");
  if (stream) {
    while (!feof(stream))
      if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
    pclose(stream);
  }
  return data;
}

int GetRamInKB(void){
    FILE *meminfo = fopen("/proc/meminfo", "r");
    if(meminfo == NULL)
        cout << "Archivo vacio" << endl;

    char line[256];
    while(fgets(line, sizeof(line), meminfo)){
        int ram;
        if(sscanf(line, "MemTotal: %d kB", &ram) == 1){
            fclose(meminfo);
            return ram;
        }
    }

    fclose(meminfo);
    return -1;
}

int main(){
    //Code to get number of cores in the machine
    int cores = thread::hardware_concurrency();

    //Code to get number of current process in the machine
    string process = GetStdoutFromCommand("ps aux | wc -l");

    //Code to get number of memory in the machine in MB
    int ram = (GetRamInKB()/1024);

    //Code to get total interruptions in the machine
    string intr = getNum(GetStdoutFromCommand("cat /proc/stat"));
    
    //Code HTML
    cout << "content-type: text/html" << endl << endl;
    cout << "<html>" << endl;
    
    cout << "<head>" << endl;
    cout << "<title>" << "OPERATING SYSTEM" << "</title>" << endl;
    cout << "</head>" << endl;
    
    cout << "<body style=\"background-color: #14F4BF;\">" << endl;

    cout << "<h1 style=\"margin-top: 5%;font-size:50px;margin-left:15%;color: #14F2F4;text-shadow:0 0 5px rgb(80, 3, 224),0 0 10px rgb(87, 0, 250),0 0 20px rgb(77, 4, 248),0 0 40px rgb(80, 40, 145),0 0 80px rgb(84, 38, 127),0 0 90px rgb(72, 38, 127),0 0 100px rgb(99, 38, 127),0 0 140px rgb(106, 38, 127),0 0 180px rgb(99, 38, 127);\">" << "NUMBER OF CORES: " << cores << "</h1>" << endl;

    cout << "<h1 style=\"margin-top: 5%;font-size:50px;margin-left:15%;color: #14F2F4;text-shadow:0 0 5px rgb(80, 3, 224),0 0 10px rgb(87, 0, 250),0 0 20px rgb(77, 4, 248),0 0 40px rgb(80, 40, 145),0 0 80px rgb(84, 38, 127),0 0 90px rgb(72, 38, 127),0 0 100px rgb(99, 38, 127),0 0 140px rgb(106, 38, 127),0 0 180px rgb(99, 38, 127);\">" << "NUMBER OF PROCESSES: " << process << "</h1>" << endl;
    
    cout << "<h1 style=\"margin-top: 5%;font-size:50px;margin-left:15%;color: #14F2F4;text-shadow:0 0 5px rgb(80, 3, 224),0 0 10px rgb(87, 0, 250),0 0 20px rgb(77, 4, 248),0 0 40px rgb(80, 40, 145),0 0 80px rgb(84, 38, 127),0 0 90px rgb(72, 38, 127),0 0 100px rgb(99, 38, 127),0 0 140px rgb(106, 38, 127),0 0 180px rgb(99, 38, 127);\">" << "TOTAL RAM: " << ram << "</h1>" << endl;
    
    cout << "<h1 style=\"margin-top: 5%;font-size:50px;margin-left:15%;color: #14F2F4;text-shadow:0 0 5px rgb(80, 3, 224),0 0 10px rgb(87, 0, 250),0 0 20px rgb(77, 4, 248),0 0 40px rgb(80, 40, 145),0 0 80px rgb(84, 38, 127),0 0 90px rgb(72, 38, 127),0 0 100px rgb(99, 38, 127),0 0 140px rgb(106, 38, 127),0 0 180px rgb(99, 38, 127);\">" << "TOTAL OF INTERUPTIONS: " << intr <<  "</h1>" << endl;
        
    cout << "</body>" << endl;
    cout << "</html>" << endl;

    return 0;
}