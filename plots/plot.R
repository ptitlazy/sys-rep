######################################
# Fonctions de plot de données via R #
######################################

#idée de template pour organiser les données :
# x<-c(VALEURS DE L'ABSCISSE)
# y<-c(mean(c(DONNES ABSCISSE 1)), ..., mean(c(DONNES ABSCISSE n)))
# plotgraph(x,y,NOM DU FICHIER (sans le .png))
# Les données sont séparées par des virgules et doivent être des valeurs numériques. (float se fait avec un '.')

#Les deux derniers paramètres de la fonctions sont optionnels. Il s'agit du label des axes des abscisses et des ordonnées.
#Trace le graph avec deux vecteurs en entrée et le nom du fichier png à générer (qui est également le titre du graph)
plotgraph <- function(x, y, name, xlabel = "x", ylabel = "y")
{
	png(paste(name,".png",sep=""), width= 800, height = 600)
	plot(x,y,type="b", ann=FALSE, pch=4, xlim=c(0,max(x)), ylim=c(0,max(y)))
	title(main=name,xlab=xlabel,ylab=ylabel)
	dev.off()	
}
#TODO : Customiser le plot & en faire d'autres d'autres types (hist, etc...)
