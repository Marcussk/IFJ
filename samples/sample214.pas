{}
var
	vyslo : integer;
	rel1 : real;
	rel2 : real;

function two(two :integer) : integer; forward;
function three( one(jedna) : integer; two(two) : integer ) : real; forward;
function one(jedna : integer) : integer;
begin
	one := 1;
end;

function two() : integer;
begin
	two := 2;
end;

function three( one(jedna : integer) : integer; two(two : integer) : integer ) : real;
begin
write("preadne hodnoty su :", one() , " ",  two() );	
three := rel2
end;


begin 
	vyslo := one(1) + two(2);
	write("vysledok : ", vyslo);

	rel2 := 1.2;
	rel1 := three( one(), two() );
	write("real : ", rel1 );


end.



