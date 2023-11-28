% Task 2: Relational Data (Данные из файла: two.pl, 1 вариант задания)

% The line below imports the data
:- ['two.pl'].

avg( List, Avg ):-
    sumlist( List, Sum ),
    length( List, Length),
    (  Length > 0
    -> Avg is Sum / Length
    ;  Avg is 0
    ).
    
% First task

group_n_score(Group, [Group|Avg]):-
	findall(X, grade(Group,_,_,X), X),
	avg(X, Avg).


answer1() :-
    group_n_score(101,X),
    group_n_score(102,Y),
    group_n_score(103,Z),
    group_n_score(104,W),
    write(X), nl, write(Y), nl, write(Z), nl, write(W), fail.
    
% Second task

failed_4_each_subject(Subject, Result):-									
	findall(Y, (grade(_,Y,Subject,Mark), Mark=2), Result).
	
answer2() :-
    failed_4_each_subject('Психология', X),
    failed_4_each_subject('Английский язык', Y),
    failed_4_each_subject('Информатика', Z),
    failed_4_each_subject('Функциональное программирование', W),
    failed_4_each_subject('Математический анализ', F),
    failed_4_each_subject('Логическое программирование', G),
    write(X), nl, write(Y), nl,write(Z), nl,write(W),
    nl, write(F), nl, write(G), nl,
    fail.

% Third task

failed_4_each_group(G, List):-									
	findall(Student, (grade(G, Student,_, M), M=2), Student),
	length(Student, List).
	
answer3() :-
    failed_4_each_group(101, X),
    failed_4_each_group(102, Y),
    failed_4_each_group(103, Z),
    failed_4_each_group(104, W),
    write(X), nl, write(Y), nl,
    write(Z), nl, write(W), nl,
    fail.
