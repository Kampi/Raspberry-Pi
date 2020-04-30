from xml.dom.minidom import Document

# Erzeugt den Kopf des KML Datei
# Rueckgabewerte:
#	1. Das XML-Baum Objekt
#	2. Das Wurzel Objekt
def CreateHead():

	# XML Dokument erzeugen
	Baum = Document()

	# Wurzelelement festlegen
	Attr = Baum.createElement("kml")
	Attr.setAttribute("xmls", "http://earth.google.com/kml/2.2")
	Baum.appendChild(Attr)
	Wurzel = Baum.createElement("Document")
	Attr.appendChild(Wurzel)
	
	return (Baum, Wurzel)

# Schreibt die Informationen fuer einen Punkt in die KML Datei
# Uebergabeparameter: 
#	1. Der Name des XML-Baum Objektes
#	2. Der Name des Wurzel (Uebergeordnetes Element) Objektes
#	3. Der Name des Punktes
#	4. - 6. Die drei Koordinaten des Punktes
# Rueckgabewerte:
#	1. Es wird der neue XML-Baum zurueck gegeben
def WritePoint(Baum, Wurzel, Mein_Name, Koordinate_1, Koordinate_2, Koordinate_3):

	# Placemark Element erstellen
	Placemark = Baum.createElement("Placemark")
	Wurzel.appendChild(Placemark)
	
	# Dem Element den uebergebenen Namen zuordnen
	Name = Baum.createElement("name")
	Inhalt = Baum.createTextNode(Mein_Name)
	Name.appendChild(Inhalt)
	Placemark.appendChild(Name)
	
	# Dem Element die Punktkoordinaten zuordnen
	Point = Baum.createElement("Point")
	Placemark.appendChild(Point)
	
	# Unterpunkt "coordinates" erzeugen und mit Koordinaten fuellen
	Coordinates = Baum.createElement("coordinates")
	Inhalt = Baum.createTextNode(str(Koordinate_1) + "," + str(Koordinate_2) + "," + str(Koordinate_3))
	Coordinates.appendChild(Inhalt)
	Point.appendChild(Coordinates)
	
	# Den neuen Baum zurueck ins Hauptprogramm uebergeben
	return Baum
	
# Schreibt die Informationen fuer eine Linie in die KML Datei
# Uebergabeparameter: 
#	1. Der Name des XML-Baum Objektes
#	2. Der Name der Wurzels (Uebergeordnetes Element) Objektes
#	3. Der Name der Linie
#	4. Eine Liste mit Koordinaten W/E, N/S, h (in drei Dimensionen [[E1, N1, h1], [E2, N2, h2], ...]
# Rueckgabewerte:
#	1. Es wird der neue XML-Baum zurueck gegeben
def WriteLine(Baum, Wurzel, Mein_Name, Koordinaten):
	
	# Placemark Element erstellen
	Placemark = Baum.createElement("Placemark")
	Wurzel.appendChild(Placemark)
	
	# Dem Element den uebergebenen Namen zuordnen
	Name = Baum.createElement("name")
	Inhalt = Baum.createTextNode(Mein_Name)
	Name.appendChild(Inhalt)
	Placemark.appendChild(Name)
	
	# Dem Element das Linestring Element zuordnen
	Linestring = Baum.createElement("LineString")
	Placemark.appendChild(Linestring)
	
	# Unterpunkt "coordinates" erzeugen und mit Koordinaten fuellen
	Coordinates = Baum.createElement("coordinates")
	
	# Koordinaten anfuegen
	for Element in Koordinaten:
		Inhalt = Baum.createTextNode(str(Element[0]) + "," + str(Element[1]) + "," + str(Element[2]))
		Coordinates.appendChild(Inhalt)

	Linestring.appendChild(Coordinates)
	
	# Den neuen Baum zurueck ins Hauptprogramm uebergeben
	return Baum


## Beispiel zur Verwendung ##
# Liste mit Fake-Koordinaten
Liste= [[6.903310,51.097691,0.000000], [6.902008,51.104309,0.000000], [6.901815,51.104954,0.000000]]

# KML Datei erzeugen
(Mein_Baum, Meine_Wurzel) = CreateHead()

# Zwei Punkte und eine Strecke abspeichern
Mein_Baum = WritePoint(Mein_Baum, Meine_Wurzel, "Start", 6.903310,51.097691,0.000000)
Mein_Baum = WriteLine(Mein_Baum, Meine_Wurzel, "Route", Liste)
Mein_Baum = WritePoint(Mein_Baum, Meine_Wurzel, "Ende", 6.903102,51.098263,0.000000)

# XML Datei schreiben
Datei = open("Geo.kml", "wb")
Mein_Baum.writexml(Datei, "", "\t", "\n") 

# Datei freigeben
Datei.close()