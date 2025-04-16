# Primer directorio en el MANPATH del sistema
MANDIR := $(shell manpath | cut -d: -f1)
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

# Regla para instalar la man page comprimida en man1
install-man:
	gzip -c psinfo.1 > psinfo.1.gz
	mkdir -p $(MANDIR)/man$(MANSECTION)
	cp psinfo.1.gz $(MANDIR)/man$(MANSECTION)/psinfo.1.gz

# Limpiar archivos generados
clean:
	rm -f *.o psinfo *.info psinfo.1.gz

