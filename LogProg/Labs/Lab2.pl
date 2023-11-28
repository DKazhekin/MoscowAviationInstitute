says("Andrey", X):-
    X = "Vitya"; X = "Tolya".

says("Vitya", X):-
    X \= "Vitya", X \= "Yura".

says("Dima", X):-
    (says("Andrey", X), not(says("Vitya", X)));
    (says("Vitya", X), not(says("Andrey", X))).

says("Yura", X):-
    not(says("Dima", X)).


true([],_).
true([H|T], X):-
        says(H, X), true(T, X).

solve(Answer):-
    member(Liar, ["Andrey", "Vitya", "Dima", "Yura"]),
    delete(["Andrey", "Vitya", "Dima", "Yura"], Liar, Final),
    true(Final, Answer), !.
