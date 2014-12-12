{ kontrola akceptovania forward }
var
    x : integer;
{ Definice funkce }
function factorial() : integer; forward;
var
    n : integer
begin
    n := 0;
    factorial := n;
end;
{ Hlavni telo programu }
begin
    x := factorial();
    write(x)
end.
