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


.. image:: ../images/map_architecture.png
  :width: 600
  :alt: Maps architecture
