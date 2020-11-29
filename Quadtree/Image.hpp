#ifndef _IMAGE_HPP
#define _IMAGE_HPP

#include<iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cstring>
#include <exception>
using namespace std;

struct fileHeader{
  unsigned long size; /*Tamaño de la imagen*/
  char resv1[4]; /*Reservados*/
  unsigned long offset; /*Tamaño de cabecera del archivo + cabecera imagen*/
};

struct bmpInfoHeader{
  unsigned int headersize; /*Tamaño cabecera*/
  unsigned int width; /*Ancho imagen*/
  unsigned int height; /*Alto de la imagen*/
  uint16_t planes; /*Planos de color (siempre 1)*/
  uint16_t bpp; /*bits por pixel (24 bits)*/
  unsigned int compress; /*Compresión*/
  unsigned int imgsize; /*Tamaño de datos de la imagen*/
  unsigned int bpmx; /*Resolución X en bits por metro*/
  unsigned int bpmy; /*Resolución Y en bits por metro*/
  unsigned int colors; /*Colores usados en la paleta*/
  unsigned int imxrcolors; /*Colores importantes. 0 si son todos*/
};

class Image{
private:
  fileHeader header;
  //unsigned char *imgdata;
public:
  unsigned char *imgdata;
  bmpInfoHeader info;

  Image();
  unsigned char *LoadBMP(char *fileName, bmpInfoHeader *bInfoHeader);
  void Divide(bmpInfoHeader info, char *fileName, int cuadrante);
  string rename(char* fileName, int n);
};

Image::Image(){
  imgdata = nullptr;
}

unsigned char* Image::LoadBMP(char *fileName, bmpInfoHeader *bInfoHeader){
  FILE *f;
  uint16_t type; /* 2 bytes identificativos */
  //fileHeader header;
  //unsigned char *imgdata;   /* datos de imagen */
  f=fopen (fileName, "r");
  if (!f)
    return NULL; /* Si no podemos leer, no hay imagen*/

    /* Leemos los dos primeros bytes */
  fread(&type, sizeof(uint16_t), 1, f);

  /*Comprobamos el formato*/
  if (type !=0x4D42){
    fclose(f);
    return NULL;
  }

  /* Leemos la cabecera de fichero completa */
  fread(&header, sizeof(fileHeader), 1, f);

  /* Leemos la cabecera de información completa */
  fread(bInfoHeader, sizeof(bmpInfoHeader), 1, f);

  /* Reservamos memoria para la imagen */
  imgdata=(unsigned char*)malloc(bInfoHeader->imgsize);

  /* Nos situamos en el sitio donde empiezan los datos de imagen,
   nos lo indica el offset de la cabecera de fichero*/
  fseek(f, header.offset, SEEK_SET);

  /* Leemos los datos de imagen, tantos bytes como imgsize */
  fread(imgdata, bInfoHeader->imgsize,1, f);

  /* Cerramos */
  fclose(f);

  /* Devolvemos la imagen */
  return imgdata;
}

void Image::Divide(bmpInfoHeader info, char *fileName, int cuadrante){
  FILE *fd;
  FILE *fo;
  fileHeader headerL;     /* cabecera */
  bmpInfoHeader infoL;
  infoL = info;
  unsigned char *data;
  int limitex = (info.width * 3);//+ (4-div(info.width,4).rem);
  int limitex1 = (info.width * 3);// + (4-div(info.width,4).rem);
  limitex1 = limitex1 + ((div(limitex1,4).rem <= 0) ? 0 :(4-div(limitex1,4).rem));

  int limitey = info.height;

  limitey = div(limitey,2).quot;
  limitex = div(limitex,2).quot;
  limitex = limitex + ((div(limitex,4).rem <= 0) ? 0 :(4-div(limitex,4).rem));

  int indicey = 0;

  try{
  fo=fopen (fileName, "r");
  if (!fo)
    cout << "Error al abrir el archivo de origen\n";  /* Si no podemos leer, no hay imagen*/

  cout << "Archivo origen abierto...\n";
  headerL.size = div(info.imgsize,4).quot+54;
  headerL.resv1[0] = '\0';
  headerL.resv1[1] = '\0';
  headerL.resv1[2] = '\0';
  headerL.resv1[3] = '\0';
  headerL.offset = 54;
  infoL.width = div(info.width,2).quot; /*x*/
  infoL.height = div(info.height,2).quot;
  infoL.imgsize = div(info.imgsize,4).quot; /*y*/
  infoL.bpmx = 0;
  infoL.bpmy = 0;

  string name = "";
  for(int k = 0; k<= strlen(fileName)-5; k++)
    name+=fileName[k];

  string s =  name+to_string(cuadrante)+".bmp";

  const char *cstr = s.c_str();

  fd=fopen(cstr,"w");
  fwrite("BM",1,sizeof(uint16_t),fd);
  fwrite(&headerL,1,sizeof(fileHeader),fd);
  fwrite(&infoL,1,sizeof(infoL),fd);
  fseek(fo, headerL.offset, SEEK_SET);
  data=(unsigned char*)malloc(limitex1);
  switch (cuadrante) {
    case 1: fseek(fo, limitex1*infoL.height, SEEK_CUR);
    cout << "Caso 1\n";
    break;
    case 2: fseek(fo, (limitex1*infoL.height)+ div(limitex1,2).quot, SEEK_CUR);
    cout << "Caso 2\n";
    break;
    case 4: fseek(fo, div(limitex1,2).quot, SEEK_CUR);
    cout << "Caso 4\n";
    break;
    default: cout << "Caso 3\n";
  }

  cout << "Entramos al while :)...\n";
  /*cout << "limitex1: " << limitex1 << "\n";
  cout << "limitex: " << limitex << "\n";*/

  do{
    fread(data,limitex1,1,fo);
    fwrite(data,1,limitex,fd);
    indicey++;
  }while(!feof(fo) && indicey < limitey);
  cout << "Salimos del while :D\n";
  fflush(fo);
  fclose(fo);
  cout << "Cerramos origen bien...\n";
  fflush(fd);
  fclose(fd);
  cout <<  "Cerrado de archivo " << cuadrante << " exitoso..\n";
}
catch (exception& e)
{
  cout <<  "Error: " << e.what() << "\n";
}
}


string Image::rename(char*fileName, int n){
  string subFileName = "";
  for(int k = 0; k<= strlen(fileName)-5; k++)
    subFileName+=fileName[k];
  string s = subFileName+to_string(n)+".bmp";
  return s;
}

#endif
