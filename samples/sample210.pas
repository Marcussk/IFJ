{ test volania funkcii }
var
	num : integer;
	vysl : integer;

function pluspat(n : integer) : integer;
var
	a : integer;
	sum : integer;
begin
	a := 5 ;
	sum := n + a;

	pluspat := sum
end;

begin
	num := 5 ;

	vysl := pluspat(num);
	write('cislo: ', vysl, ''#10'')

end.		


