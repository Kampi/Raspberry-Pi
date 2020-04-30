Pfad="/mnt/Treiber"

# In das Treiberverzeichnis wechseln
cd $Pfad

# Treiber kompilieren
make

# Modul laden
insmod Treiber.ko

# Geraetedatei anlegen
mknod /dev/Treiber c 240 0

gcc $Pfad/Treiber_Test.c -o $Pfad/Treiber_Test
$Pfad/Treiber_Test

# Treiber entfernen
rmmod Treiber

# Geraet loeschen
rm /dev/Treiber