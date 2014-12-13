{ test volania funkcii }
var
	num : integer;
	vysl : integer;

function readthis(n : integer) : integer;
var
	a : integer;
	sum : integer;
	num1 : integer;
	num2 : integer;
begin
	a := 0 ;
	num2 := 200;
	while 0 < n do 
	begin
		num1 := 113;
		sum := num1 + n;
		write('cislo: ', sum, ''#10'');
		n := n -1
	end;
	readthis := sum	
end;

begin
	num := 5 ;

	vysl := readthis(num);
	write('cislo: ', vysl, ''#10'')

end.		


