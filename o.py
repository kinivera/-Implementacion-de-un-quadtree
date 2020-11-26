from tkinter import *
from PIL import ImageTk, Image

# Main Window
width = 800
height = 900

size_Pointer = 3
img_margin = [0.05,0.05] #Porcentage of margin (up/down,left/right)

ventana=Tk()
ventana.title("Quadtree")
ventana.geometry(f"{width}x{height}")
ventana.resizable(True,True)
canvas = Canvas(ventana,width = width, height = height)
canvas.pack()

etiqueta = canvas.create_text(width/2,10,text=" Lugares turisticos cartagena ",anchor=CENTER)

# Img
filename = "siluetaacartagena.bmp"

img = Image.open(filename)
img_orig_size = img.size
print("img original",img_orig_size)
img = img.resize( (int(width*(1-2*img_margin[1])),int(height*(1-2*img_margin[0]))) )
img= ImageTk.PhotoImage(img)
mapa = canvas.create_image(width*img_margin[1],height*img_margin[0], anchor=NW, image=img)
verd_x = lambda x: img_orig_size[0] / (width*(1-img_margin[1]) -width*img_margin[1]) * (x-width*img_margin[1]) #ventana-margen=coordenadas mapa 
verd_y = lambda Y: img_orig_size[1] /  (height*(1-img_margin[0])-height*img_margin[0])*(Y-height*img_margin[0])
#canvas.delete(mapa)


#btn = Button(canvas, text="Seleccionar ubicacion",height=20,width=20)
#btn.place(relx=0.5, rely=0.5, anchor=CENTER)
global prevx, prevy,flag, pointer
prevx = 0
prevy = 0
flag = True
pointer = canvas.create_oval(-1,-1,-1,-1,fill='red')
coords_str = canvas.create_text(width*(1-img_margin[1])-5,height*(1-img_margin[0])-5,text=f"Coordenadas: x={0} y = {0}",anchor=SE)
def Mousemove(event):
    global prevx, prevy, flag,pointer
    #print (f"Mouse present in {event.x} {event.y}")
    #print (f"Mouse past was {prevx} {prevy}")

    if event.x >= canvas.coords(mapa)[0] and event.x <= int(width*(1-img_margin[1])) and event.y >= canvas.coords(mapa)[1] and event.y <= int(height*(1-img_margin[1])):
        if flag:
            pointer = canvas.create_oval(event.x-size_Pointer/2,event.y-size_Pointer/2,event.x+size_Pointer/2,event.y+size_Pointer/2,fill='red')
        flag = False
        canvas.move(pointer,event.x-prevx,event.y-prevy)

        pointer_coords = ((canvas.coords(pointer)[0]+canvas.coords(pointer)[2])/2,(canvas.coords(pointer)[1]+canvas.coords(pointer)[3])/2)

        # coordenadas(puntero) deben ser de (0,0) a (width,heigh) (d/(b-a))*(x-a)        
        coords = ( int(verd_x(pointer_coords[0])), int(verd_y(pointer_coords[1])) )
        
        print(f"Pointer in: {pointer_coords}")
        print(f"Coordenadas: x={coords[0]} y = {coords[1]}")
        canvas.itemconfig(coords_str,text=f"Coordenadas: x={coords[0]} y = {coords[1]}")

        ventana.config(cursor='none')
    else:
        canvas.delete(pointer)
        flag=True
        ventana.config(cursor='arrow')
    prevx = event.x
    prevy = event.y
def click(event):
    print (f"Mouse Clicked in {event.x} {event.y}")
    if event.x >= canvas.coords(mapa)[0] and event.x <= int(width*(1-img_margin[1])) and event.y >= canvas.coords(mapa)[1] and event.y <= int(height*(1-img_margin[1])):
        pointer_coords = ((canvas.coords(pointer)[0]+canvas.coords(pointer)[2])/2,(canvas.coords(pointer)[1]+canvas.coords(pointer)[3])/2)
        coords = ( int(verd_x(pointer_coords[0])), int(verd_y(pointer_coords[1])) )
        save_coords(coords)
        print("File saved, coords:",coords)

def save_coords(pos):
    try:
        f = open("map_coords.txt","w")
    except IOError:
        print("Error")
    f.write(f"{pos[0]},{pos[1]}")
    f.close()

ventana.bind("<Motion>", Mousemove)
ventana.bind("<Button-1>", click)
ventana.mainloop()
