<start> := <statement>;<start>

<statement> := 7(<expression>){<start>}<restOfCondition>
<statement> := 7(<expression>){<start>}
<statement> := cti
<statement> := ctfi(){<start>}
<statement> := ctfi(<functionFormalParameters>){<start>}
<statement> := di
<statement> := i=<expression>
<statement> := <expression>

<functionFormalParameters> := ti
<functionFormalParameters> := ti,<functionFormalParameters>

<expression> := l
<expression> := i
<expression> := i()
<expression> := i(<functionParameters>)
<expression> := (<expression>)
<expression> := lo<expression>
<expression> := io<expression>
<expression> := i()o<expression>
<expression> := i(<functionParameters>)o<expression>
<expression> := (<expression>)o<expression>

<functionParameters> := <expression>
<functionParameters> := <expression>,<functionParameters>

<restOfCondition> := 8(<expression>){<start>}<restOfCondition>
<restOfCondition> := 8(<expression>){<start>}
<restOfCondition> := e{<start>}

S -> cti;S | ctfi(F){T};S | ctfi(){T};S
     cti;  | ctfi(F){T};  | ctfi(){T}; 

F -> ti,F | ti

T -> 7(E){T}R;T | 7(E){T};T | cti;T | di;T | i=E;T | i();T | i(U);T | p(E);T | n(i);T | r(E);T
     7(E){T}R;  | 7(E){T};  | cti;  | di;  | i=E;  | i();  | i(U);  | p(E);  | n(i);  | r(E); 

R -> 8(E){T}R | 8(E){T} | e{T}

E -> l | i | i() | i(U) | (E) | loE | ioE | i()oE | i(U)oE | (E)oE

U -> l,U | i,U | i(),U | i(U),U | (E),U | loE,U | ioE,U | i()oE,U | i(U)oE,U | (E)oE,U
     l   | i   | i()   | i(U)   | (E)   | loE   | ioE   | i()oE   | i(U)oE   | (E)oE

*** --- ***

S -> 7(E){S}R; | 7(E){S}; | cti; | ctfi(){S}; | ctfi(F){S}; | di; | i=E; | i(); | i(U); | pE;
  -> 7(E){S}R;S | 7(E){S};S | cti;S | ctfi(){S};S | ctfi(F){S};S | di;S | i=E;S | i();S | i(U);S | pE;S

F -> ti | ti,F

E -> l | i | i() | i(U) | (E) | loE | ioE | i()oE | i(U)oE | (E)oE

U -> l | i | i() | i(U) | (E) | loE | ioE | i()oE | i(U)oE | (E)oE
  -> l,U | i,U | i(),U | i(U),U | (E),U | loE,U | ioE,U | i()oE,U | i(U)oE,U | (E)oE,U

R -> 8(E){S}R | 8(E){S} | e{S}
