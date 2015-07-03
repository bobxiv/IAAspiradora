%Aspiradora

%Hechos
:- dynamic sucia/1.
sucia(1).%habitacion 1
sucia(2).%habitacion 2
sucia(3).%habitacion 3
sucia(4).%habitacion 4

:- dynamic estoy/1.
estoy(1).

%Regla principal
%aspirar(Path):- not(pruebaMeta), (limpiar(Path,NextPath); irH1(Path,NextPath); irH2(Path,NextPath); irH3(Path,NextPath); irH4(Path,NextPath)), aspirar(NextPath).
aspirar(Path):- aspirarAux([],Path).
aspirarAux(Path,Path2):- pruebaMeta(Path,Path2) -> ! ; ( (limpiar(Path,NextPath); girar(Path,NextPath)), aspirarAux(NextPath,Path2) ).

%Iguala dos listas o variables
equal(L,L).

%Operadores
%sucia(X) -> Limpiar(X) -> ~sucia(X)
limpiar(Path,NextPath):- estoy(Pos), sucia(Pos), retract(sucia(Pos)), append([Path,[limpia]],NextPath), write('limpia'), nl.

%(estoy(2) ; estoy(3)) -> irH1(X) -> estoy(1) ^ ~estoy(2) ^ ~estoy(3)
girar(Path,NextPath):- estoy(X), ( X=1 -> irH2(Path,NextPath) ; X=2 -> irH4(Path,NextPath) ; X=3 -> irH1(Path,NextPath) ; X=4 -> irH3(Path,NextPath)).

irH1(Path,NextPath):- (estoy(2) ; estoy(3)), retract(estoy(X)), assert(estoy(1)), append([Path,[irH1]],Path), write('irH1'), nl.
irH2(Path,NextPath):- (estoy(1) ; estoy(4)), retract(estoy(X)), assert(estoy(2)), append([Path,[irH2]],NextPath), write('irH2'), nl.
irH3(Path,NextPath):- (estoy(1) ; estoy(4)), retract(estoy(X)), assert(estoy(3)), append([Path,[irH3]],NextPath), write('irH3'), nl.
irH4(Path,NextPath):- (estoy(2) ; estoy(3)), retract(estoy(X)), assert(estoy(4)), append([Path,[irH4]],NextPath), write('irH4'), nl.

%Prueba de Meta
pruebaMeta(Path,Path2):- not(sucia(1)), not(sucia(2)), not(sucia(3)), not(sucia(4)), !, true, equal(Path2,Path).