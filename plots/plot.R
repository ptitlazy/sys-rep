######################################
# Fonctions de plot de données via R #
######################################

#idée de template pour organiser les données :
# x<-c(VALEURS DE L'ABSCISSE)
# y<-c(mean(c(DONNES ABSCISSE 1)), ..., mean(c(DONNES ABSCISSE n)))
# plotgraph(x,y,NOM DU FICHIER (sans le .png))
# Les données sont séparées par des virgules et doivent être des valeurs numériques. (float se fait avec un '.')


#Trace le graph avec deux vecteurs en entrée et le nom du fichier png à générer
plotgraph <- function(x, y, name)
{
	png(paste(name,".png",sep=""), width= 800, height = 400)
	plot(x,y)
	dev.off()	
}
#TODO : Customiser le plot & en faire d'autres d'autres types (hist, etc...)
