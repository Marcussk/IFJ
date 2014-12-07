{ nedokoncena definica lokalnej var }
var
    a : integer;
{ Definice funkce }
function factorial() : integer;
var
    n : integer
begin
    n := 0;
    write(n)
end;
{ Hlavni telo programu }
begin
    write('Zadejte cislo pro vypocet faktorialu: ');
    readln(a);
    vysl := factorial(a);
    write('Vysledek je: ', vysl, ''#10'')
    end
end.
