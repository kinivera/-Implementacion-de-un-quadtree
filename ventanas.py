from tkinter import *
from PIL import ImageTk, Image

width = 800
height = 600


def validarcoordenada():
    if entrada1.get()=="coordenada" :
    #revisar que la estructura de cordenada sea correcta
        messagebox.showwarning("Listo","coordenada guardada")
        # funcion que guarde la coordenada en archivo de texto
    else:
        messagebox.showwarning("Error","formato de coordenada incorrecto")

def abrirventanaclick():
    ventana.withdraw()

    new=Toplevel(ventana)
    new.geometry(f"{width}x{height}")

    etiqueta = canvas.create_text(width/2,10,text=" Lugares turisticos cartagena ",anchor=CENTER)

    # Img
    filename = "siluetaacartagena.bmp"

    img = Image.open(filename)
    img = img.resize( (int(width*0.8),int(height*0.8)) )
    img= ImageTk.PhotoImage(img)
    mapa = canvas.create_image(width*0.1,height*0.1, anchor=NW, image=img)

    botoncerrar= Button(new,text="Salir", command=new.destroy)
    botoncerrar.pack(side=TOP)
    new.mainloop()

def abrirventanacoor():
    ventana.withdraw()
    new=Toplevel(ventana)

    canvas = Canvas(new,width = width, height = height)
    canvas.pack()

    new.geometry(f"{width}x{height}")
    etiqueta = canvas.create_text(width/2,10,text=" ingrese coordenada ",anchor=CENTER)


    new.protocol("WM_DELETE_WINDOW", lambda: [ventana.deiconify(),new.destroy()])

ventana=Tk()
ventana.title("Quadtree")
ventana.geometry(f"{width}x{height}")
ventana.resizable(True,True)
canvas = Canvas(ventana,width = width, height = height)
canvas.pack()

etiqueta = canvas.create_text(width/2,10,text=" Lugares turisticos cartagena ",anchor=CENTER)

btn = Button(canvas, text="Seleccionar ubicacion click",height=5,width=30, command=abrirventanaclick)
btn.place(relx=0.7, rely=0.5, anchor=CENTER)

btn2 = Button(canvas, text="Seleccionar ubicacion manual",height=5,width=30,command=abrirventanacoor)
btn2.place(relx=0.3, rely=0.5, anchor=CENTER)

ventana.mainloop()
