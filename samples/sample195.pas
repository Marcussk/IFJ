{ incompatible forward }
var
{ Definice funkce }
function factorial(n : integer) : integer; forward;
function factorial() : integer;
var
    n : integer;
begin
    n := 0;
    factorial := res
end;

begin
	res := factorial();
	write(res)
end.
