{Test na prazdny begin end}
var
	a : integer;
	vysl : integer;
begin
	{ Program 1: Vypocet faktorialu (iterativne) }
	write('Zadejte cislo pro vypocet faktorialu: ');
	readln(a);
	if a < 0 then
	begin
	end
	else
	begin
		vysl := 1;
		while a > 0 do
		begin
			vysl := vysl * a;
			a := a - 1
		end
		write('Vysledek je: ', vysl, ''#10'')
	end
end.