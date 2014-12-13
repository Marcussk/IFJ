{ incompatible forward }
var
	res : integer;
{ Definice funkce }
function factorial(n : integer) : integer;
var
    n2 : integer;
begin
    factorial := n+ 5
end;

begin
res := factorial(factorial(5));
write(res)
end.
