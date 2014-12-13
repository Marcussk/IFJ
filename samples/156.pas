{ test volania funkcii }
var 
	vysl : integer;
	a : integer;

function print (a : integer) : integer;

begin
	print := 1 
end;

begin
	vysl := print (a);
	write('cislo: ', vysl , ''#10'')
end.		


