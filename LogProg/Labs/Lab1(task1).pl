% Первая часть задания - предикаты работы со списками (9 вариант по списку, 10 вариант первого задания, 15 вариант второго задания)

% length() function

length([],0).
length([H|T], N):- length([T,T1], N1), N = N1 + 1.

% member() function

member([X|_],X).
member([_|T], X):-
	member(T,X).

% append() function

append([], X, X).
append([H|T], X, [H|X2]):- append(T,X,X2).

% remove() function

del(Y,[Y], []).
del(X, [X|LIST1], LIST1).
del(X, [Y|LIST], [Y|LIST1]):- del(X,LIST,LIST1).

% permutation() function

permutation([],[]).
permutation(L,[X|P]):- del(X,L,L1), permutation(L1,P).

% sublist() function

sublist([],[]).

sublist([First|Rest],[First|Sub]):- sublist (Rest,Sub).
sublist([First|Rest], Sub):- sublist(Rest, Sub).

% Вставка элемента в список на указанную позицию

% 1-ый способ

include(List, X, 1, [X|List]).
include([L1|L0], X, N, R):-
	Nn is N-1,
	include(L0, X, Nn, R0),
	R = [L1|R0].


% 2-ой способ с исользованием функции "remove()"

insert(Y,O_List, Number, N_List):-
	remove(Y, N_List, Number, O_List).

remove(Y, [Y|List1], 1, List1).
remove(Y, [B|List2], Number,[B|List1]):-
	Nn is Number - 1,
	remove(Y,List2, Nn,List1).

% Вычисление позиции первого отрицательного элемента в списке

indexOf([Element|_], 1):- 
	Element < 0.
indexOf([_|Tail], Index):-
  indexOf(Tail, Index1),
  !,
  Index is Index1+1.
