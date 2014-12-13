{ global var basics }
var
	res : integer;
{ Definice funkce }
function factorial() : integer;
var
    n : integer;
begin
	res :=5;
    n := 0;
    factorial := res
end;

begin
	
res := factorial();
write(res)
end.
