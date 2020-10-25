#include<iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

using namespace std;


struct Header{
  //char type[2]; /* 2 bytes de identificación */
  unsigned long size;        /* Tamaño del archivo */
  char resv1[4];       /* Reservado */
  //uint16_t resv2;       /* Reservado */
  unsigned long offset;      /* Offset hasta hasta los datos de imagen */
};

struct bmpInfoHeader{
  unsigned int headersize;      /* Tamaño de la cabecera */
  unsigned int width;       /* Ancho */
  unsigned int height;      /* Alto */
  uint16_t planes;          /* Planos de color (Siempre 1) */
  uint16_t bpp;             /* bits por pixel */
  unsigned int compress;        /* compresión */
  unsigned int imgsize;     /* tamaño de los datos de imagen */
  unsigned int bpmx;        /* Resolución X en bits por metro */
  unsigned int bpmy;        /* Resolución Y en bits por metro */
  unsigned int colors;      /* colors used en la paleta */
  unsigned int imxtcolors;      /* Colores importantes. 0 si son todos */
};

unsigned char *LoadBMP(char *filename, bmpInfoHeader *bInfoHeader);
void DisplayInfo(bmpInfoHeader *info);
void TextDisplay(bmpInfoHeader *info, unsigned char *img);
void SaveBMP(bmpInfoHeader *info, unsigned char *img);
void Divide(bmpInfoHeader *info, char *nomArchivo);

// void DisplayInfo1(Header *info);

int main(){
  bmpInfoHeader info;
  unsigned char *img;
  char nombre[] = "bandera.bmp";
  img=LoadBMP(nombre, &info);
  DisplayInfo(&info);
  TextDisplay(&info, img);
  Divide(&info, nombre);
  return 0;
}

unsigned char *LoadBMP(char *filename, bmpInfoHeader *bInfoHeader){
  FILE *f;
  Header header;     /* cabecera */
  unsigned char *imgdata;   /* datos de imagen */
  uint16_t type;        /* 2 bytes identificativos */

  f=fopen (filename, "r");
  if (!f)
    return NULL;        /* Si no podemos leer, no hay imagen*/

    /* Leemos los dos primeros bytes */
     fread(&type, sizeof(uint16_t), 1, f);
     if (type !=0x4D42)        /* Comprobamos el formato */
       {
         fclose(f);
         return NULL;
       }

  /* Leemos la cabecera de fichero completa */
  fread(&header, sizeof(Header), 1, f);
  // DisplayInfo1(&header);
  // if (header.type !=0x4D42){
  //   cout << "Error: tipo de archivo incorrecto";       /* Comprobamos el formato */
  //   fclose(f);
  //   return NULL;
  // }

  /* Leemos la cabecera de información completa */
  fread(bInfoHeader, sizeof(bmpInfoHeader), 1, f);

  /* Reservamos memoria para la imagen, ¿cuánta?
     Tanto como indique imgsize */
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

// void DisplayInfo1(Header *info){
//   cout << "Tipo: " << info->type << "\n";
//   printf("Tamaño del archivo: %u\n", info->size);
//   printf("Reservado: %c\n", info->resv1);
//   printf("Reservado: %c\n", info->resv2);
//   printf("Offset: %d\n", info->offset);
// }

void TextDisplay(bmpInfoHeader *info, unsigned char *img)
{
  int x, y;
  /* Reducimos la resolución vertical y horizontal para que la imagen entre en pantalla */
  static const int reduccionX=1, reduccionY=2;
  /* Si la componente supera el umbral, el color se marcará como 1. */
  static const int umbral=90;
  /* Asignamos caracteres a los colores en pantalla */
  static unsigned char colores[9]=" bgfrRGB";
  int r,g,b;

  /* Dibujamos la imagen */
  for (y=info->height; y>0; y-=reduccionY)
    {
      for (x=0; x<info->width; x+=reduccionX)
    {
      b=(img[3*(x+y*info->width)]>umbral);
      g=(img[3*(x+y*info->width)+1]>umbral);
      r=(img[3*(x+y*info->width)+2]>umbral);

      printf("%c", colores[b+g*2+r*4]);
    }
      printf("\n");
    }
}

void DisplayInfo(bmpInfoHeader *info)
{
  printf("Tamaño de la cabecera: %u\n", info->headersize);
  printf("Anchura: %d\n", info->width);
  printf("Altura: %d\n", info->height);
  printf("Planos (1): %d\n", info->planes);
  printf("Bits por pixel: %d\n", info->bpp);
  printf("Compresión: %d\n", info->compress);
  printf("Tamaño de datos de imagen: %u\n", info->imgsize);
  printf("Resolucón horizontal: %u\n", info->bpmx);
  printf("Resolucón vertical: %u\n", info->bpmy);
  printf("Colores en paleta: %d\n", info->colors);
  printf("Colores importantes: %d\n", info->imxtcolors);
}

void SaveBMP(bmpInfoHeader *info, unsigned char *img){
  FILE *f;
  Header header;     /* cabecera */
  header.size = info->imgsize+54;
  header.resv1[0] = '\0';
  header.resv1[1] = '\0';
  header.resv1[2] = '\0';
  header.resv1[3] = '\0';
  header.offset = 54;
  f=fopen("copia.bmp","w");
  fwrite("BM",1,sizeof(uint16_t),f);
  fwrite(&header,1,sizeof(Header),f);
  fwrite(info,1,sizeof(*info),f);
  fwrite(img,1,info->imgsize,f);
  fclose(f);
}

void Divide(bmpInfoHeader *info, char *nomArchivo){
  FILE *fd;
  FILE *fo;
  Header header;     /* cabecera */
  unsigned char *data;
//  int limitex = div(info->width,2).quot;
  int limitex = info->width * 3;
//  int limitey = div(info->height,2).quot;
  int limitey = info->height * 3;
  int indicex = 0;
  int indicey = 0;

  fo=fopen (nomArchivo, "r");
  if (!fo)
    cout << "Error al abrir el archivo de origen";        /* Si no podemos leer, no hay imagen*/
  cout << "Archivo origen abierto...";
  header.size = div(info->imgsize,4).quot+54;
  header.resv1[0] = '\0';
  header.resv1[1] = '\0';
  header.resv1[2] = '\0';
  header.resv1[3] = '\0';
  header.offset = 54;
  info->width = div(info->width,2).quot;
  info->height = div(info->height,2).quot;
  info->imgsize = div(info->imgsize,4).quot;
  info->bpmx = 0;
  info->bpmy = 0;
  fd=fopen("divide.bmp","w");
  fwrite("BM",1,sizeof(uint16_t),fd);
  fwrite(&header,1,sizeof(Header),fd);
  fwrite(info,1,sizeof(*info),fd);
  fseek(fo, header.offset, SEEK_SET);
  data=(unsigned char*)malloc(limitex);
  do{
    fread(data,limitex,1,fo);
    fwrite(data,1,limitex,fd);
    //fseek(fo, limitex, SEEK_CUR);
    indicex++;
    indicey++;
  }while(indicex <= 50 && indicey <= 50);
  fclose(fd);
  fclose(fo);
}
