{premenna nemoze zacinat znakom 3}
var
	3aba : integer;
	vysl : integer;
begin
	{ Program 1: Vypocet faktorialu (iterativne) }
	write('Zadejte cislo pro vypocet faktorialu: ');
	{readln(a);}
	a:=14;
	if a < 0 then
	begin
		write('Faktorial nelze spocitat'#10'')
	end
	else
	begin
		vysl := 1;
		while 3aba > 0 do
		begin
			vysl := vysl * a;
			a := a - 1
		end;
		write('Vysledek je: ', vysl, ''#10'')
	end
end.
