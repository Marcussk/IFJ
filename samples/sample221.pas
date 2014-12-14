{ unary minus with division}
var 
	a : integer;
	b : real;
 	c : real;
 	
begin
	a := -10 + (-5);
	b := -10.0;
	c := b/a;
 	write(a, ''#10'', b, ''#10'', c)
end.