{ prepis lokalnou hodnotou }
var
    n : integer;
{ Definice funkce }
function factorial() : integer;
var
    n : integer
begin
    n := 0;
    factorial := n;
end;
{ Hlavni telo programu }
begin
    n := 1;
    write('Global: ', n, ''#10'');
    vysl := factorial();
    write('Local: ', vysl, ''#10'')
    end
end.
