import sys
import os
import re

if len(sys.argv) != 2:
	print('Erreur : nombre d\'arguments incorrect', file=sys.stderr)
	quit()

filesIncluded = []

def getNameIncludedFile(line):
	line = str(line)
	i = 10
	l = []
	while line[i] != "\"" :
		l.append(line[i])
		i += 1
	return "".join(l)


def printFile(nomFile):
	#Ouverture
	nomFile = os.path.abspath(nomFile)
	if nomFile in filesIncluded:
		return
	#print(nomFile)
	filesIncluded.append(nomFile)
	file = open(nomFile,"r")

	#On se place dans le répertoire du fichier
	os.chdir(os.path.dirname(nomFile))
	content = file.read().split('\n')

	for line in content:
		if line.strip() == "#pragma once":
			continue
		#line = line.strip()
		if re.match(r"\#include \".+\"",line.strip()) != None:
			includedFile = getNameIncludedFile(line)
			printFile(os.path.abspath(includedFile))
			os.chdir(os.path.dirname(nomFile))
			print("")
			continue
		print(line)

	#Fermeture du fichier
	file.close()

printFile(sys.argv[1])