# Ako zo snarku spraviť multipól šírky 6?

Na vytvorenie multipólu šírky 6 z daného snarku je viacero spôsobov, priblížime 4 z nich. Existuje aj spôsob, akým vieme
vypočítať šírku výsledného multipólu, ktorý platí pre každý graf, nie len snark.

> Na začiatku je stupeň multipólu 0
> Odstránenie bodu: stupeň multipólu += (stupeň daného bodu – počet “visiacich hrán“)
> Odstránenie hrany (nevisiacej): stupeň multipólu += 2
> _Pozn.: stupeň rátame bez visiacich hrán_

- **Odstránením 3 hrán**
    - Odstránením jednej hrany sa zníži stupeň oboch incidentných vrcholov o 1, čím sa v našom prípade zvýši stupeň
      multipólu o 2.
    - Odstránením 3 hrán nám tým pádom vznikne multipól st. 6.
    - **Nemôžeme** ale odstrániť hranu, ktorá z grafu spraví nesúvislý. V tomto prípade nemôžeme odstrániť 3 hrany
      incidentné s jedným vrcholom.

- **Odstránením 2 bodov, ktoré nie sú incidentné**
- **Odstránením 2 incidentných bodov a ľubovoľnej hrany, ktorá nie je incidentná s ani jedným z týchto bodov**
    - Toto sa dá ľahko ošetriť tým, že hranu odstránime až po odstránení bodov.
- **Odstránením bodov cesty dĺžky 4**
- **Odstránením bodu a jeho 3 susedov**

---

# How to create multipole with degree 6 from snark?

There are multiple ways to create a multipole with degree 6 from given snark, we will show 4 of them. Before that we
will show a solution on how to compute degree of resulting multipole, which applies to every graph, not only snarks.

> At the beggining the degree of multipole is 0
> Removing of vertex: degree of multipole += (degree of given vertex - number of unconnected edges from given vertex)
> Removing of edge (connected): degree of multipole += 2
> _Note: We compute degree of vertex without unconnected edges_

- **Removing 3 edges**
    - By removing one edge, the degree of both incident vertices decrements, which results in degree of the multipole
      incrementing by 2.
    - Thus by removing three edges, the resulting multipole will have degree 6.
        - **Warning:** We can't remove edge that will result in making the graph discontinuos. In this case we can't
          remove 3 edges incident with one vertex.

- **Removing 2 non-incidental vertices**
- **Removing 2 incidental vertices and one edge non-incidental with any of those two vertices**
- We can fulfill this by removing the edge after removing the vertices.
- **Removing vertices on path with length 4**
- **Removing a vertex and its 3 neighbours**
