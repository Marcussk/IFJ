{ unary minus }
var 
	a : integer;
 	
begin
	a := -(10 + (-1*(-5+3))); { Expected -12 }
 	write(a)
end.