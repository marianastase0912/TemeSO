Nume: Nastase Maria

Grupă: 335CC

# Tema 1

1. Soluția de ansamblu aleasă:

Am implementat un preprocesor pentru fisiere cu cod C. Directivele de efinitie din linia de comanda sau din fisiere sunt stocate intr-un hashmap pentru a le putea inlocui mai usor in etapa de procesare. Directiva de include deschide un nou fisier pentru a putea fi procesat. Directivele de conditie sunt evaluate doar la true sau false, urmand ca in fisierul procesat sa existe numai branch-ul care se poate evalua.


Implementare
- Directiva DEFINE
    * Pentru stocarea in memorie a variabilelor de tip -D<VAR>=<VALUE> si #define <VAR> este implementat un HashMap.
    * In implemntarea structurii de date se regasesc functiile de init, insert, delete, getValue, getKey, find si free.
    * Pentru fiecare definitie de variabila, aceasta este inserata in hashmap.
    * Daca exista in fisier un define, este adaugat in hashmap si se verifica daca valoarea variabilei definite exista deja in map, in caz afirmativ este inlocuita cu valoarea deja stocata.
    * Variabilele stocate vor fi inlocuite corespunzator in fisierul procesat.
 
- Directiva Include
    * Prima data se verifica daca fisierul este in directorul curent fisierului de procesat.
    * Daca nu exista, se verifica daca acesta este prezent in directoarele pasate ca argument cu flagurile -I care sunt stocate intr-un array.
    * Daca fisierul nu exista sau nu se poate deschide nici in path-urile acestea, programul va returna un semnal de eroare ENOENT.
    * Fisierele deschise vor fi procesate.
 
 - Directiva Condition
    * Pentru fiecare conditie intalnita, folosind atoi() se verifica daca aceasta se evalueaza la true sau la false
    * Pentru conditia evaluata ca true, se va scrie in fisierul de output numai branch-ul respectiv.
    * In cazul unui branch evaluat la false, se cauta un else sau un elif la care abordarea este asemanatoare.
 
- Daca nu este primit un fisier de input, se asteapta de la stdin, daca nu este primit un fisier de output, se scrie la stdout.
- Daca sunt primite mai multe fisiere, priumul este considerat fisier de input, al doilea de output, restul sunt ignorate.
- Se verifica fiecare alocare de memorie, in caz de esec se returneaza un semnal de eroare

- Tema este utila deoarece este o oprtunitate de recapitulare a lucrului cu memoria in C, de familiarizare cu standardele de coding style din kernel.
- Consider implemnatrea eficienta.

Cum se compilează și cum se rulează?
- Se linkeaza fisierele obiect hashmap.o: hashmap.c hashmap.h, utilsFnctions.o : utilsFunctions.c utils.h si so-cpp.o: so-cpp.c
* Exemplu de rulare ./so-cpp <argumente>

Bibliografie

- https://ocw.cs.pub.ro/courses/so/laboratoare/laborator-01
- https://ocw.cs.pub.ro/courses/so/laboratoare/laborator-02

Git
- https://github.com/marianastase0912/TemeSO
