{ Program 1: Vypocet faktorialu (iterativne) }
var
    a : integer;
    vysl : integer;
begin
    a := 5;
    write('Zadejte cislo pro vypocet faktorialu: ');
    readln(a);
    if a < 0 then
    begin
        write('Faktorial nelze spocitat'#10'')
    end
    else
    begin
        vysl := 1;
        while a > 0 do
        begin
            vysl := vysl * a;
            a := a - 1
        end;
        write('Vysledek je: ', vysl,''#10'' )
    end
end.
