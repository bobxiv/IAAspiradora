%Aspiradora

%Hechos
:- dynamic sucia/1.
sucia(1).%habitacion 1
sucia(2).%habitacion 2
sucia(3).%habitacion 3
sucia(4).%habitacion 4

estoy(1).

%Regla principal
aspirar(Pos,Path):- not(pruebaMeta), (limpiar(Pos,Path); irH1(Pos,Path); irH2(Pos,Path); irH3(Pos,Path); irH4(Pos,Path)), aspirar(Pos,Path).

%Operadores
%sucia(X) -> Limpiar(X) -> ~sucia(X)
limpiar(Pos,[Path|limpia]):- sucia(Pos), retract(sucia(Pos)), write('limpia'), nl.

%(Pos=h2 ; Pos=h3) -> irH1(X) -> Pos=h1
irH1(Pos,[Path|irH1]):- (Pos=:=2 ; Pos=:=3), Pos = 1, write('irH1'), nl.
irH2(Pos,[Path|irH2]):- (Pos=:=1 ; Pos=:=4), Pos = 2, write('irH2'), nl.
irH3(Pos,[Path|irH3]):- (Pos=:=1 ; Pos=:=4), Pos = 3, write('irH3'), nl.
irH4(Pos,[Path|irH4]):- (Pos=:=2 ; Pos=:=3), Pos = 4, write('irH4'), nl.

%Prueba de Meta
pruebaMeta:- not(sucia(1)), not(sucia(2)), not(sucia(3)), not(sucia(4)), !.