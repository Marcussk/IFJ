{ Empty function }
var
	res : integer;
{ Definice funkce }

function foo2() : integer ; forward;

function foo1(n : integer) : integer;
var 
	a : integer;
begin
	foo1 := foo2() * n
end;

function foo2() : integer;
var
    n : integer;
begin
	n := 5;
    foo2 := n
end;

begin

res := foo1(2);
write(res)

end.