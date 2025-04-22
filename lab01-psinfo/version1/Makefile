# Primer directorio en el MANPATH del sistema
# Con cut cortamos la salida del comando manpath que es el que necesitamos para extraer la dirección de las man pages del usuario
# Con -d cambiamos el delimitador por defecto para el corte que es el tab por los :
# -f1 es para que tomemos la primera parte que se cortó de la salida del comando manpath para guardar allí el zip
MANDIR := $(shell manpath | cut -d: -f1)
# Guardamos la man page en la sección 1
MANSECTION := 1

# Reglas de compilación
psinfo: main.o list.o functions.o
	gcc -o psinfo main.o list.o functions.o

main.o: main.c list.h
	gcc -c main.c

list.o: list.c list.h
	gcc -c list.c

functions.o: functions.c functions.h list.c list.h
	gcc -c functions.c

# Cuando se haga make install se puede ejecutar el programa como un comando global de la terminal
# install:
	# Creamos la carpeta que contendrá el ejecutable en el PATH
	#mkdir -p $(HOME)/.local/bin
	# Copiamos el ejecutable a la carpeta para que esté disponible cuando se llame
	#cp psinfo $(HOME)/.local/bin/

# Regla para instalar la man page comprimida en man1
# make install-man
install-man:
	#Comprimimos el archivo psinfo.1 donde se creó toda la documentación del comando psinfo
	gzip -c psinfo.1 > psinfo.1.gz
	#Creamos en la dirección que extraímos de las man pages un directorio donde contendrá las man pages de la sección 1, man1
	mkdir -p $(MANDIR)/man$(MANSECTION)
	#Copiamos dentro del directorio que creamos el comprimido
	cp psinfo.1.gz $(MANDIR)/man$(MANSECTION)/psinfo.1.gz

# Limpiar archivos generados
clean:
	rm -f *.o psinfo *.info psinfo.1.gz
	rm -f $(HOME)/.local/bin/psinfo
