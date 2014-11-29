{ Program 1: Vypocet faktorialu (iterativne) }
var
    a : integer;
    vysl : integer;
begin
    a := 5;
    write('Zadejte cislo pro vypocet faktorialu: ');
    {readln(a);}
    write(a);
    write(''#10'');
    if a < 0 then
    begin
        write('Faktorial nelze spocitat'#10'')
    end
    else
    begin

	write('else block'#10'');
        vysl := 1;
        while a > 0 do
        begin
	    write('Whilebody'#10'');
            vysl := vysl * a;
            a := a - 1
        end;
        write('Vysledek je: ');
        write(vysl);
	write( ''#10'')
    end
end.
