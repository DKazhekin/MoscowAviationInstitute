% Запускаем цикл Элизы
eliza :-
    write("Hello, what's up ? Talk to me.."), nl,
    eliza_loop.

% Сам цикл Элизы (чтение ввода и вызов предиката, определяющего формат вывода)
eliza_loop :-
    write('Eliza --> '),
    read(Input), respond(Input).

% Динамический вывод разных ответов на неизвестные Элизе предложения
:- dynamic excuses/1.

excuses(["Okaay..", "I am good", "Hm..", "Let me think a bit..", "And what's next.."]).

% Всевозможные ответы Элизы
respond(Input) :-
    member(Word, Input),
    member(Word, [quit, leave, bye, exit]),
    write('Nice to meet you, bye-bye'), nl.

respond([_,my,name,is, Name | _]) :-
    write('Hello,'), write(Name), write('! Pleased to meet you!'), nl,
    eliza_loop.

respond(Input) :-
    member(Word1, Input),
    member(Word1, [ hate, dislike ]),
    member(Word2, Input),
    member(Word2, [ you ]),
    write("Noo, of course it's false! :("), nl,
    eliza_loop.

respond(Input) :-
    member(Animal, Input),
    member(Animal, [dog, cat, fish, hamster, gerbil, snake, tortoise]),
    write("Hmm... Tell me more about pretty "),
    write(Animal), nl,
    eliza_loop.

respond(Input) :-
    member(Word, Input),
    member(Word, [upset, tired, confused, broken]),
    member(Word1, Input),
    member(Word1, [ i ]),
    write("To not to be "),
    write(Word),
    write(" just get distracted and relax a bit"), 
    nl,
    eliza_loop.

respond([how,are,you]) :-
    write("Yea I am fine, thanks, and you ?"), 
    nl,
    eliza_loop.

respond(Input) :-
    member(Word, Input),
    member(Word, [happy, joyful, fine]),
    write(Word),
    write("! Good mood is very important in business"), 
    nl,
    eliza_loop.

respond(Input) :-
    member(Word1, Input),
    member(Word1, [ think, suppose, tell]),
    member(Word2, Input),
    member(Word2, [ you, me ]),
    write("I do not know what to "),
    write(Word1),
    write(" the better way is to apply to your circle"), nl,
    eliza_loop.

respond(Input) :-
    member(Word, Input),
    member(Word, [what]),
    member(Word1, Input),
    member(Word1, [name]),
    write("My favourite name is Eliza"),
    nl,
    eliza_loop.

% Ответы Элизы, , причем динамические, на непонятные ей высказывания
respond(_) :-
    retract(excuses([ Head | Tail ])),
    append(Tail, [ Head ], Newexcuses),
    asserta(excuses(Newexcuses)),
    write(Head),
    nl,
    eliza_loop.
