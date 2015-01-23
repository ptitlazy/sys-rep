# Ce fichier est ici présent à type d'exemple.
# Il montre comment tracer un graph avec des données brutes en entrée.
# A exécuter dans le dossier 'plots' en utilisant R grâce à la commande "source("template.R")".

source("plot.R");
#Données des abscisses.
x <- c(0,1,2,5,10,15)
#Données des ordonnées.
y <- c(mean(c(0.2,0.3,0.5)),mean(c(1,1.1)),mean(c(1.5,1.7)),mean(c(2.4,2.8)),mean(c(3.1,3.2)),mean(c(3.7,3.4,3.1)))
#Tracé du graph et sauvegarde dans exemple.png
plotgraph(x,y,"exemple","Abscisses","Ordonnées")

