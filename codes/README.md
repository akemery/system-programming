# Producteurs consommateurs
## Problème
Deux classes de processus
Les producteurs produisent de l'information et la place dans un
buffer partagé
Les consommateurs traitent de l'information en allant la chercher
dans un buffer partagé

Comment coordonner ces processus ?

```
 _ _ _ _ _ _ _
|_|_|_||_|_|_|

```

```
/**Initialisation **/
Semaphore mutex = 1 ;
Semaphore empty = N ;
Semaphore full = 0 ;

Void producer(void){
  int item;
  while(TRUE){
    item=produce(item);
    down(&empty);
    down(&mutex);
    insert_item();
    up(&mutex);
    up(&full);
  }
}
```

```
void consumer(void)
{
  int item;
  while(TRUE){
    down(&full);
    down(&mutex);
    item=remove(item);
    up(&mutex);
    up(&empty);
  }
```

Mutex : exclusion mutuelle pour l'accès au buffer
Empty : nombre de places libres dans le buffer
Full : nombre de places occupées dans le buffer

Implémenter dans le langage de votre choix l'algorithme. Les producteurs produisent
toutes les 2 secondes tandis que les consommateurs consomment toutes les 6 secondes.

Vous devez donner le choix à l'utilisateur de saisir au démarrage du programme le
nombre de producteurs, le nombre de consommateurs et le nombres d'élements maximum 
à produire (i.e la taille de la mémoire partagée).

Donner la possibilité d'afficher toutes les secondes le contenu de la mémoire partagée.


# Pistes

## Exemple d'exécution de fork_ex.c

```
$ gcc fork_ex.c -o fork
$ ./fork
```

## Exemple d'exécution de exec_ex.c
Compilation de test_facto.c pour produire le programme `facto`

``` 
$ gcc test_facto.c -o facto
```

Compilation de test_pal.c pour produire le programme `pal`

```
$ gcc test_pal.c -o pal
```

Test de `facto`

```
$ ./facto
USAGE: ./facto <entier>

$ ./facto 12
Le factoriel de 12 est 479001600
```

Test de `pal`

```
./pal 
USAGE: ./pal <chaine de caractere>
./pal sas
sas est un palindrome
```

Compilation de exec_ex.c

```
$ gcc exec_ex.c -o exec
```

Test de `exec`

```
$ ./exec kevin
kevin n'est pas un palindrome

$ ./exec 12
Le factoriel de 12 est 479001600

```

## Exemple d'exécution de shared_mem.c
### Compilation

```
gcc shared_mem.c -o shared_mem

```

### Tests
Vous pouvez répéter plusieurs fois l'étape suivante:

```
./shared_mem
```

Pour plus d'information sur les mémoires paratgées, les sémaphores veuillez
visiter le lien suivant: https://docs.oracle.com/cd/E19455-01/806-4750/6jdqdfltg/index.html



