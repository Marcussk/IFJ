{Test na cislicove literaly}
var	
	result : boolean;

begin
	result := ((2*(3+4*(5+6*(7+7))))) = (((((7+7)*6+5)*4+3)*2));
	write(result)
end.
