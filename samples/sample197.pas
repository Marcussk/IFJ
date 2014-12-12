{ redefinice sort }
var
	res : integer;
{ Definice funkce }
function sort(n : integer) : integer;
var
    n2 : integer;
begin
    sort := n+ 5
end;

begin
res := sort(sort(5));
write(res)
end.
