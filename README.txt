CAMPOS Ludovic, email: ludo830@hotmail.fr
CANNARD Adrien, email: adriencannard@gmail.com

Sujet : Casse-brique en GTK+
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~COMPILATION~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
Pour compiler, il suffit de rentrer dans un terminal la ligne de commande "make depend all".

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~LANCEMENT DU PROGRAMME~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
Une fois la compilation effectuée, vous pouvez lancer le programme en entrant la ligne de commande "./my_game"
Un menu s'ouvre alors, vous offrant différentes possibilités. Il suffit de cliquer sur l'option voulu pour la lancer.

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~CARACTERISTIQUES DU PROGRAMME~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
Le programme est assez complet étant donné que toutes les options présentées par l'énoncé ont été faite 
(sauf la version à deux joueurs).
Le programme dispose d'un mode de jeu normale, d'un éditeur de niveau avec la possibilité de jouer 
dans les niveaux créés. Il garde en mémoire les dix meilleurs scores disponibles depuis le menu de départ.  
Certaines briques renferment des bonus (avec un éventail de huit bonus différents tels que : la multi_balle, la planche collante ...).
Les briques ont jusqu'à trois solidités différentes.
Les rebonds de la balle varient en fonction de l'endroit où celle-ci touche la raquette.

Le code est assez rigoureux ce qui est plutôt interessant car on peut le compléter sans pour autant avoir besoin de tout changer.
D'autre part le casse brique peut prendre en argument un niveau édité, afin de jouer directement dans celui-ci, 
s'il n'y a pas d'argument il lance le jeu normalement.
De plus tous les niveaux du jeu par défaut tiennent dans un seul fichier ".txt", évitant ainsi de surcharger le dossier du programme.
Un point agréable du code de l'éditeur est qu'il demande automatiquement, lors de la fermeture du programme ou 
de l'ouverture d'un nouveau niveau, si le joueur veut sauvegarder son niveau (seulement si cela n'a pas été fait).
Enfin un dernier point intéressant est le menu apparaissant lors du lancement du programme.
   
