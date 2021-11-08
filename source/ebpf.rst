Qu'est-ce que l'eBPF ?
----------------------

eBPF est une technologie révolutionnaire issue du noyau Linux qui peut exécuter des programmes en bac à sable dans un noyau de système d'exploitation. Il est utilisé pour étendre de manière sûre et efficace les capacités du noyau sans avoir à modifier le code source du noyau ou à charger les modules du noyau.

Historiquement, le système d'exploitation a toujours été un endroit idéal pour mettre en œuvre des fonctionnalités d'observabilité, de sécurité et de mise en réseau en raison de la capacité privilégiée du noyau à superviser et contrôler l'ensemble du système. Dans le même temps, un noyau de système d'exploitation est difficile à faire évoluer en raison de son rôle central et de ses exigences élevées en matière de stabilité et de sécurité. Le taux d'innovation au niveau du système d'exploitation a donc traditionnellement été inférieur à celui des fonctionnalités mises en œuvre en dehors du système d'exploitation.

.. image:: ../images/ebpfoverview.png
  :width: 600
  :alt: eBPF overview
  

eBPF change fondamentalement cette formule. En permettant d'exécuter des programmes en bac à sable dans le système d'exploitation, les développeurs d'applications peuvent exécuter des programmes eBPF pour ajouter des fonctionnalités supplémentaires au système d'exploitation lors de l'exécution. Le système d'exploitation garantit alors la sécurité et l'efficacité d'exécution comme s'il était compilé nativement à l'aide d'un compilateur et d'un moteur de vérification Just-In-Time (JIT). Cela a conduit à une vague de projets basés sur eBPF couvrant un large éventail de cas d'utilisation, y compris les fonctionnalités de mise en réseau, d'observabilité et de sécurité de nouvelle génération.

Aujourd'hui, eBPF est largement utilisé pour gérer une grande variété de cas d'utilisation : fournir une mise en réseau et un équilibrage de charge hautes performances dans les centres de données modernes et les environnements cloud natifs, extraire des données d'observabilité de la sécurité à granularité fine à faible coût, aider les développeurs d'applications à tracer les applications, fournissant des informations pour le dépannage des performances, l'application préventive de la sécurité des applications et des conteneurs, et bien plus encore. Les possibilités sont infinies et l'innovation que eBPF a débloquée ne fait que commencer.

Aperçu du crochet (hook)
========================

Les programmes eBPF sont pilotés par les événements et sont exécutés lorsque le noyau ou une application passe un certain point de crochet. Les hooks prédéfinis incluent les appels système, l'entrée/la sortie de fonction, les points de trace du noyau, les événements réseau et plusieurs autres.

.. image:: ../images/hookoverview.png
  :width: 600
  :alt: Hook overview
  
Si un hook prédéfini n'existe pas pour un besoin particulier, il est possible de créer une sonde noyau (kprobe) ou une sonde utilisateur (uprobe) pour attacher des programmes eBPF presque n'importe où dans les applications noyau ou utilisateur.


.. image:: ../images/hookoverview2.png
  :width: 600
  :alt: Hook overview with process
  

Comment sont écrits les programmes eBPF ?
=========================================

Dans de nombreux scénarios, eBPF n'est pas utilisé directement mais indirectement via des projets comme `Cilium <https://ebpf.io/projects/#cilium>`_   , `bcc <https://ebpf.io/projects/#bcc>`_ ou `bpftrace <https://ebpf.io/projects/#bpftrace>`_ qui fournissent une abstraction au-dessus d'eBPF et ne nécessitent pas d'écrire des programmes directement mais offrent à la place la possibilité de spécifier des définitions basées sur l'intention qui sont ensuite implémentés avec eBPF.


.. image:: ../images/clang.png
  :width: 600
  :alt: Clang overview
  

S'il n'existe aucune abstraction de niveau supérieur, les programmes doivent être écrits directement. Le noyau Linux s'attend à ce que les programmes eBPF soient chargés sous forme de bytecode. Bien qu'il soit bien sûr possible d'écrire du bytecode directement, la pratique de développement la plus courante consiste à tirer parti d'une suite de compilateurs telle que `LLVM <https://llvm.org/>`_ pour compiler du code pseudo-C en bytecode eBPF.


Chargeur et architecture de vérification
========================================

