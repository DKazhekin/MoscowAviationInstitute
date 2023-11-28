sentance(X,Y) --> main_part(X), but_part(Y).

main_part(X) --> person(Actor), verb_group(Actor, X).
but_part(Y) --> but(_), person(Actor), verb_group(Actor, Y).

verb_group(Actor, X) --> verb(Actor, Y, X), object(Y).

object(Y) --> prime_object(Y).

verb(X,Y, likes(X, Y)) --> [likes].
verb(X,Y, dislikes(X, Y)) --> [dislikes].

prime_object(apples) --> [apples].
prime_object(grapes) --> [grapes].
prime_object(nuts) --> [nuts].
prime_object('grapes and apples') --> [grapes, and, apples].
prime_object('apples and grapes') --> [apples, and, grapes].
prime_object('nuts and apples') --> [nuts, and, apples].
prime_object('apples and nuts') --> [apples, and, nuts].
prime_object('grapes and nuts') --> [grapes, and, nuts].
prime_object('nuts and tomatoes') --> [nuts, and, tomatoes].

person(nyusha) --> [nyusha].
person(misha) --> [misha].

but(but) --> [but].
