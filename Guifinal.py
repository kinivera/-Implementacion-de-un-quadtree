# -*- coding: utf-8 -*-
from tkinter import * #libreria para diseñar ventana 
from PIL import ImageTk, Image


#medidas 
width = 800
height = 600

size_Pointer = 3
img_margin = [0.05,0.05] #Porcentage of margin (up/down,left/right)

#ventana que abre imagen y guarda coordenadas
global img
filename = "san_andres.bmp"
img = Image.open(filename)
img_orig_size = img.size

def abrirventanaclick():
    global img,img_orig_size
    ventana.withdraw()




# Forma en la que se carga la imagen     
    filename = "san_andres.bmp"
    img = Image.open(filename)
    
    width = int (img.size[0]*1.2)
    height =int( img.size[1]*1.2)

    new=Toplevel(ventana)
    new.geometry(f"{width}x{height}")
    ca = Canvas(new,width = width, height = height)
    ca.pack()
    
    etiqueta = ca.create_text(width/2,30,text=" Lugares turisticos cartagena ",font=("Helvetica Neue",15,"bold"),fill="black",anchor=CENTER)#texto
    
    img_orig_size = img.size
    print("img original",img_orig_size)
    
#Ajustar tamaño de la imagen    
    img = img.resize( (int(width*(1-2*img_margin[1])),int(height*(1-2*img_margin[0]))) )
    img= ImageTk.PhotoImage(img)
    mapa = ca.create_image(width*img_margin[1],height*img_margin[0], image=img, anchor=NW)
    
#ventana-margen=coordenadas mapa 
    verd_x = lambda x: img_orig_size[0] / (width*(1-img_margin[1]) -width*img_margin[1]) * (x-width*img_margin[1]) 
    verd_y = lambda Y: img_orig_size[1] /  (height*(1-img_margin[0])-height*img_margin[0])*(Y-height*img_margin[0])

    global prevx, prevy,flag, pointer
    prevx = 0
    prevy = 0
    flag = True

#Puntero, texto coordenadas 
    pointer = ca.create_oval(-1,-1,-1,-1,fill='red')
    coords_str = ca.create_text(width*(1-img_margin[1])-5,height*(1-img_margin[0])-5,text=f"Coordenadas: x={0} y = {0}",anchor=SE)

    
    new.protocol("WM_DELETE_WINDOW", lambda: [ventana.deiconify(),new.destroy()])#mantener ventana abierta



    def Mousemove(event):
    
        global prevx, prevy, flag,pointer
        #print (f"Mouse present in {event.x} {event.y}")
        #print (f"Mouse past was {prevx} {prevy}")
        
#Coordenadas y puntero solo funcionen dentro de los limites de la imagen
    
        if event.x >= ca.coords(mapa)[0] and event.x <= int(width*(1-img_margin[1])) and event.y >= ca.coords(mapa)[1] and event.y <= int(height*(1-img_margin[1])):
            if flag:
                pointer = ca.create_oval(event.x-size_Pointer/2,event.y-size_Pointer/2,event.x+size_Pointer/2,event.y+size_Pointer/2,fill='red')
            flag = False
            ca.move(pointer,event.x-prevx,event.y-prevy)
    
            pointer_coords = ((ca.coords(pointer)[0]+ca.coords(pointer)[2])/2,(ca.coords(pointer)[1]+ca.coords(pointer)[3])/2)
    
            # coordenadas(puntero) deben ser de (0,0) a (width,heigh) (d/(b-a))*(x-a)        
            coords = ( int(verd_x(pointer_coords[0])), int(verd_y(pointer_coords[1])) )
            
            #print(f"Pointer in: {pointer_coords}")
            #print(f"Coordenadas: x={coords[0]} y = {coords[1]}")
            ca.itemconfig(coords_str,text=f"Coordenadas: x={coords[0]} y = {coords[1]}")
    
            new.config(cursor='none')
        else:
            ca.delete(pointer)
            flag=True
            new.config(cursor='arrow')
        prevx = event.x
        prevy = event.y
        
        
    def click(event):
        
# guardar coordenadas con click solo si esta dentro de la imagen 
        print (f"Mouse Clicked in {event.x} {event.y}")
        if event.x >= ca.coords(mapa)[0] and event.x <= int(width*(1-img_margin[1])) and event.y >= ca.coords(mapa)[1] and event.y <= int(height*(1-img_margin[1])):
            pointer_coords = ((ca.coords(pointer)[0]+ca.coords(pointer)[2])/2,(ca.coords(pointer)[1]+ca.coords(pointer)[3])/2)
            coords = ( int(verd_x(pointer_coords[0])), int(verd_y(pointer_coords[1])) )
            save_coords(coords)
            
    
    def save_coords(pos):
        
# Guarda coordenadas o manda error 
        global img,img_orig_size
        try:
            f = open("map_coords.txt","w")
        except IOError:
            print("Error")
        f.write(f"{pos[0]},{pos[1]}")
        f.close()
        print("File saved, coords:",pos)
        
# Enlace con el quadtree de c++
        
        import subprocess
        subprocess.run(["a.exe"])

# Abre el archivo de genera c++
        
        f = open ('return.txt','r',encoding="utf-8")
        respuesta= f.readlines()
        print(respuesta)
        f.close()
        
# Muestra a manera de texto e imagen el resultado de c++ (Ubicacion encontrada mediante quadtree)
 
        img_margin = [0.05,0.005] #up/d left/r
        filename = respuesta[0][0:-1]
        img = Image.open(filename)
        #width = int (img.size[0]*1.2)
        height =int( img.size[1]*8)
        print(width)
        
       
        new.withdraw()
    
        new2=Toplevel(ventana)
        new2.geometry(f"{int(width*2.3)}x{height}")
        ca2 = Canvas(new2,width = width*2.3, height = height)
        ca2.pack()

    


        
        img_orig_size = img.size
        print("img original",img_orig_size)
        img = img.resize( (int(width*(1-2*img_margin[1])),int(height*(1-2*img_margin[0]))) )
        img= ImageTk.PhotoImage(img)
        mapa = ca2.create_image(width*img_margin[1],height*img_margin[0], image=img, anchor=NW)
    
        
        

        etiqueta2 = ca2.create_text(width+10,300,text="".join(respuesta[1:]),font=("Helvetica Neue",15),anchor=NW)
        
    
        
    
        etiqueta = ca2.create_text(width/2,10,text=" Lugares turisticos cerca de ti:  ",font=("Helvetica Neue",17),anchor=CENTER)
        
        new2.protocol("WM_DELETE_WINDOW", lambda: [new2.destroy(),abrirventanaclick()])

# Permiten la ejecucion del movimiento del mouse y el click mediante eventos   
    new.bind("<Motion>", Mousemove)
    new.bind("<Button-1>", click)

    new.protocol("WM_DELETE_WINDOW", lambda: [ventana.deiconify(),new.destroy()])


# ventana principal 


ventana=Tk()
ventana.title("Quadtree")
ventana.geometry(f"{width}x{height}")
ventana.resizable(True,True)
can = Canvas(ventana,width = width, height = height)
can.pack()

etiqueta = can.create_text(width/2,100,text=" Lugares turisticos cartagena ",font=("Helvetica Neue",30,"bold"),fill="black",anchor=CENTER)

btn = Button(can, text="Seleccionar ubicacion click",height=5,width=30,bg="light salmon", command=abrirventanaclick)
btn.place(relx=0.5, rely=0.5, anchor=CENTER)




#ventana.bind("<Motion>", Mousemove)
#ventana.bind("<Button-1>", click)
ventana.mainloop()
