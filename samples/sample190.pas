{ global var basics }
var
	res : integer;
{ Definice funkce }
function factorial() : integer;
var
    n : integer;
begin
    n := 0;
    factorial := res
end;

begin
	res :=5;
res := factorial();
write(res)
end.
