#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

int leer(char *N_archivo){
   clock_t comienzo, fin;
   double tiempo;

   comienzo = clock();
   char tipo[15];
   int tamano = 3;
   int i = 0;
   bool No_encontrada = true;

   char Palabra[strlen(N_archivo)];
   strcpy(Palabra, N_archivo);
   for (int p = 0; p < strlen(Palabra); p++)
      Palabra[p] = toupper(Palabra[p]);
   
   Palabra[strlen(Palabra) - 4] = '\0';
   char ruta[50] = "Archivos/";
   strcat(ruta, N_archivo);

   FILE *archivo;
   archivo = fopen(ruta, "r");

   if(archivo ==NULL){
      printf("ERROR AL ABRIR EL ARCHIVO\n");
      return 1;
   }

   fgets(tipo, 14, archivo);
   
   while(fgetc(archivo) != '\n'){
      tamano++;
   }

   rewind(archivo);
   fgets(tipo, 14, archivo);
   

   if(strstr(tipo, "horizontal")){
      char AUX[80] = "";
      while(i < strlen(Palabra)){
         AUX[(2*i) - 1] = ' ';
         AUX[i*2] = Palabra[i];
         i++;
      }

      char columna[tamano];
      while (fgets(columna, tamano, archivo) && No_encontrada){
         if(strstr (columna, AUX)){
            printf("%s - ", Palabra);
            No_encontrada = false;  
         }
      }
   }

   else{
      int a;
      int largo = strlen(Palabra) - 1;
      int c;
      char sopa[tamano][tamano];
      i = 0;
      
      while (fgets(sopa[i], tamano, archivo))
         i++;

      int l = 0;
      i = 0;
      while ((i <= tamano) && No_encontrada){
         l = 0;
         
         while (l <= tamano && No_encontrada){
            if(sopa[i][l] == toupper(Palabra[0])){
               c = 0;
               a = i;

               while(sopa[a][l] == toupper(Palabra[c])){
                  if(c == largo){
                     No_encontrada = false;
                     printf("%s - ", Palabra);
                  }
                  a++;
                  c++;
               }
            }
            l+=2;
         }
         i++;
      }
   }

   
   tamano = (tamano - 3) / 2;
   struct stat buffer;
   char AUXILIAR[7];
   sprintf(AUXILIAR, "%i", tamano);
   
   char ruta_f[60] = "";
   tipo[strlen(tipo) - 2] = '\0';
   tipo[0] = toupper(tipo[0]);

   strcat(ruta_f, tipo);
   strcat(ruta_f, "/");
   strcat(ruta_f, AUXILIAR);
   strcat(ruta_f, "x");
   strcat(ruta_f, AUXILIAR);


   if(stat(ruta_f, &buffer))
      mkdir(ruta_f, 0777);
   
   strcat(ruta_f, "/");
   strcat(ruta_f, N_archivo);


   char instruccion[120] = "mv ";
   strcat(instruccion, ruta);
   strcat(instruccion, " ");
   strcat(instruccion, ruta_f);
   //system(instruccion);
   
   fin = clock();
   tiempo = fin - comienzo/(double)CLOCKS_PER_SEC;
   printf("%ld - %ld = %f\n", fin, comienzo, tiempo);   
   return 0;
}

int main(){
   DIR *dir;
   struct dirent *ent;

   dir = opendir("Archivos");
   if (dir == NULL){
      printf("NO SE PUDO ABRIR EL DIRECTORIO");
      return 1;
   }
 
   
   mkdir("Horizontal", 0777);
   mkdir("Vertical", 0777);
   

   while ((ent = readdir (dir)) != NULL){
      if ( (strcmp(ent->d_name, ".")!=0) && (strcmp(ent->d_name, "..")!=0) ){
         if(strstr(ent->d_name, ".txt"))
            leer(ent->d_name);
      }
   }
   
   closedir (dir);
   return 0;
   
}
