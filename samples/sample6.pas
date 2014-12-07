{Test na prazdny begin end}
var
	a : integer;
	vysl : integer;
begin
	{ Vypocet absolutnej hodnoty }
	write('Funkce abs:'#10'');
	a := 5;
	if a > 0 then
	begin
	end
	else
	begin
		a := a - a
	end;
        write('Vysledek je: ', a, ''#10'')
end.