Lorsque le hook souhaité a été identifié, le programme eBPF peut être chargé dans le noyau Linux à l'aide de l'appel système bpf. Cela se fait généralement à l'aide de l'une des bibliothèques eBPF disponibles. La section suivante fournit une introduction aux chaînes d'outils de développement disponibles.

.. image:: ../images/verificateur.png
  :width: 600
  :alt: Go library architecture
  

Au fur et à mesure que le programme est chargé dans le noyau Linux, il passe par deux étapes avant d'être attaché au hook demandé :

Vérification
............

L'étape de vérification garantit que le programme eBPF est sûr à exécuter. Il valide que le programme remplit plusieurs conditions, par exemple :

.. image:: ../images/loader.png
  :width: 600
  :alt: Chargement d'un programme eBPF
  
  
* Le processus de chargement du programme eBPF détient les capacités requises (privilèges). À moins que l'eBPF non privilégié ne soit activé, seuls les processus privilégiés peuvent charger les programmes eBPF.
* Le programme ne plante pas et n'endommage pas le système.
* Le programme s'exécute toujours jusqu'à la fin (c'est-à-dire que le programme ne reste pas dans une boucle pour toujours, ce qui retarde le traitement).

Compilation JIT
...............

L'étape de compilation Just-in-Time (JIT) traduit le bytecode générique du programme dans le jeu d'instructions spécifiques à la machine pour optimiser la vitesse d'exécution du programme. Cela permet aux programmes eBPF de s'exécuter aussi efficacement que le code du noyau compilé en natif ou que le code chargé en tant que module du noyau.

Maps
....

Un aspect essentiel des programmes eBPF est la capacité de partager les informations collectées et de stocker l'état. À cette fin, les programmes eBPF peuvent tirer parti du concept de Maps eBPF pour stocker et récupérer des données dans un large ensemble de structures de données. Les Maps eBPF sont accessibles depuis les programmes eBPF ainsi que depuis les applications dans l'espace utilisateur via un appel système.


.. image:: ../images/maparchitecture.png
  :width: 600
  :alt: Maps architecture
  
  Ce qui suit est une liste incomplète des types de Maps pris en charge pour donner une compréhension de la diversité des structures de données. Pour différents types de Maps, une variante partagée et une variante par CPU sont disponibles.
  
* Hash tables, Arrays
* LRU (Least Recently Used)
* Ring Buffer
* Stack Trace
* LPM (Longest Prefix match)


Helper Calls
............

Les programmes eBPF ne peuvent pas appeler des fonctions arbitraires du noyau. Permettre cela lierait les programmes eBPF à des versions de noyau particulières et compliquerait la compatibilité des programmes. Au lieu de cela, les programmes eBPF peuvent effectuer des appels de fonction dans des fonctions helpers, une API bien connue et stable offerte par le noyau.

.. image:: ../images/helper.png
  :width: 600
  :alt: Helper call
  
L'ensemble des calls helper disponibles est en constante évolution. Exemples de call helpers disponibles :

     * Générer des nombres aléatoires
     * Obtenir l'heure et la date actuelles
     * Accès à la Map eBPF
     * Obtenir le contexte du processus/groupe de contrôle
     * Manipuler les paquets réseau et la logique de transfert
     
Tail & Function Calls
.....................

Les programmes eBPF sont composables avec le concept d'appels de queue et de fonction. Les appels de fonction permettent de définir et d'appeler des fonctions dans un programme eBPF. Les appels de queue peuvent appeler et exécuter un autre programme eBPF et remplacer le contexte d'exécution, de la même manière que l'appel système execve() fonctionne pour les processus normaux.

.. image:: ../images/tailcall.png
  :width: 600
  :alt: Tail call
  

eBPF Safety
===========

Grand pouvoir, rime avec une grande responsabilité.

eBPF est une technologie incroyablement puissante et s'exécute désormais au cœur de nombreux composants critiques de l'infrastructure logicielle. Lors du développement d'eBPF, la sécurité d'eBPF était l'aspect le plus crucial lorsque eBPF a été envisagé pour l'inclusion dans le noyau Linux. La sécurité eBPF est assurée par plusieurs couches :


Privilèges requis
.................

Sauf si eBPF non privilégié est activé, tous les processus qui ont l'intention de charger des programmes eBPF dans le noyau Linux doivent s'exécuter en mode privilégié (root) ou nécessiter la capacité CAP_BPF. Cela signifie que les programmes non approuvés ne peuvent pas charger les programmes eBPF.

Si eBPF non privilégié est activé, les processus non privilégiés peuvent charger certains programmes eBPF soumis à un ensemble de fonctionnalités réduit et avec un accès limité au noyau.


Vérificateur
............

Si un processus est autorisé à charger un programme eBPF, tous les programmes passent toujours par le vérificateur eBPF. Le vérificateur eBPF assure la sécurité du programme lui-même. Cela signifie, par exemple :

     * Les programmes sont validés pour s'assurer qu'ils s'exécutent toujours jusqu'à leur achèvement, par ex. un programme eBPF ne peut jamais bloquer ou rester dans une boucle pour toujours. Les programmes eBPF peuvent contenir ce que l'on appelle des boucles limitées, mais le programme n'est accepté que si le vérificateur peut s'assurer que la boucle contient une condition de sortie qui est garantie de devenir vraie.
     * Les programmes ne doivent pas utiliser de variables non initialisées ou accéder à la mémoire hors limites.
     * Les programmes doivent correspondre aux exigences de taille du système. Il n'est pas possible de charger des programmes eBPF arbitrairement volumineux.
     * Le programme doit avoir une complexité finie. Le vérificateur évaluera tous les chemins d'exécution possibles et doit être capable de terminer l'analyse dans les limites de la limite supérieure de complexité configurée.
     
Durcissement
............

Une fois la vérification terminée avec succès, le programme eBPF exécute un processus de durcissement selon que le programme est chargé à partir d'un processus privilégié ou non.

Cette étape comprend :

    * Protection de l'exécution du programme : la mémoire du noyau contenant un programme eBPF est protégée et en lecture seule. Si pour une raison quelconque, qu'il s'agisse d'un bug du noyau ou d'une manipulation malveillante, le programme eBPF est tenté d'être modifié, le noyau plantera au lieu de lui permettre de continuer à exécuter le programme corrompu/manipulé.
    * Atténuation contre Spectre : sous la spéculation, les processeurs peuvent prédire de manière erronée les branches et laisser des effets secondaires observables qui pourraient être extraits via un side channel. Pour ne citer que quelques exemples : les programmes eBPF masquent l'accès mémoire afin de rediriger l'accès sous des instructions transitoires vers des zones contrôlées, le vérificateur suit également des chemins de programme accessibles uniquement en exécution spéculative et le compilateur JIT émet des Retpolines au cas où les appels de queue ne peuvent pas être convertis en appels directs .
    * Blindage constant : toutes les constantes du code sont masquées pour empêcher les attaques par pulvérisation JIT. Cela empêche les attaquants d'injecter du code exécutable en tant que constantes qui, en présence d'un autre bogue du noyau, pourraient permettre à un attaquant de sauter dans la section mémoire du programme eBPF pour exécuter du code.
    
Contexte d'exécution abstrait
.............................

Les programmes eBPF ne peuvent pas accéder directement à la mémoire arbitraire du noyau. L'accès aux données et aux structures de données qui se trouvent en dehors du contexte du programme doit être accessible via les helpers eBPF. Cela garantit un accès cohérent aux données et soumet un tel accès aux privilèges du programme eBPF, par ex. un programme eBPF en cours d'exécution est autorisé à modifier les données de certaines structures de données si la modification peut être garantie pour être sûre. Un programme eBPF ne peut pas modifier aléatoirement les structures de données dans le noyau.

Le pouvoir de la programmabilité
................................

Commençons par une analogie. Vous vous souvenez de GeoCities ? Il y a 20 ans, les pages Web étaient presque exclusivement écrites en langage de balisage statique (HTML). Une page Web était essentiellement un document avec une application (navigateur) capable de l'afficher. En regardant les pages Web aujourd'hui, les pages Web sont devenues des applications à part entière et la technologie Web a remplacé une grande majorité d'applications écrites dans des langages nécessitant une compilation. Qu'est-ce qui a permis cette évolution ?

.. image:: ../images/geocities.png
  :width: 600
  :alt: Geocities
  
La réponse courte est la programmabilité avec l'introduction de JavaScript. Il a débloqué une révolution massive entraînant l'évolution des navigateurs vers des systèmes d'exploitation presque indépendants.

Pourquoi l'évolution s'est-elle produite ? Les programmeurs n'étaient plus aussi liés aux utilisateurs exécutant des versions de navigateur particulières. Au lieu de convaincre les organismes de normalisation qu'une nouvelle balise HTML était nécessaire, la disponibilité des blocs de construction nécessaires a découplé le rythme d'innovation du navigateur sous-jacent de l'application exécutée par dessus. C'est bien sûr un peu simpliste car le HTML a évolué au fil du temps et a contribué au succès, mais l'évolution du HTML lui-même n'aurait pas été suffisante.

Avant de prendre cet exemple et de l'appliquer à eBPF, examinons quelques aspects clés qui ont été essentiels à l'introduction de JavaScript :

* Sureté : Un code non fiable s'exécute dans le navigateur de l'utilisateur. Ce problème a été résolu en mettant en sandbox les programmes JavaScript et en faisant abstraction de l'accès aux données du navigateur.
* Livraison continue : L'évolution de la logique du programme doit être possible sans nécessiter d'envoyer constamment de nouvelles versions de navigateur. Cela a été résolu en fournissant les bons blocs de construction de bas niveau suffisants pour construire une logique arbitraire.
* Performance : La programmabilité doit être fournie avec un minimum de coûts. Ce problème a été résolu avec l'introduction d'un compilateur Just-in-Time (JIT).

Pour tout ce qui précède, des contre-parties exactes peuvent être trouvées dans eBPF pour la même raison.

L'impact d'eBPF sur le noyau Linux
..................................

Revenons maintenant à eBPF. Afin de comprendre l'impact de la programmabilité d'eBPF sur le noyau Linux, il est utile d'avoir une compréhension de haut niveau de l'architecture du noyau Linux et de la façon dont il interagit avec les applications et le matériel.

.. image:: ../images/kernel_arch.png
  :width: 600
  :alt: Architecture du kernel
  
L'objectif principal du noyau Linux est d'abstraire le matériel ou le matériel virtuel et de fournir une API cohérente (appels système) permettant aux applications d'exécuter et de partager les ressources. Pour y parvenir, un large ensemble de sous-systèmes et de couches est maintenu pour répartir ces responsabilités. Chaque sous-système permet généralement un certain niveau de configuration pour tenir compte des différents besoins des utilisateurs. Si un comportement souhaité ne peut pas être configuré, une modification du noyau est requise, historiquement, laissant deux options :

* Support natif

     (1) Modifier le code source du noyau et convainquez la communauté du noyau Linux que le changement est nécessaire.
     (2) Attendre plusieurs années que la nouvelle version du noyau devienne disponible au grand public.


* Module de noyau
     (1) Écrire un module noyau
     (2) Le mettre à jour régulièrement, car chaque version du noyau peut le casser
     (3) Risque de corrompre votre noyau Linux en raison du manque de limites de sécurité
     
Avec eBPF, une nouvelle option est disponible qui permet de reprogrammer le comportement du noyau Linux sans nécessiter de modifications du code source du noyau ou de charger un module de noyau. À bien des égards, cela ressemble beaucoup à la façon dont JavaScript et d'autres langages de script ont déverrouillé l'évolution de systèmes devenus difficiles ou coûteux à modifier.

Outils de développement
=======================

Plusieurs outils de développement existent pour aider au développement et à la gestion des programmes eBPF. Tous répondent à des besoins différents des utilisateurs :

BCC
...

BCC est un framework qui permet aux utilisateurs d'écrire des programmes Python avec des programmes eBPF intégrés à l'intérieur. Le framework est principalement destiné aux cas d'utilisation qui impliquent le profilage/traçage d'applications et de systèmes où un programme eBPF est utilisé pour collecter des statistiques ou générer des événements et un équivalent dans l'espace utilisateur collecte les données et les affiche sous une forme lisible par l'humain. L'exécution du programme python générera le bytecode eBPF et le chargera dans le noyau.


.. image:: ../images/bcc.png
  :width: 600
  :alt: BCC
 
funccount
+++++++++

Compte les évènements, spécialement les appels de fontion et peut répondre aux questions suivantes:

 * Est ce une fonction kernel ou une fonction utilisateur?
 * Quel est le taux d'appel de cette fonction /seconde  

execsnoop
+++++++++

Trace les nouveau processus exécutés. Il peut être utilisé pour pour détecter les problèmes des processus à courte durée de vie et peut être aussi utiliser pour débugger l'exécution des applications.

.. code-block:: console

   emery@assogba:~/Documents/2022/uac/cours_os$ execsnoop.bt -h
   

exitsnoop
+++++++++

Trace les processus qui exit. Affiche leur âge et la raison pour laquelle il se termine.

.. code-block:: console

   emery@assogba:~/Documents/2022/uac/cours_os$ exitsnoop.bt -h


runqlat
+++++++

Permet de mesurer la latence de l'ordonnanceur du CPU. Peut permettre d'identifier les problèmes liés à la saturation du CPU.

.. code-block:: console

   emery@assogba:~/Documents/2022/uac/cours_os$ runqlat.bt -h
 

runqlen
+++++++

Permet de compter le nombre de tâche attendant leur tour pour être exécutées.

.. code-block:: console

   emery@assogba:~/Documents/2022/uac/cours_os$ runqlen.bt -h
   

runqslower
++++++++++

Liste les instance de processus dépassant la limite de latence configurée.


.. code-block:: console

   emery@assogba:~/Documents/2022/uac/cours_os$ runqslower.bt -h
   
   
cpudist
+++++++

Affiche la distribution des tâches par CPU

.. code-block:: console

   emery@assogba:~/Documents/2022/uac/cours_os$ cpudist.bt -h
   

 

bpftrace
........

bpftrace est un langage de traçage de haut niveau pour Linux eBPF et disponible dans les noyaux Linux récents (4.x). bpftrace utilise LLVM comme backend pour compiler des scripts en bytecode eBPF et utilise BCC pour interagir avec le sous-système Linux eBPF ainsi que les capacités de traçage Linux existantes : traçage dynamique du noyau (kprobes), traçage dynamique au niveau utilisateur (uprobes) et tracepoints. Le langage bpftrace est inspiré des traceurs awk, C et prédécesseurs tels que DTrace et SystemTap.

.. image:: ../images/bpftrace.png
  :width: 600
  :alt: bpftrace
  
  
Bibliothèque eBPF Go
....................

La bibliothèque eBPF Go fournit une bibliothèque eBPF générique qui découple le processus d'accès au bytecode eBPF et le chargement et la gestion des programmes eBPF. Les programmes eBPF sont généralement créés en écrivant un langage de niveau supérieur, puis en utilisant le compilateur clang/LLVM pour compiler en bytecode eBPF.

.. image:: ../images/go.png
  :width: 600
  :alt: Go
  

Bibliothèque libbpf C/C++

La bibliothèque libbpf est une bibliothèque eBPF générique basée sur C/C++ qui aide à découpler le chargement des fichiers objets eBPF générés à partir du compilateur clang/LLVM dans le noyau et fait généralement abstraction de l'interaction avec l'appel système BPF en fournissant des API de bibliothèque faciles à utiliser pour les applications.

.. image:: ../images/libbpf.png
  :width: 600
  :alt: libbpf

Demo BPF applications
=====================

Point de départ pour le développement d'applications BPF avec libbpf et BPF CO-RE.

`libbpf-bootstrap <https://github.com/libbpf/libbpf-bootstrap>_` 

Il permet d'avoir un point de départ efficace pour débuter le développement d'application
basée sur eBPF.


BCC
...

` BCC <https://github.com/iovisor/bcc>_`


Type de programme eBPF
----------------------

Types de programmes liés aux sockets - SOCKET_FILTER, SK_SKB, SOCK_OPS
======================================================================

Tout d'abord, considérons les types de programmes liés aux sockets qui nous permettent de filtrer, de rediriger les données de socket et de surveiller les événements de socket. Le cas d'utilisation du filtrage est lié aux origines de BPF. Lors de l'observation du réseau, nous voulons voir seulement une partie du trafic réseau, par exemple tout le trafic provenant d'un système problématique. Les filtres sont utilisés pour décrire le trafic que nous voulons voir, et idéalement nous voulons qu'il soit rapide, et nous voulons donner aux utilisateurs un ensemble ouvert d'options de filtrage. Mais nous avons un problème ; nous voulons jeter les données inutiles le plus tôt possible, et pour ce faire, nous devons filtrer dans le contexte du noyau. Considérez l'alternative à une solution dans le noyau - encourir le coût de la copie des paquets dans l'espace utilisateur et du filtrage là-bas. Cela coûterait très cher, surtout si nous voulons seulement voir une partie du trafic réseau et jeter le reste. Pour y parvenir, un mini-langage sûr a été inventé pour traduire les filtres de haut niveau en un programme de bytecode que le noyau peut utilisation (appelée BPF classique, cBPF). L'objectif du langage était de prendre en charge un ensemble flexible d'options de filtrage tout en étant rapide et sûr. Les filtres écrits dans ce langage de type assembleur pourraient être poussés par des programmes en espace utilisateur tels que tcpdump pour effectuer le filtrage dans le noyau.

Voir

https://www.tcpdump.org/papers/bpf-usenix93.pdf

...pour l'article classique décrivant ce travail. L'eBPF moderne a repris ces concepts, étendu le registre et le jeu d'instructions, ajouté des structures de données appelées Maps, considérablement élargi les types d'événements auxquels nous pouvons nous attacher, et bien plus encore !

Pour le filtrage des sockets, le cas courant est de s'attacher à un socket brut (SOCK_RAW), et en fait, vous remarquerez que la plupart des programmes qui filtrent les sockets ont une ligne comme celle-ci :

.. code-block:: c

  s = socket(AF_PACKET,SOCK_RAW,htons(ETH_P_ALL));
  
  
En créant un tel socket, nous spécifions le domaine (AF_PACKET), le type de socket (SOCK_RAW) et le protocole (tous les types de paquets). Dans le noyau Linux, la réception de paquets bruts est implémentée par la fonction raw_local_deliver(). Il est appelé par ip_local_deliver_finish(), juste avant d'appeler le gestionnaire du protocole IP concerné, qui est l'endroit où le paquet est passé à TCP, UDP, ICMP, etc. Ainsi, à ce stade, le trafic n'a pas été associé à un socket spécifique ; cela se produit plus tard, lorsque la pile IP détermine le mappage du paquet au protocole de couche 4, puis au socket correspondant (le cas échéant). Vous pouvez voir les bytecodes cBPF générés par tcpdump en utilisant l'option -d. Ici, je veux exécuter tcpdump sur l'interface wlp4s0, en filtrant uniquement le trafic TCP :


.. code-block:: console

  # tcpdump -i wlp4s0 -d 'tcp'
  (000) ldh      [12]
  (001) jeq      #0x86dd          jt 2    jf 7
  (002) ldb      [20]
  (003) jeq      #0x6             jt 10    jf 4
  (004) jeq      #0x2c            jt 5    jf 11
  (005) ldb      [54]
  (006) jeq      #0x6             jt 10    jf 11
  (007) jeq      #0x800           jt 8    jf 11
  (008) ldb      [23]
  (009) jeq      #0x6             jt 10    jf 11
  (010) ret      #65535
  (011) ret      #0
  

BPF_PROG_TYPE_SOCKET_FILTER
...........................

Les actions de filtrage incluent la suppression de paquets (si le programme renvoie 0) ou la suppression de paquets (si le programme renvoie une longueur inférieure à l'original). Voir sk_filter_trim_cap() et son appel à bpf_prog_run_save_cb(). Notez que nous ne rognons ni ne supprimons le paquet d'origine qui atteindrait toujours intact le socket prévu ; nous travaillons avec une copie des métadonnées du paquet auxquelles les sockets bruts peuvent accéder pour l'observabilité. En plus de filtrer le flux de paquets vers notre socket, nous pouvons également faire des choses qui ont des effets secondaires ; par exemple la collecte de statistiques dans les Maps BPF.

Les programmes BPF peuvent être attachés aux sockets via SO_ATTACH_BPF setsockopt(), qui transmet un descripteur de fichier au programme.

Quel contexte est fourni ? Un pointeur vers la structure __sk_buff contenant les métadonnées/données du paquet. Cette structure est définie dans include/linux/bpf.h, et inclut les champs clés du vrai sk_buff. Le vérificateur bpf convertit l'accès aux champs __sk_buff valides en décalages dans le "vrai" sk_buff, voir https://lwn.net/Articles/636647/ pour plus de détails.

    Quand fonctionne-t-il ? Les filtres de socket s'exécutent pour la réception dans sock_queue_rcv_skb() qui est appelé par divers protocoles (TCP, UDP, ICMP, sockets bruts, etc.) et peut être utilisé pour filtrer le trafic entrant.

Pour donner une idée de l'apparence des programmes, nous allons créer ici un filtre qui coupe les données de paquets que nous filtrons sur la base du type de protocole ; pour IPv4 TCP, prenons uniquement l'en-tête IPv4 + TCP, tandis que pour UDP, nous prendrons uniquement l'en-tête IPv4 et UDP. Nous ne traiterons pas des options IPv4 car c'est un exemple simple, donc dans tous les autres cas, nous renvoyons 0 (drop packet).


